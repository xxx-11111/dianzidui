/*
 * pid.c
 *
 *  Created on: 2023年10月20日
 *      Author: 胡屹松
 */

#include "pid.h"
#include "motor.h"
#include "zf_common_headfile.h"
//PID参数的结构体
//PID参数的结构体


PID speedpid_left;                          //赛道左轮速度环PID
PID speedpid_right;                         //赛道右轮速度环PID
//图像转向环PID
PID turnpid_image;//左转
float turnright_P = 12;//右转转向的P
float turnright_D = 0;//右转转向的D
float turngyro_right = 0.01;//右转角速度抑制的增益

//速度环前馈控制
FFC speedffc_left;
FFC speedffc_right;


/***********************************************
* @brief : 初始化PID参数
* @param : pid:PID参数的结构体指针
*          P:比例系数
*          I:积分项
*          D:微分项
*          out：输出值
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
* @brief : 电机速度环增量式PI控制
* @param : encoder_data:编码器的值
*          target_data:目标值
*          pid:电机度环的PID
* @return: out:给电机的PWM大小
* @date  : 2023.6.15
* @author: L & 刘骏帆
************************************************/
int PIDSpeed(int encoder_speed,int target_speed,PID* pid)
{
    float integral_out=0;
    pid->err = (float)(target_speed-encoder_speed);                                  //计算偏差
    //积分输出限幅
    integral_out=pid->I*pid->err;
    if(integral_out>pid->I*65)  integral_out=pid->I*65;
    else if(integral_out<-pid->I*65) integral_out=-pid->I*65;
    //PID计算
    pid->out += (int)(pid->P*(pid->err-pid->last_err)+integral_out);
    pid->last_err = pid->err;                                                        //保存上一次的值

    return pid->out;
}

/* @brief : 初始化前馈控制参数
* @param : ffc:前馈控制结构体
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
* @brief : 前馈控制
* @param : target:前馈控制输入
*          ffc:前馈控制结构体
* @return: ffc->output:前馈控制输出量
* @date  : 2023.6.7
* @author: L
************************************************/
int FeedForwardCtrl(int target,FFC* ffc)
{
    ffc->iuput = target;

    ffc->output = (int)(ffc->conf_1*(ffc->iuput-ffc->last_input)+ffc->conf_2*(ffc->iuput-2*ffc->last_input+ffc->pre_input))+ffc->conf_3;

    /*ffc->conf_1，ffc->conf_2 和 ffc->conf_3 是加权系数，用于调整当前输入(ffc->iuput)、前一次输入(ffc->last_input)和前两次输入(ffc->pre_input)的权重。
    ffc->conf_1*(ffc->iuput-ffc->last_input) 部分考虑了当前输入和前一次输入的差异。
    ffc->conf_2*(ffc->iuput-2*ffc->last_input+ffc->pre_input) 部分考虑了当前输入、前一次输入和前两次输入的差异。
    然后将这两个部分相加，并加上第三个系数ffc->conf_3，得到最终的输出值ffc->output。

    这种方法的优点是它可以适应不同的数据变化速度。例如，如果数据变化很快，那么你可以增加ffc->conf_1的值，减少ffc->conf_2的值，以减少对快速变化的响应。反之亦然，如果数据变化很慢，你可以减少ffc->conf_1的值，增加ffc->conf_2的值，以增强对缓慢变化的响应。

    同时，通过改变这些加权系数，这行代码可以实现多种类型的平滑，例如简单移动平均、指数平滑等。*/

    ffc->pre_input = ffc->last_input;
    ffc->last_input = ffc->iuput;

    if(ffc->output>5000) ffc->output=5000;
    else if(ffc->output<-5000) ffc->output=-5000;
    return ffc->output;
}

/***********************************************
* @brief : 串级pid
* @param : encoder_data:编码器的值
*          target_data:目标值
*          pid inner:内环
*          pid outer：外环
*          outerRef:外环编码器值
*          outerFdb：外环目标值
*          pid->outer.out：内环输入值
*          innerFdb：内环目标值
*
*
* @return: output:给电机的PWM大小
* @date  : 2023.10.20
* @author:胡屹松
************************************************/

void PID_CascadeCalc(CascadePID *pid,int outerRef,float outerFdb,float innerFdb)
{
    PIDSpeed(outerRef,outerFdb,&pid->outer);//计算外环
    PIDSpeed(pid->outer.out,innerFdb,&pid->inner);//计算内环
    pid->output=pid->inner.out;//内环输出就是串级PID的输出
}


/***********************************************
* @brief : 按键调参
* @param :key1 按键1
*         key2 按键2
*
* @return: output:给电机的PWM大小
* @date  : 2023.10.20
* @author:胡屹松
************************************************/





































