#include  "uart.h"
bit uart1_tx_busy=0;

void uart1_init (void)
{
	//1、串口配置 1)串口初始化
	AUXR|=S1ST2;                  //(1)采用T2作为波特率发生器
	SCON=0x50;                      //  (2) 串口工作在方式1，使能接收
    //1、串口配置 2)定时器2初始化
	AUXR&=~T2CT;                //(1) T2 定时模式
    AUXR|=T2X12;                  //(2) T2 1T模式
	T2H= TIMER2_ROLOAD >>8;// (3)T2高8位初值
	T2L =TIMER2_ROLOAD;      //      T2低8位初值
	AUXR|=T2R;                     //  (4)启动T2
	//2、开中断
	ES=1;//使能串口1中断
	EA=1;//开总中断
	//3、设置中断优先级
	PS=1;//设为高优先级	
}

void uart1_isr(void)  interrupt  UART1_VECTOR
{
	if(RI)//收到一个字节的数据?
	{
		RI=0;//接收中断标置清零，需手动清
        SBUF=SBUF; //接收的数据发回去
	}
	
	if(TI)//发送完一个字节的数据?
	{
		TI=0;//发送中断标置清零，需手动清
		uart1_tx_busy=0;
	}
}


void uart1_send_byte(uint8_t  dat)
{
	
	while(uart1_tx_busy); //等待发送完成
    SBUF=dat;   //将数据写入SBUF，开始发送数据
	uart1_tx_busy =1;       //发送完成标志清零
}

char putchar(char c)
{
	 uart1_send_byte(c);
	return c;
}
