/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/

#include "uart.h"
#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "steer.h"
#include "mem.h"
#include "IR.h"
#include "timer.h"	 
#include "Soft_uart.h"

extern uint16 se_timer[8];
uint8 xdata buffer[3];
uint8 rec_flag=0;	//等于0等待接受 等于1正在接受 
extern uchar IR_Type;  
extern uchar IR_Num;
extern uchar Cruising_Flag; 

bit IS_CON;

void UART_init(void)
{
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	BRT = RELOAD_COUNT;		//设定独立波特率发生器重装值
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	ES      =   1;    //允许串口中断
    EA      =   1;    //开总中断
	TI      =   1;	
	EX0=0;
	EX1=0;
}

void UART_send_byte(uint8 byte)
{
	ES     =   0;  //关串口中断
    TI     =   0;  //清零串口发送完成中断请求标志
    SBUF   =   byte;
    while(TI ==0); //等待发送完成
    TI     =   1;  //清零串口发送完成中断请求标志
    ES     =   1;  //允许串口中断
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
				UART_send("FF000300FF",strlen("FF000300FF"));   
				return;
		    case 0x04:
				MOTOR_GO_RIGHT;   
				UART_send("FF000400FF",strlen("FF000400FF"));
				return;
			case 0x00:
				MOTOR_GO_STOP;   
				UART_send("FF000000FF",strlen("FF000000FF")); 
				return;
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
    else if(buffer[0] == 0x40)      //机械臂动作组控制(向软串口发送指令)
    {
        INT8U Byte[5] = ""; 
        switch(buffer[1])
        {
           case 0x01:
//             sprintf(Tbuf1, "F410F");
//  		   rs_send_byte('f');
               //UART_send(Tbuf1, strlen(Tbuf1)); 
               
               rs_UART_send("F410F");
               UART_send("ss_F410F", strlen("ss_F410F")); 
               return;
           case 0x02:
               rs_UART_send("F420F"); 
               UART_send("ss_F420F", strlen("ss_F410F")); 
               return;
           case 0x00:
               rs_UART_send("F400F"); 
               UART_send("ss_F400F", strlen("ss_F410F")); 
               return;
           default:
               return;
        } 
        
    }
    else if(buffer[0] == 0x50)      //遥控器红外接收(学习
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Rec(); 
		}
		return;			  
	}
	else if(buffer[0]==0x51)	  //遥控器红外发射(控制)
	{
		if((buffer[1]<5)&&(buffer[2]<10))
		{
			IR_Type = buffer[1];
			IR_Num = buffer[2];
			IR_Tra(); 
		}
		return;			  
	}
	else if(buffer[0]==0x13)//模式切换开关
	{
	    switch(buffer[1])
		{
            case 0x01: Cruising_Flag = 0x01; break;//跟随
            case 0x02: Cruising_Flag = 0x02; break;//巡线
            case 0x03: Cruising_Flag = 0x03; break;//避障
            case 0x04: Cruising_Flag = 0x04; break;//雷达避障
            case 0x00: 
            {
                
                if(buffer[2] == 0x00)             //云台控制
                {
                  IS_CON = 1;
                  UART_send("IS_CON1", strlen("IS_CON1")); 
                  break; //正常模式
                }
                else if(buffer[2] == 0x01)        //软串口初始 gokit控制
                {
                  IS_CON = 0;
                  UART_send("IS_CON0", strlen("IS_CON0")); 
                  
                  TMOD &= 0x00;
                  AUXR &= 0X00;
                  IP &= 0x00; //定时器0中断优先级最高
                  TR0 = 0;
                  ET0 = 0;
                  initiate_soft_uart();
                  break; //正常模式
                }
                else
                ;
                
                Cruising_Flag = 0x00; 
                
                break; 
            }
            default:Cruising_Flag = 0x00; break;//正常模式
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
					UART_init(); //	解决串口死机问题
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