/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stc_new_8051.h"

/* 直流电机管脚配置 */
sbit MOTOR_A_CON1=P0^0;
sbit MOTOR_A_CON2=P0^2;
sbit MOTOR_A_EN=P0^1;

sbit MOTOR_B_CON1=P0^3;
sbit MOTOR_B_CON2=P0^5;
sbit MOTOR_B_EN=P0^4;
//sbit MAINLIGHT_CON=P2^2;

/* 超声波管脚配置 */
sbit Echo=P3^4;
sbit Trig=P3^5;


/* 舵机管脚配置 */
/*
sbit SE1=P1^0;
sbit SE2=P1^1;
sbit SE3=P1^2;
sbit SE4=P1^3;
sbit SE5=P1^4;
sbit SE6=P1^5;
sbit SE7=P1^6;
sbit SE8=P1^7;
*/
#define SE1_Deg  90 //舵机1上电默认角度
#define SE2_Deg  45 //舵机2上电默认角度
#define SE3_Deg  67 //舵机3上电默认角度
#define SE4_Deg  67 //舵机4上电默认角度
#define SE5_Deg  90 //舵机5上电默认角度
#define SE6_Deg  90 //舵机6上电默认角度
#define SE7_Deg  60 //舵机7上电默认角度
#define SE8_Deg  90 //舵机8上电默认角度

/* LED管脚配置 */
sbit LED6=P0^6;
sbit LED5=P0^7;
sbit LED4=P2^7;
sbit LED3=P2^6;
sbit LED2=P2^5;
sbit LED1=P2^4;

/* 软串口管脚配置 */
sbit TxD1 = P2^0;
sbit RxD1 = P2^1;

/* 跟随管脚配置 
sbit Input_Detect0=P3^7;	  //右侧探头
sbit Input_Detect1=P3^3;	 //距离探测
sbit Input_Detect2=P2^3;     //左侧探头 P2^1
*/

/* 寻迹管脚配置 */
sbit Input_Detect_LEFT=P3^7;	  //右侧探头 
sbit Input_Detect_center=P3^3;    //中间探头 
sbit Input_Detect_RIGHT=P2^3;     //左侧探头

/*避障管脚配置*/
sbit Input_Detect1=P3^6; 

/* 红外管脚配置 */
sbit IR_T=P2^2;
sbit IR_R=P3^2;

/* 串口配置 */
/*晶振为22.1184MHz 以下为可选波特率*/
#define BPS_9600     0x70  
#define BPS_19200	 0xB8
#define BPS_38400	 0xDC
#define BPS_57600	 0xE8
#define BPS_115200	 0xF4

#define RELOAD_COUNT   BPS_9600	//波特率选择

#endif