/*
 * motor.c
 *
 *  Created on: 2023年10月20日
 *      Author: 胡屹松
 */
#include "motor.h"
#include "zf_common_headfile.h"
#include "filter.h"

int speed_left = 0,speed_right = 0;                                     //左右轮当前编码器的值
int target_left = 0,target_right = 0;                                   //左右轮的目标速度的值
uint16 base_speed = 0;                                                  //基础速度

uint8 encoder_dis_flag = 0;                                             //编码器积分标志位
float dis = 0;                                                          //距离



void EncoderInit(void)
{
    encoder_quad_init(ENCODER_RIGHT,ENCODER_RIGHT_A,ENCODER_RIGHT_B);       //初始化右边编码器模块
    encoder_quad_init(ENCODER_LEFT,ENCODER_LEFT_A,ENCODER_LEFT_B);          //初始化左边编码器模块

}

/***********************************************
* @brief : 获取左右两个编码器的值
* @param : data_left:存取左编码器的值
*          data_right:存取右编码器的值
* @return: void
* @date  : 2023.1.2
* @author: L
************************************************/
void EncoderGetCount(int* data_left,int* data_right)
{
    int last_data_left = *data_left,last_data_right = *data_right;

    *data_left = encoder_get_count(ENCODER_LEFT);                            //获取左边编码器的值
    *data_right = -encoder_get_count(ENCODER_RIGHT);                         //获取右边编码器的值

    *data_left = SecondOrderFilter_L(*data_left);                           //左编码器一阶低通滤波
    *data_right = SecondOrderFilter_R(*data_right);                         //右编码器一阶低通滤波
    //限幅
    if(*data_left > 255 || *data_left < -255) *data_left = last_data_left;
    if(*data_right > 255 || *data_right < -255) *data_right = last_data_right;

    encoder_clear_count(ENCODER_LEFT);                                      //清空左边编码器计数
    encoder_clear_count(ENCODER_RIGHT);                                     //清空右边编码器计数

    //编码器积分
    if(encoder_dis_flag == 1)
    {
        float data_mid = 0;
        data_mid = (float)(*data_left+*data_right)/2;

        dis += ((data_mid/1024.0)*30.0/68.0)*(float)CIRCLE;
    }
    else if(encoder_dis_flag == 0)//清零积分的距离
        dis = 0;
}


/***********************************************
* @brief : 初始化左右两个电机
* @param : void
* @return: void
* @date  : 2023.1.5
* @author: L
************************************************/
void MotorInit(void)
{
    pwm_init(MOTOR_LEFT_1,12500,0);                   //初始化左电机
    //pwm_init(MOTOR_LEFT_2,12500,0);                   //初始化左电机
    gpio_init(MOTOR_LEFT_2, GPO, 0, GPO_PUSH_PULL);
    pwm_init(MOTOR_RIGHT_1,12500,0);                  //初始化右电机
    gpio_init(MOTOR_RIGHT_2, GPO, 0, GPO_PUSH_PULL);
    //pwm_init(MOTOR_RIGHT_2,12500,0);                  //初始化右电机
}
/***********************************************
* @brief : 控制左右两个电机的转速和正反转,采用H桥的单极模式
* @param : pwm_left:给左电机的pwm大小，控制左电机转速
*          pwm_right:给右电机的pwm大小，控制右电机转速
* @return: void
* @date  : 2023.1.5
* @author: L
************************************************/
void MotorSetPWM(int pwm_left,int pwm_right)
{
    //对输入电机的pwm进行限幅
    if(pwm_left>MOTOR_PWM_MAX)
        pwm_left = MOTOR_PWM_MAX;
    else if(pwm_left<-MOTOR_PWM_MAX)
        pwm_left = -MOTOR_PWM_MAX;
    if(pwm_right>MOTOR_PWM_MAX)
        pwm_right = MOTOR_PWM_MAX;
    else if(pwm_right<-MOTOR_PWM_MAX)
        pwm_right = -MOTOR_PWM_MAX;
    //控制电机正反转和转速
    //左电机
    gpio_set_level(MOTOR_LEFT_2, 1);
    pwm_set_duty(MOTOR_LEFT_1,5000+(int)(pwm_left/2));
    //右电机
    gpio_set_level(MOTOR_RIGHT_2, 1);
    pwm_set_duty(MOTOR_RIGHT_1,5000-(int)(pwm_right/2));
}

