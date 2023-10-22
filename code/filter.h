/*
 * filter.h
 *
 *  Created on: 2023��10��20��
 *      Author: ������
 */

#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_



#ifndef CODE_FILTER_H_
#define CODE_FILTER_H_

#include "zf_common_headfile.h"

typedef struct
{
    int16 x;                    //Ԥ��ֵ
    float A;                    //x(k)=A*x(k-1)+B*u(k),u(k)~N(0,Q)
    float H;                    //z(k)=H*x(k)+w(k),w(k)~N(0,R)
    float Q;                    //Ԥ������Э����
    float R;                    //�������Э����
    float p;                    //�������Э����
    float gain;                 //����������
}kalman_filter_1;

extern kalman_filter_1 kalman_adc;
extern kalman_filter_1 kalman_gyro;


void KalmanInit(kalman_filter_1* state,float Q,float R);                //��ʼ���������˲���ϵ��
int16 KalmanFilter(kalman_filter_1* state,float z);                     //�������˲�
int SecondOrderFilter_L(int value);                                     //����������׵�ͨ�˲�
int SecondOrderFilter_R(int value);                                     //�ұ��������׵�ͨ�˲�

#endif /* CODE_FILTER_H_ */




#endif /* CODE_FILTER_H_ */
