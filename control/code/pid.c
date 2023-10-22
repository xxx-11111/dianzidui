/*
 * pid.c
 *
 *  Created on: 2023��10��20��
 *      Author: ������
 */

#include "pid.h"
#include "motor.h"
#include "zf_common_headfile.h"
//PID�����Ľṹ��
//PID�����Ľṹ��


PID speedpid_left;                          //���������ٶȻ�PID
PID speedpid_right;                         //���������ٶȻ�PID
//ͼ��ת��PID
PID turnpid_image;//��ת
float turnright_P = 12;//��תת���P
float turnright_D = 0;//��תת���D
float turngyro_right = 0.01;//��ת���ٶ����Ƶ�����

//�ٶȻ�ǰ������
FFC speedffc_left;
FFC speedffc_right;


/***********************************************
* @brief : ��ʼ��PID����
* @param : pid:PID�����Ľṹ��ָ��
*          P:����ϵ��
*          I:������
*          D:΢����
*          out�����ֵ
* @return: void
* @date  : 2023.1.9
* @author: L
************************************************/
void PIDInit(PID* pid,float P,float I,float D)
{
    pid->P = P;
    pid->I = I;
    pid->D = D;
    pid->err = 0;
    pid->last_err = 0;
    pid->out = 0;
}
/***********************************************
* @brief : ����ٶȻ�����ʽPI����
* @param : encoder_data:��������ֵ
*          target_data:Ŀ��ֵ
*          pid:����Ȼ���PID
* @return: out:�������PWM��С
* @date  : 2023.6.15
* @author: L & ������
************************************************/
int PIDSpeed(int encoder_speed,int target_speed,PID* pid)
{
    float integral_out=0;
    pid->err = (float)(target_speed-encoder_speed);                                  //����ƫ��
    //��������޷�
    integral_out=pid->I*pid->err;
    if(integral_out>pid->I*65)  integral_out=pid->I*65;
    else if(integral_out<-pid->I*65) integral_out=-pid->I*65;
    //PID����
    pid->out += (int)(pid->P*(pid->err-pid->last_err)+integral_out);
    pid->last_err = pid->err;                                                        //������һ�ε�ֵ

    return pid->out;
}

/* @brief : ��ʼ��ǰ�����Ʋ���
* @param : ffc:ǰ�����ƽṹ��
*          conf_1:
*          conf_2:
*          conf_3:
* @return: void
* @date  : 2023.6.7
* @author: L
************************************************/
void FFCInit(FFC* ffc,float conf_1,float conf_2,float conf_3)
{
    ffc->conf_1 = conf_1;
    ffc->conf_2 = conf_2;
    ffc->conf_3 = conf_3;
    ffc->iuput = 0;
    ffc->last_input = 0;
    ffc->pre_input = 0;
    ffc->output = 0;
}
/***********************************************
* @brief : ǰ������
* @param : target:ǰ����������
*          ffc:ǰ�����ƽṹ��
* @return: ffc->output:ǰ�����������
* @date  : 2023.6.7
* @author: L
************************************************/
int FeedForwardCtrl(int target,FFC* ffc)
{
    ffc->iuput = target;

    ffc->output = (int)(ffc->conf_1*(ffc->iuput-ffc->last_input)+ffc->conf_2*(ffc->iuput-2*ffc->last_input+ffc->pre_input))+ffc->conf_3;

    /*ffc->conf_1��ffc->conf_2 �� ffc->conf_3 �Ǽ�Ȩϵ�������ڵ�����ǰ����(ffc->iuput)��ǰһ������(ffc->last_input)��ǰ��������(ffc->pre_input)��Ȩ�ء�
    ffc->conf_1*(ffc->iuput-ffc->last_input) ���ֿ����˵�ǰ�����ǰһ������Ĳ��졣
    ffc->conf_2*(ffc->iuput-2*ffc->last_input+ffc->pre_input) ���ֿ����˵�ǰ���롢ǰһ�������ǰ��������Ĳ��졣
    Ȼ��������������ӣ������ϵ�����ϵ��ffc->conf_3���õ����յ����ֵffc->output��

    ���ַ������ŵ�����������Ӧ��ͬ�����ݱ仯�ٶȡ����磬������ݱ仯�ܿ죬��ô���������ffc->conf_1��ֵ������ffc->conf_2��ֵ���Լ��ٶԿ��ٱ仯����Ӧ����֮��Ȼ��������ݱ仯����������Լ���ffc->conf_1��ֵ������ffc->conf_2��ֵ������ǿ�Ի����仯����Ӧ��

    ͬʱ��ͨ���ı���Щ��Ȩϵ�������д������ʵ�ֶ������͵�ƽ����������ƶ�ƽ����ָ��ƽ���ȡ�*/

    ffc->pre_input = ffc->last_input;
    ffc->last_input = ffc->iuput;

    if(ffc->output>5000) ffc->output=5000;
    else if(ffc->output<-5000) ffc->output=-5000;
    return ffc->output;
}

/***********************************************
* @brief : ����pid
* @param : encoder_data:��������ֵ
*          target_data:Ŀ��ֵ
*          pid inner:�ڻ�
*          pid outer���⻷
*          outerRef:�⻷������ֵ
*          outerFdb���⻷Ŀ��ֵ
*          pid->outer.out���ڻ�����ֵ
*          innerFdb���ڻ�Ŀ��ֵ
*
*
* @return: output:�������PWM��С
* @date  : 2023.10.20
* @author:������
************************************************/

void PID_CascadeCalc(CascadePID *pid,int outerRef,float outerFdb,float innerFdb)
{
    PIDSpeed(outerRef,outerFdb,&pid->outer);//�����⻷
    PIDSpeed(pid->outer.out,innerFdb,&pid->inner);//�����ڻ�
    pid->output=pid->inner.out;//�ڻ�������Ǵ���PID�����
}


/***********************************************
* @brief : ��������
* @param :key1 ����1
*         key2 ����2
*
* @return: output:�������PWM��С
* @date  : 2023.10.20
* @author:������
************************************************/





































