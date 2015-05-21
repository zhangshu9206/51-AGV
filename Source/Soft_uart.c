
#include "Soft_uart.h"
#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "timer.h"
#include "steer.h"
#include "IR.h"

INT8U xdata Rbuf1[BufLong]; //FIFO接收区
INT8U Rptr1,Rnum1;
INT8U xdata Tbuf1[BufLong];//FIFO发送区
INT8U Tptr1,Tnum1;
signed char TimCnt1A,TimCnt1B; 
INT8U Mtbuf1,Mrbuf1,TxdCnt1,RxdCnt1;
//INT8U RxdCnt2,Rnum2,Rptr2,Mrbuf2,Rbuf2[BufLong]; //如果需要两个9600 RxD，则添加这些变量。

INT16U	ReceiveNextByteToRead,SendNextByteToWrite;	               //接收数据读取指针
bit  Brxd1,Srxd1;		//RxD检测电平
//INT16U  ReceiveNextByteToRead2,SendNextByteToWrite2;			  //如果需要两个9600 RxD，则添加这些变量。
//bit  Brxd2,Srxd2;												  //如果需要两个9600 RxD，则添加这些变量。

static uint8 rs_rec_flag = 0; 
static uint8 xdata rs_buffer[3]; 
static uint8 rs_i; 

void Recv(void)
{
 
  
  if(RxdCnt1>1)      //存数据位8个
 
  {
 
    Mrbuf1>>=1;
 
    if(RxD1==1) Mrbuf1=Mrbuf1|0x80;
 
  }
 
  RxdCnt1--;
 
  if(RxdCnt1==1 /*&& RxD1==1*/) //数据接收完毕，未接收停止位。	 RxdCnt1==0时，接收停止位
 
  { 
	Brxd1=Srxd1=1;	 //重置接收状态等待下一个起始位。
    if(Rnum1<BufLong)		//FIFO队列未满
	{
		Rbuf1[Rptr1]=Mrbuf1; //存储到FIFO队列
		if(++Rptr1>BufLong-1) Rptr1=0;
		if(++Rnum1>BufLong) Rnum1=BufLong;	//FIFO队列已满，不再允许数据添加
	}

  }
 
}

void Send(void)
{
 
 if(TxdCnt1!=0)  //字节发送状态机
 
 {
 
  if(TxdCnt1==11) TxD1=0;//发起始位0
 
  else if(TxdCnt1>2) //发数据位
 
   { Mtbuf1>>=1; TxD1=CY;}
 
  else  TxD1=1;     //发终止位1
 
  TxdCnt1--;
 
 }
 
 else if(Tnum1>0)  //检测FIFO队列
 
 {
 
   Tnum1--;
 
   Mtbuf1=Tbuf1[Tptr1]; //读取FIFO数据
 
   if(++Tptr1>=BufLong) Tptr1=0;
 
   TxdCnt1=11;     //启动发送状态机
 
 }
 
}

#ifdef TIMER_0
void Timer_0(void) interrupt 1 using 3
{
    /*调用红外与舵机*/
    if(IS_CON == 1)
    {
        Timer0_Init();

        if(IR_EN == 1)
        {
            Timer_For_IR();
        }
        else
        {
            Steering_Engine_Control();
        }
    }
    else
    {
//      TMOD &= 0x00;
//      AUXR &= 0X00;
//      IP &= 0x00; //定时器0中断优先级最高
//      TR0 = 0;
//      ET0 = 0;
//      initiate_soft_uart();
    }
    
    /*调用软件串口*/
    if(RxdCnt1 == 0)  //接收起始识别

    {
        if(RxD1 == 0 && Brxd1 == 0 && Srxd1 == 1)     //第一个数据位间隔取4，保证读取脉冲中间
        {RxdCnt1 = 9; TimCnt1B = 0;}
        Srxd1 = Brxd1; Brxd1 = RxD1;
    }


    if(++TimCnt1B >= 3 && RxdCnt1 != 0) //数据接收，间隔为3
    {TimCnt1B = 0; Recv();}

    if(++TimCnt1A >= 3) //数据发送
    {TimCnt1A = 0; Send();}
}
#endif

void soft_rs232_init (void)            //串口初始化  
{ 
  TH0=-BaudT; TL0=-BaudT; TR0=1;
 
  Rptr1=0;Rnum1=0;Tptr1=0;Tnum1=0;
  
  RxdCnt1=TxdCnt1=0;
  
 	AUXR   |=   0x00;                  //Set T0x12 = 0, 12分频
    TMOD &= TMOD_AND_WORD;
    TMOD |= TMOD_TIME_MODE2;           //模式2，溢出触发中断后硬件自动将THx重装入TLx 确保定时器精度
    Brxd1=Srxd1=RxD1 = 1;                        //接收脚置成高电平  
    TxD1 = 1;                        //发射脚置成高电平  
	ReceiveNextByteToRead=SendNextByteToWrite=0;	//接收环形缓冲读取/写入指针初始化
    IP_PTx = 1;                        //置中断优先级为高  
	EA=1;                              //开启总中断
    IE_ETx = 1;                        //允许定时器中断 
	IE|=0x82; 
    TCON_ENABLE_TIMER = 1;             //启动定时器  
    
}

void initiate_soft_uart (void)               //软件串口初始化  
{
    soft_rs232_init();                 //串口初始化  
//  Timer0_Init();
    EA = 1;                            //开中断  
}


//由收转到发时，要先调用 soft_send_enable ()  
void rs_send_byte(INT8U SendByte)      //发送一个字节  
{
	while ( Tnum1 >= BufLong);             //等待FIFO队列有空
	Tbuf1[SendNextByteToWrite]=SendByte; 
	if(++SendNextByteToWrite>BufLong-1) SendNextByteToWrite=0;
	if(++Tnum1>BufLong) Tnum1=BufLong;	//FIFO队列已满，不再允许数据添加
}

void rs_Communication_Decode(void)
{
    if(rs_buffer[0] == '0') 
    {
        switch(rs_buffer[1]) 
        {
            case '1':
               MOTOR_GO_FORWARD;
               //UART_send("FF000100FF", strlen("FF000100FF"));
               return;
            case '2':
               MOTOR_GO_BACK;
               //UART_send("FF000200FF", strlen("FF000200FF"));
               return;
            case '3':
               MOTOR_GO_LEFT;
               //UART_send("FF000200FF",strlen("FF000200FF"));
               return;
            case '4':
               MOTOR_GO_RIGHT; 
               return;
            case '0':
               MOTOR_GO_STOP; 
               return;
            default:
               return;
        }
    }
    else if(rs_buffer[0] == '3')
    {
        IS_CON = 1;
    }
    else 
    {
        return;
    }
}

INT8U rs_receive_byte(void)      //接收一个字节  先调用soft_receive_enable()
{
     
	INT8U outputByte=0;  
    while(Rnum1==0);			//等待接收到一个字符
	outputByte=Rbuf1[ReceiveNextByteToRead];
/*    
    UART_send("outputByte:", strlen("outputByte:"));  
    UART_send(&outputByte, strlen(&outputByte)); 
*/
    //Arduino端信息解析
    if(rs_rec_flag == 0) 
    {
        if(outputByte == 'F') 
        { 
            rs_rec_flag = 1; 
            rs_i = 0; 
        }
    }
    else
    {
        if(outputByte == 'F') 
        {
            rs_rec_flag = 0; 
            if(rs_i == 3) 
            {
                rs_Communication_Decode();
                //UART_init(); //	解决串口死机问题
            }
            rs_i = 0; 
        }
        else
        {
            rs_buffer[rs_i] = outputByte; 
            rs_i++; 
        }
    }
 
    
	if( ReceiveNextByteToRead<BufLong-1)
	{
		ReceiveNextByteToRead++;
	}else{
		ReceiveNextByteToRead=0;
	}
	Rnum1--;					//读取完毕，从FIFO队列中腾出一个字符

	return outputByte;
}

