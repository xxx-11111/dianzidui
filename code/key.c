/*
 * key.c
 *
 *  Created on: 2023��10��21��
 *      Author: ������
 */
#include "pid.h"
#include "motor.h"
#include "zf_common_headfile.h"
#include "zf_driver_gpio.h"
#include "key.h"

#define LOOSE 0
//��ʼ������
void KEYInit(void)
{
    gpio_init(P33_10,GPI,LOOSE,GPI_PULL_DOWN);
    gpio_init(P33_11,GPI,LOOSE,GPI_PULL_DOWN);
}

void key1_handler(CascadePID *pid)
{
    if (key1 == 0) {
        // ���°���1���޸��⻷��P��I����
        // ����Ҫ����ʵ���������þ�����޸��߼�
        pid->outer.P += 0.1; // ����ÿ�ΰ������������⻷��Pֵ0.1
        pid->outer.I += 0.05; // ����ÿ�ΰ������������⻷��Iֵ0.05
        key1 = 1; // ��ǰ���1�Ѱ���
    } else {
        key1 = 0; // ��ǰ���1���ɿ�
    }
}

void key2_handler(CascadePID *pid)
{
    if (key2 == 0) {
        // ���°���2���޸��ڻ���P��I����
        // ����Ҫ����ʵ���������þ�����޸��߼�
        pid->inner.P += 0.05; // ����ÿ�ΰ������������ڻ���Pֵ0.05
        pid->inner.I += 0.02; // ����ÿ�ΰ������������ڻ���Iֵ0.02
        key2 = 1; // ��ǰ���2�Ѱ���
    } else {
        key2 = 0; // ��ǰ���2���ɿ�
    }
}
//�߼�������


