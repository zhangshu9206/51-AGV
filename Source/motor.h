/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "type.h"
#include "config.h"

void Motor_Init(void);//���������������
void Delay_ForBarrier(uint32);//������ʱ����
#define MOTOR_GO_FORWARD   MOTOR_A_CON1=1;MOTOR_A_CON2=0;MOTOR_B_CON1=1;MOTOR_B_CON2=0;LED1=0;LED2=1;LED3=1;LED4=1 //����ǰ��	                            
#define MOTOR_GO_BACK	   MOTOR_A_CON1=0;MOTOR_A_CON2=1;MOTOR_B_CON1=0;MOTOR_B_CON2=1;LED1=1;LED2=0;LED3=1;LED4=1 //�������
#define MOTOR_GO_RIGHT	   MOTOR_A_CON1=0;MOTOR_A_CON2=1;MOTOR_B_CON1=1;MOTOR_B_CON2=0;LED1=1;LED2=1;LED3=0;LED4=1 //������ת
#define MOTOR_GO_LEFT	   MOTOR_A_CON1=1;MOTOR_A_CON2=0;MOTOR_B_CON1=0;MOTOR_B_CON2=1;LED1=1;LED2=1;LED3=1;LED4=0 //������ת
#define MOTOR_GO_STOP	   MOTOR_A_CON1=0;MOTOR_A_CON2=0;MOTOR_B_CON1=0;MOTOR_B_CON2=0;LED1=1;LED2=1;LED3=1;LED4=1 //����ֹͣ
#define IR_SEND_LED_ON	 LED4=1;LED5=0;LED6=1
#define IR_SEND_LED_OFF	 LED4=0;LED5=1;LED6=0;
#endif

