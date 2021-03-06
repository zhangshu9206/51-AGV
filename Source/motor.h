/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "type.h"
#include "config.h"
#include "uart.h"

void Motor_Init(void);//声明电机驱动函数
void Follow_Track(void);
void FollowLine(void);
void Avoiding(void);
void Send_wave(void);
uchar Get_Distance(void);
void AvoidByRadar(void);
void Send_Distance(void);
void Cruising_Mod(void);
void Delay_ForBarrier(uint32);//声明延时函数
//extern void rs_UART_send(uint8 * rs_Buffer, uint16 rs_Length); 

#define MOTOR_GO_FORWARD   MOTOR_A_CON1=1;MOTOR_A_CON2=0;MOTOR_B_CON1=1;MOTOR_B_CON2=0;LED1=0;LED2=1;LED3=1;LED4=1 //车体前进	                            
#define MOTOR_GO_BACK	   MOTOR_A_CON1=0;MOTOR_A_CON2=1;MOTOR_B_CON1=0;MOTOR_B_CON2=1;LED1=1;LED2=0;LED3=1;LED4=1 //车体后退
#define MOTOR_GO_RIGHT	   MOTOR_A_CON1=0;MOTOR_A_CON2=1;MOTOR_B_CON1=1;MOTOR_B_CON2=0;LED1=1;LED2=1;LED3=0;LED4=1 //车体左转
#define MOTOR_GO_LEFT	   MOTOR_A_CON1=1;MOTOR_A_CON2=0;MOTOR_B_CON1=0;MOTOR_B_CON2=1;LED1=1;LED2=1;LED3=1;LED4=0 //车体右转
#define MOTOR_GO_STOP	   MOTOR_A_CON1=0;MOTOR_A_CON2=0;MOTOR_B_CON1=0;MOTOR_B_CON2=0;LED1=1;LED2=1;LED3=1;LED4=1 //车体停止
#define IR_SEND_LED_ON	 LED4=1;LED5=0;LED6=1
#define IR_SEND_LED_OFF	 LED4=0;LED5=1;LED6=0;


#endif

