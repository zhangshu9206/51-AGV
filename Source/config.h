/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stc_new_8051.h"

/* ֱ������ܽ����� */
sbit MOTOR_A_CON1=P0^0;
sbit MOTOR_A_CON2=P0^2;
sbit MOTOR_A_EN=P0^1;

sbit MOTOR_B_CON1=P0^3;
sbit MOTOR_B_CON2=P0^5;
sbit MOTOR_B_EN=P0^4;
//sbit MAINLIGHT_CON=P2^2;

/* �������ܽ����� */
sbit Echo=P3^4;
sbit Trig=P3^5;


/* ����ܽ����� */
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
#define SE1_Deg  90 //���1�ϵ�Ĭ�ϽǶ�
#define SE2_Deg  45 //���2�ϵ�Ĭ�ϽǶ�
#define SE3_Deg  67 //���3�ϵ�Ĭ�ϽǶ�
#define SE4_Deg  67 //���4�ϵ�Ĭ�ϽǶ�
#define SE5_Deg  90 //���5�ϵ�Ĭ�ϽǶ�
#define SE6_Deg  90 //���6�ϵ�Ĭ�ϽǶ�
#define SE7_Deg  60 //���7�ϵ�Ĭ�ϽǶ�
#define SE8_Deg  90 //���8�ϵ�Ĭ�ϽǶ�

/* LED�ܽ����� */
sbit LED6=P0^6;
sbit LED5=P0^7;
sbit LED4=P2^7;
sbit LED3=P2^6;
sbit LED2=P2^5;
sbit LED1=P2^4;


/* ����ܽ����� */
sbit Input_Detect0=P3^7;	  //�Ҳ�̽ͷ
sbit Input_Detect1=P3^3;	 //����̽��
sbit Input_Detect2=P2^1;	 //���̽ͷ

/* ���Ϲܽ����� */
sbit Input_Detect_LEFT=P2^0;	  //�Ҳ�̽ͷ
sbit Input_Detect_RIGHT=P2^3;	 //���̽ͷ


/* ����ܽ����� */
sbit IR_T=P2^2;
sbit IR_R=P3^2;

/* �������� */
/*����Ϊ22.1184MHz ����Ϊ��ѡ������*/
#define BPS_9600     0x70  
#define BPS_19200	 0xB8
#define BPS_38400	 0xDC
#define BPS_57600	 0xE8
#define BPS_115200	 0xF4

#define RELOAD_COUNT   BPS_9600	//������ѡ��

#endif