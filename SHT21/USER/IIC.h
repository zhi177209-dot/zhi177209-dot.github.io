/*/////////////////////////////////////////////////////////////////////////////////	 
//  文 件 名   : IIC.c
//  版 本 号   : v1.0
//  作    者   : psufncel
//  生成日期   : 2022-3-4
//  最近修改   : 
//  功能描述   : 
//              说明: STC15单片机I2C 程序，时钟频率11.0592MHz
				
//              ----------------------------------------------------------------
//              ----------------------------------------------------------------
修改历史   :
日    期   : 
作    者   : 
修改内容   : 
//All rights reserved
******************************************************************************/
#ifndef __IIC_H__
#define __IIC_H__
#include    "config.h"
sbit SDA=P0^7;
sbit SCL=P0^6;


#define I2C_READ  0x01
#define I2C_WRITE 0x00
void ADS115_Write(unsigned char addr,unsigned int dat);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_ACK(void);
void IIC_NACK(void);
bit IIC_ReadACK(void);
void IIC_WriteByte(uint8_t dat);
uint8_t IIC_ReadByte(void);
#endif