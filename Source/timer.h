/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ����������Ĳȡ������
By WIFI���������������˴��⹤����
*/
#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"
#include "type.h"

//extern uint8 rec_flag;
extern uint8 rec_flag;
extern uint16 Sampling_cnt;
extern bit IR_38K_En;
extern bit IR_R_T;
extern bit IR_EN; 
extern void Steering_Engine_Control(void);

void Delay_Ms(uint32 t);
void Timer0_Init(void);
void Timer1_Init(void);

#endif