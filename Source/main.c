/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#include "stc_new_8051.h"
#include "motor.h"
#include "uart.h"
#include <stdio.h>
#include "type.h"
#include "timer.h"
#include "SelfTest.h"
extern void UART_send_byte(uint8 byte);
#define DEBUG 0//是否开启打印开关


void main(void)
{
	#if DEBUG
	{
		TestApp_Init();//初始化流水灯延时
	}
	#endif
	UART_init();//串口初始

	Timer0_Init();//舵机定时（或生成红外方波）
	Timer1_Init();//串口接收LED闪动
	Motor_Init();

    while(1)
	{
  		Cruising_Mod();//模式功能执行子函数
	}
}
