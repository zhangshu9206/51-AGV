/*
版权声明：
麦周老大(weibo.com/maizhoulaoda)版权所有(本程序在STC12系列上调试通过，收发都正常。)
基于赵学军先生在微计算机信息上的文章启示，修改一些错误并完善了该程序（修复bug例如recv中对停止位的处理等）。
您可以任意修改本程序，并应用于任何项目。禁止收取任何软件费用。使用时请保留本段声明标明版权情况。
用定时器 T0 模拟全双工串行口程序。由麦周老大，Michael Zhou基于现有半双工STC软件串口程序修改
最高波特率(12 clock/3):
    收、发波特率相同。

使用说明:
    1. 本程序使用一个定时器（使用3倍波特率的定时）和任意 2 个 I/O 口模拟一个串行口(如果MCU主频够快，懂得编成，完全可以修改成模拟多个串口)。
    2. 1位起始位，8位数据位，1位停止位。发数据位时先发低位。
    3. 支持全双工通讯。收、发波特率相同。
    4. 应把定时器中断优先级设置为最高级。
    5. 本程序使用收，发两个环行缓冲区。使用者不必关心缓冲区的处理。如果接收缓冲满，则停止接收新的Rx输入，直到有一个字节的空间被腾出。
**************************************************************************
编程说明:
----------------
定时器0使用自动重装的8位计数器模式，保证波特率精度。
----------------

发送:
	调用 rs_send_byte () 启动发送一个字节的过程。
    发送口平时为高电平，rs_send_byte ()函数使发送口变为低电平开始发送起始位; 
同时设置和启动定时器，为发送数据位在预定的时刻产生定时器中断。发送数据位和停止位都在定时器的中断服务程序中进行。
----------------
接收：
    定时器以 3 倍波特率的频率产生中断检测Rx I/O管脚收到的起始位，准备在下一个定时器中断中接收第 1 个数据位。
    中断服务程序中处理以下情况：
    1. 收到的是起始位: 调整定时器产生中断的频率与波特率相同。
    2. 收到第 8 位数据位: 存储接收到的字节。
    3. 收到第 1--7 位数据位: 存储到收、发移位暂存器。
    4. 收到停止位: 检测下一个起始位。 
    5. 处理出错的情况。
**************************************************************************

*/
#ifndef _SOFT_UART_H_
#define _SOFT_UART_H_


#include "config.h"			//请修改该行，使用合适的头文件。一般头文件只需要芯片寄存器/管脚描述就可以。
#include "type.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
sfr16	DPTR = 0x82;

//typedef	unsigned char	INT8U;
//typedef	unsigned int	INT16U;
//typedef	signed int	INT16;
//typedef	signed char	INT8;

#define	YES   1
#define	NO    0

//定义使用哪个定时器, 只可定义一个   
#define TIMER_0
//#define TIMER_1

//定义串口收、发送管脚。需要注意不要与其他管脚定义冲突
//sbit TxD1 = P2^0;
//sbit RxD1 = P2^1;
//sbit RxD2 = P3^6;	//如果需要两个9600 RxD，则添加这些变量。

//根据定时器确定参数  
//新一代 1T 8051系列 单片机定时器特殊功能寄存器
//                                            7     6     5     4     3     2     1     0     Reset Value
//sfr TCON = 0x88; //T0/T1 Control           TF1   TR1   TF0   TR0   IE1   IT1   IE0   IT0    0000,0000

//新一代 1T 8051系列 单片机中断特殊功能寄存器
//有的中断控制、中断标志位散布在其它特殊功能寄存器中，这些位在位地址中定义
//其中有的位无位寻址能力，请参阅 新一代 1T 8051系列 单片机中文指南
//                                             7     6     5    4     3    2    1    0   Reset Value
//sfr IE      = 0xA8;  //中断控制寄存器        EA  ELVD  EADC   ES   ET1  EX1  ET0  EX0  0x00,0000

//-----------------------
//                                            7     6     5    4    3    2    1    0    Reset Value
//sfr IP      = 0xB8; //中断优先级低位      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000

//                                          7     6       5      4     3      2      1      0   Reset Value
//sfr AUXR  = 0x8E; //Auxiliary Register  T0x12 T1x12 UART_M0x6 BRTR S2SMOD BRTx12 EXTRAM S1BRS  0000,0000

//-----------------------------------
//sfr TMOD = 0x89; //T0/T1 Modes             GATE1 C/T1  M1_1  M1_0  GATE0 C/T0  M0_1  M0_0   0000,0000

#ifdef TIMER_0
    #define TMOD_AND_WORD   0xF0
    #define TMOD_TIME_MODE2  0x02
	#define TMOD_TIME_MODE1  0x01
    #define TMOD_COUNT_MODE 0x05

	sbit  TCON_ENABLE_TIMER = TCON^4;  //TR0
    sbit  TCON_TFx = TCON^5;           //中断标志位  
    sbit  IE_ETx = IE^1;               //中断允许位为 ET0  
    sbit  IP_PTx = IP^1;               //中断优先级  
//    sfr TL0 = 0x8B;              //TL1
//    sfr TH0 = 0x8D;              //TH1  
#endif




//选择以下一个晶体频率
//#define Fosc 6000000                 //6MHz 
//#define Fosc 11059200                  //11.059MHz 
#define Fosc 22118400					//22.1184MHz
//#define Fosc 12000000
//#define Fosc 18432000
//#define Fosc 20000000
//#define Fosc 24000000
//#define Fosc 30000000
//#define Fosc 40000000

//选择以下一个波特率:
//#efine Baud 300                      //11.059MHz时，baud 最低为 300   
//#define Baud 1200
//#define Baud 2400
//#define Baud 4800
#define Baud 9600
//#define Baud 14400
//#define Baud 19200
//#define Baud 28800
//#define Baud 38400
//#define Baud 57600

#define BaudT (Fosc/Baud/3/12)					//Fosc=22.1184M, Baud=9600, BaudT=192/3=64
 
#define BufLong 32    //FIFO长度 

void Recv(void);
void Send(void);
void soft_rs232_interrupt( void );
void soft_receive_init();
extern void rs_send_byte(INT8U SendByte);
void soft_rs232_init(void);
extern void initiate_soft_uart (void);
void rs_Communication_Decode(void); 
extern INT8U rs_receive_byte(void);

#endif


