/*
 * key.c
 *
 *  Created on: 2023年10月21日
 *      Author: 胡屹松
 */
#include "pid.h"
#include "motor.h"
#include "zf_common_headfile.h"
#include "zf_driver_gpio.h"
#include "key.h"

#define LOOSE 0
//初始化按键
void KEYInit(void)
{
    gpio_init(P33_10,GPI,LOOSE,GPI_PULL_DOWN);
    gpio_init(P33_11,GPI,LOOSE,GPI_PULL_DOWN);
}

void key1_handler(CascadePID *pid)
{
    if (key1 == 0) {
        // 按下按键1，修改外环的P或I参数
        // 你需要根据实际需求设置具体的修改逻辑
        pid->outer.P += 0.1; // 假设每次按键按下增加外环的P值0.1
        pid->outer.I += 0.05; // 假设每次按键按下增加外环的I值0.05
        key1 = 1; // 标记按键1已按下
    } else {
        key1 = 0; // 标记按键1已松开
    }
}

void key2_handler(CascadePID *pid)
{
    if (key2 == 0) {
        // 按下按键2，修改内环的P或I参数
        // 你需要根据实际需求设置具体的修改逻辑
        pid->inner.P += 0.05; // 假设每次按键按下增加内环的P值0.05
        pid->inner.I += 0.02; // 假设每次按键按下增加内环的I值0.02
        key2 = 1; // 标记按键2已按下
    } else {
        key2 = 0; // 标记按键2已松开
    }
}
//逻辑有问题


