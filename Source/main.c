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
#include <string.h>
#include "type.h"
#include "timer.h"
#include "SelfTest.h"
#include "Soft_uart.h"

INT8U xdata ReceivedData;
#define DEBUG 1//�Ƿ�����ӡ����

void main(void)
{
	
    #if DEBUG
    {
		TestApp_Init();//��ʼ����ˮ����ʱ
    }
    #endif

    UART_init();//���ڳ�ʼ
	
//  Timer0_Init();//�����ʱ�������ɺ��ⷽ����
    Timer1_Init();//���ڽ���LED����
    Motor_Init();

    UART_send("UART_init OK", strlen("UART_init OK")); 

    initiate_soft_uart();//1�����ڳ�ʼ 2�������ʼ��������������ţ�
//  UART_send("soft_UART_init OK", strlen("soft_UART_init OK"));

    while(1)
	{
  		Cruising_Mod();//ģʽ����ִ���Ӻ���
        ReceivedData=rs_receive_byte();
        rs_send_byte(ReceivedData);
	}
}
