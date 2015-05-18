/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于向他人牟取暴利。
By WIFI机器人网·机器人创意工作室
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"
#include "type.h"

void Delay_Ms(uint32 t);
void Timer0_Init(void);
void Timer1_Init(void);

#endif