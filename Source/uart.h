/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#ifndef _UART_H_
#define _UART_H_

#include "type.h"
#include "stc_new_8051.h"

extern void UART_init(void);  // ��ʼ������
extern void UART_send_byte(uint8 byte); // ���ڷ����ֽ�����
extern void UART_send(uint8 *Buffer, uint8 Length); // ���ڷ�����������


#endif