/*
 * key.h
 *
 *  Created on: 2023Äê10ÔÂ21ÈÕ
 *      Author: ºúÒÙËÉ
 */

#ifndef CODE_KEY_H_
#define CODE_KEY_H_

#include "pid.h"
#include "motor.h"
#include "zf_common_headfile.h"
#include "zf_driver_gpio.h"

#define key1 gpio_get_level(P33_10)
#define key2 gpio_get_level(P33_11)

void key1_handler(CascadePID *pid);
void key2_handler(CascadePID *pid);


#endif /* CODE_KEY_H_ */
