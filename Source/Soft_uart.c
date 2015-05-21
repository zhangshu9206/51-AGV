
#include "Soft_uart.h"
#include <stdio.h>
#include <string.h>
#include "motor.h"
#include "timer.h"
#include "steer.h"
#include "IR.h"

INT8U xdata Rbuf1[BufLong]; //FIFO������
INT8U Rptr1,Rnum1;
INT8U xdata Tbuf1[BufLong];//FIFO������
INT8U Tptr1,Tnum1;
signed char TimCnt1A,TimCnt1B; 
INT8U Mtbuf1,Mrbuf1,TxdCnt1,RxdCnt1;
//INT8U RxdCnt2,Rnum2,Rptr2,Mrbuf2,Rbuf2[BufLong]; //�����Ҫ����9600 RxD���������Щ������

INT16U	ReceiveNextByteToRead,SendNextByteToWrite;	               //�������ݶ�ȡָ��
bit  Brxd1,Srxd1;		//RxD����ƽ
//INT16U  ReceiveNextByteToRead2,SendNextByteToWrite2;			  //�����Ҫ����9600 RxD���������Щ������
//bit  Brxd2,Srxd2;												  //�����Ҫ����9600 RxD���������Щ������

static uint8 rs_rec_flag = 0; 
static uint8 xdata rs_buffer[3]; 
static uint8 rs_i; 

void Recv(void)
{
 
  
  if(RxdCnt1>1)      //������λ8��
 
  {
 
    Mrbuf1>>=1;
 
    if(RxD1==1) Mrbuf1=Mrbuf1|0x80;
 
  }
 
  RxdCnt1--;
 
  if(RxdCnt1==1 /*&& RxD1==1*/) //���ݽ�����ϣ�δ����ֹͣλ��	 RxdCnt1==0ʱ������ֹͣλ
 
  { 
	Brxd1=Srxd1=1;	 //���ý���״̬�ȴ���һ����ʼλ��
    if(Rnum1<BufLong)		//FIFO����δ��
	{
		Rbuf1[Rptr1]=Mrbuf1; //�洢��FIFO����
		if(++Rptr1>BufLong-1) Rptr1=0;
		if(++Rnum1>BufLong) Rnum1=BufLong;	//FIFO�������������������������
	}

  }
 
}

void Send(void)
{
 
 if(TxdCnt1!=0)  //�ֽڷ���״̬��
 
 {
 
  if(TxdCnt1==11) TxD1=0;//����ʼλ0
 
  else if(TxdCnt1>2) //������λ
 
   { Mtbuf1>>=1; TxD1=CY;}
 
  else  TxD1=1;     //����ֹλ1
 
  TxdCnt1--;
 
 }
 
 else if(Tnum1>0)  //���FIFO����
 
 {
 
   Tnum1--;
 
   Mtbuf1=Tbuf1[Tptr1]; //��ȡFIFO����
 
   if(++Tptr1>=BufLong) Tptr1=0;
 
   TxdCnt1=11;     //��������״̬��
 
 }
 
}

#ifdef TIMER_0
void Timer_0(void) interrupt 1 using 3
{
    /*���ú�������*/
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
//      IP &= 0x00; //��ʱ��0�ж����ȼ����
//      TR0 = 0;
//      ET0 = 0;
//      initiate_soft_uart();
    }
    
    /*�����������*/
    if(RxdCnt1 == 0)  //������ʼʶ��

    {
        if(RxD1 == 0 && Brxd1 == 0 && Srxd1 == 1)     //��һ������λ���ȡ4����֤��ȡ�����м�
        {RxdCnt1 = 9; TimCnt1B = 0;}
        Srxd1 = Brxd1; Brxd1 = RxD1;
    }


    if(++TimCnt1B >= 3 && RxdCnt1 != 0) //���ݽ��գ����Ϊ3
    {TimCnt1B = 0; Recv();}

    if(++TimCnt1A >= 3) //���ݷ���
    {TimCnt1A = 0; Send();}
}
#endif

void soft_rs232_init (void)            //���ڳ�ʼ��  
{ 
  TH0=-BaudT; TL0=-BaudT; TR0=1;
 
  Rptr1=0;Rnum1=0;Tptr1=0;Tnum1=0;
  
  RxdCnt1=TxdCnt1=0;
  
 	AUXR   |=   0x00;                  //Set T0x12 = 0, 12��Ƶ
    TMOD &= TMOD_AND_WORD;
    TMOD |= TMOD_TIME_MODE2;           //ģʽ2����������жϺ�Ӳ���Զ���THx��װ��TLx ȷ����ʱ������
    Brxd1=Srxd1=RxD1 = 1;                        //���ս��óɸߵ�ƽ  
    TxD1 = 1;                        //������óɸߵ�ƽ  
	ReceiveNextByteToRead=SendNextByteToWrite=0;	//���ջ��λ����ȡ/д��ָ���ʼ��
    IP_PTx = 1;                        //���ж����ȼ�Ϊ��  
	EA=1;                              //�������ж�
    IE_ETx = 1;                        //����ʱ���ж� 
	IE|=0x82; 
    TCON_ENABLE_TIMER = 1;             //������ʱ��  
    
}

void initiate_soft_uart (void)               //������ڳ�ʼ��  
{
    soft_rs232_init();                 //���ڳ�ʼ��  
//  Timer0_Init();
    EA = 1;                            //���ж�  
}


//����ת����ʱ��Ҫ�ȵ��� soft_send_enable ()  
void rs_send_byte(INT8U SendByte)      //����һ���ֽ�  
{
	while ( Tnum1 >= BufLong);             //�ȴ�FIFO�����п�
	Tbuf1[SendNextByteToWrite]=SendByte; 
	if(++SendNextByteToWrite>BufLong-1) SendNextByteToWrite=0;
	if(++Tnum1>BufLong) Tnum1=BufLong;	//FIFO�������������������������
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

INT8U rs_receive_byte(void)      //����һ���ֽ�  �ȵ���soft_receive_enable()
{
     
	INT8U outputByte=0;  
    while(Rnum1==0);			//�ȴ����յ�һ���ַ�
	outputByte=Rbuf1[ReceiveNextByteToRead];
/*    
    UART_send("outputByte:", strlen("outputByte:"));  
    UART_send(&outputByte, strlen(&outputByte)); 
*/
    //Arduino����Ϣ����
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
                //UART_init(); //	���������������
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
	Rnum1--;					//��ȡ��ϣ���FIFO�������ڳ�һ���ַ�

	return outputByte;
}

