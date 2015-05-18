/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/
#include "motor.h"
#include "uart.h"
#include "timer.h"
#include "mem.h"
uchar Cruising_Flag =0x00;
uchar  Robots_Run_Status;
uchar Pre_Cruising_Flag = 0x00;

/**�����ʼ��**/
void Motor_Init(void)
{
	MOTOR_A_EN=1;//A�˵��ʹ��
	MOTOR_B_EN=1;//B�˵��ʹ��
	MOTOR_GO_STOP;
}
//����ģʽ
void Follow_Track(void)
{
  		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		
	     if(Input_Detect1 == 1)	//�м䴫����OK
		  {
		  
		   	if((Input_Detect0 == 0)&& (Input_Detect2 == 0)) //����ͬʱ̽�⵽�ϰ���
			{
			
				 Robots_Run_Status=0x04;//ֹͣ 
			} 
			 
			if((Input_Detect0 == 0)&& (Input_Detect2 == 1))//����ϰ���
			{
			
				Robots_Run_Status=0x01;//��ת 
			}
		
			if((Input_Detect0 == 1)&& (Input_Detect2 == 0))//�Ҳ��ϰ���
			{
				Robots_Run_Status=0x02;//��ת 
			}
			 
			if((Input_Detect0 == 1)&& (Input_Detect2 == 1))//���κ��ϰ���
			{
				Robots_Run_Status=0x03;//ֱ�� 
			}
		  }
		  else
		  {
		  	   Robots_Run_Status=0x04;//�����ֹͣ
		  }

}
//Ѳ��ģʽ
void FollowLine(void)
{
   		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		   	if((Input_Detect_LEFT == 0)&& (Input_Detect_RIGHT == 0))//����ͬʱ̽�⵽�ϰ���
			{
			
				 Robots_Run_Status=0x03;//ֱ�� 
			} 
			 
			if((Input_Detect_LEFT == 0)&& (Input_Detect_RIGHT == 1))//�Ҳ������ϰ�  
			{
			
				Robots_Run_Status=0x02;//��ת 
			}
		
			if((Input_Detect_LEFT == 1)&& (Input_Detect_RIGHT == 0))//��������ϰ� 
			{
				Robots_Run_Status=0x01;//��ת 
			}
			 
			if((Input_Detect_LEFT == 1)&& (Input_Detect_RIGHT == 1))//���Ҷ���⵽��������Ƶ�е���������һ����Ľ��� 
			{
				Robots_Run_Status=0x04;//ֹͣ 
			}
}
//����ģʽ
void Avoiding(void)
{
   		 switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;
			case 0x05:MOTOR_GO_BACK; break;		   
		 }

		   	if((Input_Detect_LEFT == 1) || (Input_Detect_RIGHT == 1) || (Input_Detect1==0))
			{
		    
				Robots_Run_Status=0x04;
			} 
			else //������ִ��ǰ������
			{
				  Robots_Run_Status=0x03;
			}
}
//���ͳ�����
void Send_wave(void)
{
    uint16 i;

	Trig = 1;
	for(i=0;i<150;i++);
	Trig = 0;
}
//��þ���ֵ
uchar Get_Distance(void)
{
    uint32 Distance = 0;

    Send_wave();
    TH1 = 0;
    TL1 = 0;
	while(TH1<250 && Echo!= 1);	  
	if(TH1 <= 250)	//��෶Χ<0.5M
	{
	   TH1 = 0;
	   TL1 = 0;
	   while(Echo == 1);
	   Distance = TH1;
	   Distance = Distance*256;
	   Distance = Distance + TL1;
	   Distance = Distance * 17; 
	   Distance = Distance / 22118; 
	   return (uchar)(Distance&0xFF);
	}
}
//ͨ���״����
void AvoidByRadar(void)
{
   if(Get_Distance()<0x0A)//����״�ز�����С��10���״���
	{
		 MOTOR_GO_STOP;
	}
	else
	{
	  MOTOR_GO_FORWARD;
	}

}
void Send_Distance(void)
{
	   UART_send_byte(0xFF);
	   UART_send_byte(0x03);
	   UART_send_byte(0x00);
	   UART_send_byte(Get_Distance());
	   UART_send_byte(0xFF);
	   Delay_Ms(1000);   
}
//ģʽִ���Ӻ��������ݱ�־λ�����ж�
void Cruising_Mod(void)
{
       
	 if(Pre_Cruising_Flag != Cruising_Flag)
	 {
	     if(Pre_Cruising_Flag != 0)
		 {
		     MOTOR_GO_STOP; 
		 }

    	 Pre_Cruising_Flag =  Cruising_Flag;
	 }	

	switch(Cruising_Flag)
	{
	   case 0x01:Follow_Track(); break;//����ģʽ
	   case 0x02:FollowLine(); break;//Ѳ��ģʽ
	   case 0x03:Avoiding(); break;//����ģʽ
	   case 0x04:AvoidByRadar();break;//����������ģʽ
	   default:break;
	}	 
}


void Delay_ForBarrier(uint32 t)
{  
    uint16 i;
	while(t--)
	{
	   for(i=0;i<1050;i++);
	}
}







