/*
��Ȩ������
�����ϴ�(weibo.com/maizhoulaoda)��Ȩ����(��������STC12ϵ���ϵ���ͨ�����շ���������)
������ѧ��������΢�������Ϣ�ϵ�������ʾ���޸�һЩ���������˸ó����޸�bug����recv�ж�ֹͣλ�Ĵ���ȣ���
�����������޸ı����򣬲�Ӧ�����κ���Ŀ����ֹ��ȡ�κ�������á�ʹ��ʱ�뱣����������������Ȩ�����
�ö�ʱ�� T0 ģ��ȫ˫�����пڳ����������ϴ�Michael Zhou�������а�˫��STC������ڳ����޸�
��߲�����(12 clock/3):
    �ա�����������ͬ��

ʹ��˵��:
    1. ������ʹ��һ����ʱ����ʹ��3�������ʵĶ�ʱ�������� 2 �� I/O ��ģ��һ�����п�(���MCU��Ƶ���죬���ñ�ɣ���ȫ�����޸ĳ�ģ��������)��
    2. 1λ��ʼλ��8λ����λ��1λֹͣλ��������λʱ�ȷ���λ��
    3. ֧��ȫ˫��ͨѶ���ա�����������ͬ��
    4. Ӧ�Ѷ�ʱ���ж����ȼ�����Ϊ��߼���
    5. ������ʹ���գ����������л�������ʹ���߲��ع��Ļ������Ĵ���������ջ���������ֹͣ�����µ�Rx���룬ֱ����һ���ֽڵĿռ䱻�ڳ���
**************************************************************************
���˵��:
----------------
��ʱ��0ʹ���Զ���װ��8λ������ģʽ����֤�����ʾ��ȡ�
----------------

����:
	���� rs_send_byte () ��������һ���ֽڵĹ��̡�
    ���Ϳ�ƽʱΪ�ߵ�ƽ��rs_send_byte ()����ʹ���Ϳڱ�Ϊ�͵�ƽ��ʼ������ʼλ; 
ͬʱ���ú�������ʱ����Ϊ��������λ��Ԥ����ʱ�̲�����ʱ���жϡ���������λ��ֹͣλ���ڶ�ʱ�����жϷ�������н��С�
----------------
���գ�
    ��ʱ���� 3 �������ʵ�Ƶ�ʲ����жϼ��Rx I/O�ܽ��յ�����ʼλ��׼������һ����ʱ���ж��н��յ� 1 ������λ��
    �жϷ�������д������������
    1. �յ�������ʼλ: ������ʱ�������жϵ�Ƶ���벨������ͬ��
    2. �յ��� 8 λ����λ: �洢���յ����ֽڡ�
    3. �յ��� 1--7 λ����λ: �洢���ա�����λ�ݴ�����
    4. �յ�ֹͣλ: �����һ����ʼλ�� 
    5. �������������
**************************************************************************

*/
#ifndef _SOFT_UART_H_
#define _SOFT_UART_H_


#include "config.h"			//���޸ĸ��У�ʹ�ú��ʵ�ͷ�ļ���һ��ͷ�ļ�ֻ��ҪоƬ�Ĵ���/�ܽ������Ϳ��ԡ�
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

//����ʹ���ĸ���ʱ��, ֻ�ɶ���һ��   
#define TIMER_0
//#define TIMER_1

//���崮���ա����͹ܽš���Ҫע�ⲻҪ�������ܽŶ����ͻ
//sbit TxD1 = P2^0;
//sbit RxD1 = P2^1;
//sbit RxD2 = P3^6;	//�����Ҫ����9600 RxD���������Щ������

//���ݶ�ʱ��ȷ������  
//��һ�� 1T 8051ϵ�� ��Ƭ����ʱ�����⹦�ܼĴ���
//                                            7     6     5     4     3     2     1     0     Reset Value
//sfr TCON = 0x88; //T0/T1 Control           TF1   TR1   TF0   TR0   IE1   IT1   IE0   IT0    0000,0000

//��һ�� 1T 8051ϵ�� ��Ƭ���ж����⹦�ܼĴ���
//�е��жϿ��ơ��жϱ�־λɢ�����������⹦�ܼĴ����У���Щλ��λ��ַ�ж���
//�����е�λ��λѰַ����������� ��һ�� 1T 8051ϵ�� ��Ƭ������ָ��
//                                             7     6     5    4     3    2    1    0   Reset Value
//sfr IE      = 0xA8;  //�жϿ��ƼĴ���        EA  ELVD  EADC   ES   ET1  EX1  ET0  EX0  0x00,0000

//-----------------------
//                                            7     6     5    4    3    2    1    0    Reset Value
//sfr IP      = 0xB8; //�ж����ȼ���λ      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000

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
    sbit  TCON_TFx = TCON^5;           //�жϱ�־λ  
    sbit  IE_ETx = IE^1;               //�ж�����λΪ ET0  
    sbit  IP_PTx = IP^1;               //�ж����ȼ�  
//    sfr TL0 = 0x8B;              //TL1
//    sfr TH0 = 0x8D;              //TH1  
#endif




//ѡ������һ������Ƶ��
//#define Fosc 6000000                 //6MHz 
//#define Fosc 11059200                  //11.059MHz 
#define Fosc 22118400					//22.1184MHz
//#define Fosc 12000000
//#define Fosc 18432000
//#define Fosc 20000000
//#define Fosc 24000000
//#define Fosc 30000000
//#define Fosc 40000000

//ѡ������һ��������:
//#efine Baud 300                      //11.059MHzʱ��baud ���Ϊ 300   
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
 
#define BufLong 32    //FIFO���� 

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


