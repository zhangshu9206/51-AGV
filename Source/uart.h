/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#ifndef _UART_H_
#define _UART_H_

#include "type.h"
#include "stc_new_8051.h"

extern bit IS_CON;
extern void UART_init(void);  // 初始化串口
extern void UART_send_byte(uint8 byte); // 串口发送字节数据
extern void UART_send(uint8 * Buffer, uint16 Length); // 串口发送数组数据


#endif