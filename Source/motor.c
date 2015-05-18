/*
版权声明：
WIFI机器人网·机器人创意工作室版权所有 www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
By WIFI机器人网·机器人创意工作室
*/
#include "motor.h"
#include "uart.h"
#include "timer.h"
#include "mem.h"
uchar Cruising_Flag =0x00;
uchar  Robots_Run_Status;
uchar Pre_Cruising_Flag = 0x00;

/**电机初始化**/
void Motor_Init(void)
{
	MOTOR_A_EN=1;//A端电机使能
	MOTOR_B_EN=1;//B端电机使能
	MOTOR_GO_STOP;
}
//跟随模式
void Follow_Track(void)
{
  		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		
	     if(Input_Detect1 == 1)	//中间传感器OK
		  {
		  
		   	if((Input_Detect0 == 0)&& (Input_Detect2 == 0)) //两边同时探测到障碍物
			{
			
				 Robots_Run_Status=0x04;//停止 
			} 
			 
			if((Input_Detect0 == 0)&& (Input_Detect2 == 1))//左侧障碍物
			{
			
				Robots_Run_Status=0x01;//右转 
			}
		
			if((Input_Detect0 == 1)&& (Input_Detect2 == 0))//右侧障碍物
			{
				Robots_Run_Status=0x02;//左转 
			}
			 
			if((Input_Detect0 == 1)&& (Input_Detect2 == 1))//无任何障碍物
			{
				Robots_Run_Status=0x03;//直行 
			}
		  }
		  else
		  {
		  	   Robots_Run_Status=0x04;//距离近停止
		  }

}
//巡线模式
void FollowLine(void)
{
   		switch(Robots_Run_Status)
		 {
			case 0x01:MOTOR_GO_RIGHT;     break;    
			case 0x02:MOTOR_GO_LEFT;     break;	   
			case 0x03:MOTOR_GO_FORWARD;  break;	   
			case 0x04:MOTOR_GO_STOP;  break;	
		 }

		   	if((Input_Detect_LEFT == 0)&& (Input_Detect_RIGHT == 0))//两边同时探测到障碍物
			{
			
				 Robots_Run_Status=0x03;//直行 
			} 
			 
			if((Input_Detect_LEFT == 0)&& (Input_Detect_RIGHT == 1))//右侧遇到障碍  
			{
			
				Robots_Run_Status=0x02;//左转 
			}
		
			if((Input_Detect_LEFT == 1)&& (Input_Detect_RIGHT == 0))//左侧遇到障碍 
			{
				Robots_Run_Status=0x01;//右转 
			}
			 
			if((Input_Detect_LEFT == 1)&& (Input_Detect_RIGHT == 1))//左右都检测到，就如视频中的那样遇到一道横的胶带 
			{
				Robots_Run_Status=0x04;//停止 
			}
}
//避障模式
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
			else //否则电机执行前进动作
			{
				  Robots_Run_Status=0x03;
			}
}
//发送超声波
void Send_wave(void)
{
    uint16 i;

	Trig = 1;
	for(i=0;i<150;i++);
	Trig = 0;
}
//获得距离值
uchar Get_Distance(void)
{
    uint32 Distance = 0;

    Send_wave();
    TH1 = 0;
    TL1 = 0;
	while(TH1<250 && Echo!= 1);	  
	if(TH1 <= 250)	//测距范围<0.5M
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
//通过雷达避障
void AvoidByRadar(void)
{
   if(Get_Distance()<0x0A)//如果雷达回波数据小于10厘米触发
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
//模式执行子函数，根据标志位进行判断
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
	   case 0x01:Follow_Track(); break;//跟随模式
	   case 0x02:FollowLine(); break;//巡线模式
	   case 0x03:Avoiding(); break;//避障模式
	   case 0x04:AvoidByRadar();break;//超声波壁障模式
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







