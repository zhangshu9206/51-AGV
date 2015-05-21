/*
��Ȩ������
WIFI���������������˴��⹤���Ұ�Ȩ���� www.wifi-robots.com
�����������޸ı����򣬲�Ӧ���������з�������С�������˼��������Ӳ�Ʒ�ϣ����ǽ�ֹ������ҵĲ����
By WIFI���������������˴��⹤����
*/

#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "steer.h"
#include "mem.h"
#include "IR.h"
#include "timer.h"	 

extern uint16 se_timer[8];
uint8 xdata buffer[3];
uint8 rec_flag=0;	//����0�ȴ����� ����1���ڽ��� 
extern uchar IR_Type;  
extern uchar IR_Num;
extern uchar Cruising_Flag; 

void UART_init(void)
{
	PCON |= 0x80;		//ʹ�ܲ����ʱ���λSMOD
	SCON = 0x50;		//8λ����,�ɱ䲨����
	BRT = RELOAD_COUNT;		//�趨���������ʷ�������װֵ
	AUXR |= 0x04;		//���������ʷ�����ʱ��ΪFosc,��1T
	AUXR |= 0x01;		//����1ѡ����������ʷ�����Ϊ�����ʷ�����
	AUXR |= 0x10;		//�������������ʷ�����
	ES      =   1;    //�������ж�
    EA      =   1;    //�����ж�
	TI      =   1;	
	EX0=0;
	EX1=0;
}

void UART_send_byte(uint8 byte)
{
	ES     =   0;  //�ش����ж�
    TI     =   0;  //���㴮�ڷ�������ж������־
    SBUF   =   byte;
    while(TI ==0); //�ȴ��������
    TI     =   1;  //���㴮�ڷ�������ж������־
    ES     =   1;  //�������ж�
}

void UART_send(uint8 * Buffer, uint16 Length)
{
	while(Length != 0)
	{
		UART_send_byte(*Buffer);
		Buffer++;
		Length--;
	}

}

void Communication_Decode(void)  
{	
	if(buffer[0]==0x00)
	{
		switch(buffer[1])
		{
           	case 0x01:
				MOTOR_GO_FORWARD;	
				UART_send("FF000100FF",strlen("FF000100FF")); 
				return; 
			case 0x02:
				MOTOR_GO_BACK;		
				UART_send("FF000200FF",strlen("FF000200FF"));
				return;
			case 0x03:
				MOTOR_GO_LEFT;    
				//UART_send("FF000200FF",strlen("FF000200FF"));
				return;
		    case 0x04:MOTOR_GO_RIGHT;   return;
			case 0x00:MOTOR_GO_STOP;    return;
			default: return;
		}	
	}
	else if(buffer[0]==0x01)							
	{
		if(buffer[2]>180)
			return;
		switch(buffer[1])
		{
			case 0x01:se_timer[0]=buffer[2]; return;
			case 0x02:se_timer[1]=buffer[2]; return;
			case 0x03:se_timer[2]=buffer[2]; return;
			case 0x04:se_timer[3]=buffer[2]; return;
			case 0x05:se_timer[4]=buffer[2]; return;
			case 0x06:se_timer[5]=buffer[2]; return;
			case 0x07:se_timer[6]=buffer[2]; return;
			case 0x08:se_timer[7]=buffer[2]; return;
			default : return;
		}
	}
	else if(buffer[0]==0x50)	  //ң�����������(ѧϰ)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Rec(); 
		}
		return;			  
	}
	else if(buffer[0]==0x51)	  //ң�������ⷢ��(����)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Tra(); 
		}
		return;			  
	}
	else if(buffer[0]==0x13)//ģʽ�л�����
	{
	    switch(buffer[1])
		{
		  case 0x01: Cruising_Flag = 0x01; break;//����
		  case 0x02: Cruising_Flag = 0x02; break;//Ѳ��
		  case 0x03: Cruising_Flag = 0x03; break;//����
		  case 0x04: Cruising_Flag = 0x04; break;//�״����
           case 0x00: 
               {
                  Cruising_Flag = 0x00; 
                  if(buffer[2] == 0x00)
                  {
                      IS_CON = 1;
                      UART_send("IS_CON1", strlen("IS_CON1")); 
                      break; //����ģʽ
                  }
                  else if(buffer[2] == 0x01)
                  {
                      IS_CON = 0;
                      UART_send("IS_CON0", strlen("IS_CON0")); 
                      
                      TMOD &= 0x00;
                      AUXR &= 0X00;
                      IP &= 0x00; //��ʱ��0�ж����ȼ����
                      TR0 = 0;
                      ET0 = 0;
                      initiate_soft_uart();
                      break; //����ģʽ
                  }
                  else
                  ;
                  break; 
               }
             
		  default:Cruising_Flag = 0x00; break;//����ģʽ
		}
	}
	else
	{
		return;
	}
}

void UART_Interrupt_Receive(void) interrupt 4
{
	static uint8 i;

    if(RI==1)
    {
        RI  =   0;
        if(rec_flag==0)
		{
			if(SBUF==0xff)
			{
				rec_flag=1;	
				i=0;
			}			
		}
		else
		{
			if(SBUF==0xff)
			{	
				rec_flag=0;	
				if(i==3)
				{
					Communication_Decode();	
					UART_init(); //	���������������
				}
				i=0;
			}
			else
			{
				buffer[i]=SBUF;
				i++;
			}
		}		
    }
    else
    {
        TI  =  0;
    }
		
}