  /*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#include "stc_new_8051.h"
#include "timer.h"
#include "steer.h"
#include "IR.h"

extern uint8 rec_flag;
extern uint8 rec_flag;
extern uint16 Sampling_cnt;
extern bit IR_38K_En;
extern bit IR_R_T; 
extern bit IR_EN;
extern void Steering_Engine_Control(void);
uint16 se_timer[8]={SE1_Deg,SE2_Deg,SE3_Deg,SE4_Deg,SE5_Deg,SE6_Deg,SE7_Deg,SE8_Deg};
/* ms ��ʱ */
void Delay_Ms(uint32 t)
{  
    uint16 i;
	while(t--)
	{
	   for(i=0;i<1050;i++);
	}
}

void Timer0_Init(void)
{
	TMOD   &=   0xf0; 
    TMOD   |=   0x01; 
	AUXR   |=   0XC0;
	IP   |=   0x02;//��ʱ��0�ж����ȼ����
	TR0=1;
	ET0=1;
}

void Timer1_Init(void)
{
    ET1     =	  1;
    TMOD   &=   0x0f; 
    TMOD   |=   0x10;   
    TH1     =   0xA9;   //1MS��ʱ
    TL1     =   0x9A;
    TR1     =   1;    //����ʱ��1

}

void Timer_0(void) interrupt 1 
{
	if(IR_EN ==1)
	{
		Timer_For_IR();
	}
	else
	{
		Steering_Engine_Control();
	}
}

void Timer_1(void) interrupt 3
{
    static uint16 ms_count=0;

	TH1     =   0xA9;   
    TL1     =   0x9A;

	if(ms_count++ >= 1000)
	{
	     ms_count = 0;
		 if(rec_flag)
		 {
		     LED5 = ~LED5;
		 }
		 else
		 {
		     LED5 = 1;
		 }
	}
	
}