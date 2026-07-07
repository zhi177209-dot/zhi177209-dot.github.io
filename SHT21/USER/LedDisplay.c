#include	"LedDisplay.H"
#define DARK 23



unsigned char Flash=0;//(1<<2)+(1<<5);
unsigned char Point=0;

unsigned char DisBuf[8]={0,1,2,3,4,5,6,7}; //定义显示缓冲区
unsigned char code leddata[]={ 
 
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"20
                0x5C,  //"o"
                0x40,  //"-"22
                0x00,  //熄灭
                0x00  //自定义
 
                         };

/**************** 向HC595发送一个字节函数 ******************/
void Send_595(unsigned char dat)
{       
    unsigned char  i;
    for(i=0; i<8; i++)
    {
       
		dat <<= 1;
        HC595_DAT   = CY;		
        HC595_SCLK = 1;
        HC595_SCLK = 0;
    }
}
//
void SendHc595TwoByte(unsigned char sel,unsigned char seg)
{

	Send_595(sel);
	Send_595(seg);
	HC595_LCLK=1;
	HC595_LCLK=0;
}
void LedDriver(void)
{
	static unsigned char  DisPtr=0; //定义静态局部变量
	static unsigned int   cnt=0;
	static unsigned char  FlashF=0;
	//数码管显示
	cnt++;
	if(cnt>=500)
	{
		cnt=0;
		FlashF=~FlashF;
	}
	DisPtr++;
	if(DisPtr>=8)
	{
		DisPtr=0;
	}            //实现DisPtr0~7计数
	if(FlashF==0xff)
	{
		SendHc595TwoByte(~(1<<DisPtr),leddata[DisBuf[DisPtr]]+((Point<<(7-DisPtr))&0x80));
	}else
		{
			if(((1<<DisPtr)&Flash)!=0)
			{
				SendHc595TwoByte(~(1<<DisPtr),0x00);
			}else
				{
					SendHc595TwoByte(~(1<<DisPtr),leddata[DisBuf[DisPtr]]+((Point<<(7-DisPtr))&0x80));
				}
			
		}
	
	
	//点亮第DisPtr位显示DisBuf中第DisPtr个元素
}




//显示无符号16位二进制数带消隐功能
void DisU16(unsigned int dat)
{
    unsigned char i=0,j=0;
	DisBuf[0]=dat%10;        //个位
	DisBuf[1]=dat/10%10;     //十位
	DisBuf[2]=dat/100%10;    //百位
	DisBuf[3]=dat/1000%10;   //千位
	DisBuf[4]=dat/10000%10;  //万位
	//以下程序用于实现消隐，即把高位的0去掉
	
	//从最高位开始查找第一个非0
	for(i=4;i>0;i--)
	{
		if(DisBuf[i]!=0)
		break;
	}
	//最第一个非数前面的零消一隐
	for(j=i+1;j<=4;j++)
	{
		DisBuf[j]=23;
	}
	//unsigned int 数据类型最多就5位，5-7位固定不显示
	DisBuf[5]=23;
	DisBuf[6]=23;
	DisBuf[7]=23;
}

