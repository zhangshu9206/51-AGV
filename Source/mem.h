/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#ifndef _MEM_H_
#define _MEM_H_

#include "type.h"
#include "stc_new_8051.h"
uchar Read_Byte(uint Addr);
void Write_Byte(uint Addr,uchar My_Data);
void Earse_Section(uint Addr);
uint Read_Word(uint Addr);
void Write_Word(uint Addr,uint My_Data);

#endif