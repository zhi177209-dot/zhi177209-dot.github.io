/*/////////////////////////////////////////////////////////////////////////////////	 
//  文 件 名   : IIC.c
//  版 本 号   : v1.0
//  作    者   : psufncel
//  生成日期   : 2026-1-7
//  最近修改   : 
//  功能描述   : 
//              说明: SHT21驱动程序
                使用方法：以1毫秒周期调用SHT21_Task函数，该函数每隔约1秒测量下
				温度和湿度并保存在Temperture_SHT21和Humidity_SHT21中，结果乘上0.1
				即为湿度值和温度值
				
//              ----------------------------------------------------------------
//              ----------------------------------------------------------------
修改历史   :
日    期   : 
作    者   : 
修改内容   : 
//All rights reserved
******************************************************************************/
#include  "SHT21.h"
#include  "IIC.h"

uint16_t Temperture_SHT21=0;
uint16_t Humidity_SHT21=0;
/********************************************
函数名：SHT21_Task
功能:   每隔1秒测下温度和湿度值并存在
        Temperture_SHT21和Humidity_SHT21中。
输入参数1：无
输入参数1：无
输出参数： 无
返回值：   无
*********************************************/
void SHT21_Task(void)
{
	static uint8_t time=3;
	uint16_t xdata DATAT;
	uint8_t xdata Datbuf[4];
	float  h;
	static uint8_t xdata state=0;
	static uint16_t xdata TimeOut=0;
	if(++time>=10)
	{
		time=0;
		 
		switch(state)
		{
			case 0: //启动测量湿度，非保持方式
					IIC_Start();  
					IIC_WriteByte(SHT21_WRITE);
					IIC_ReadACK();
					IIC_WriteByte(CONVERT_RH_NO_HOLD); //非保持测湿度
					IIC_ReadACK();	
					IIC_Stop(); 
					state=1;
					TimeOut=0;
					break;
			case 1: //查询是否测量完成，并读取转换结果，保存在Humidity_SHT21中
					IIC_Start();
					IIC_WriteByte(SHT21_READ);
					if(IIC_ReadACK())
					{
					
						Datbuf[0]=IIC_ReadByte();
						IIC_ACK(); 
						Datbuf[1]=IIC_ReadByte();
						IIC_ACK(); 
						Datbuf[2]=IIC_ReadByte();
						IIC_NACK(); 
						IIC_Stop();   
						DATAT=Datbuf[0]*256+Datbuf[1];
						DATAT&=0xfffc;
						h=-60.0+1250.0*(float)DATAT/65536.0;//分辨力0.1%
						Humidity_SHT21=(uint16_t)h;;					
						state=2;					
					}
					//超时检则
					TimeOut++;
					if(TimeOut>=200)
					{
						TimeOut=0;
						state=2;
						
					}
					break;
			case 2:
					//启动测量湿度，非保持方式
					IIC_Start();  
					IIC_WriteByte(SHT21_WRITE);
					IIC_ReadACK();
					IIC_WriteByte(CONVERT_T_NO_HOLD); //非保持测温度
					IIC_ReadACK();	
					IIC_Stop(); 
					state=3;
					TimeOut=0;
				break;
			case 3:
					//查询是否测量完成，并读取转换结果，保存在Humidity_SHT21中
					IIC_Start();
					IIC_WriteByte(SHT21_READ);
					if(IIC_ReadACK())
					{
					
						Datbuf[0]=IIC_ReadByte();
						IIC_ACK(); 
						Datbuf[1]=IIC_ReadByte();
						IIC_ACK(); 
						Datbuf[2]=IIC_ReadByte();
						IIC_NACK(); 
						IIC_Stop();   
						DATAT=Datbuf[0]*256+Datbuf[1];
						DATAT&=0xfffc;
						h=-468.5+1757.2*(float)DATAT/65536;//0.1C
						Temperture_SHT21=(unsigned int )h;;
						
						state=4;
					
					}
					//超时检则
					TimeOut++;
					if(TimeOut>=200)
					{
						TimeOut=0;
						state=4;
					}
					break;
			case 4: TimeOut++;
					if(TimeOut>=100)
					{
						TimeOut=0;
						state=0;
					}
					break;
			default:state=0;
					break;
		
		}		
	}
	
}