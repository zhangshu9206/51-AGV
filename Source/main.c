/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#include "stc_new_8051.h"
#include "motor.h"
#include "uart.h"
#include <stdio.h>
#include "type.h"
#include "timer.h"
#include "SelfTest.h"
extern void UART_send_byte(uint8 byte);
#define DEBUG 0//�Ƿ�����ӡ����


void main(void)
{
	#if DEBUG
	{
		TestApp_Init();//��ʼ����ˮ����ʱ
	}
	#endif
	UART_init();//���ڳ�ʼ

	Timer0_Init();//�����ʱ�������ɺ��ⷽ����
	Timer1_Init();//���ڽ���LED����
	Motor_Init();

    while(1)
	{
  		Cruising_Mod();//ģʽ����ִ���Ӻ���
	}
}
