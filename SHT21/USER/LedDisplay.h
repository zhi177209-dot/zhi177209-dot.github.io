#ifndef	__LEDDISPLAY_H
#define	__LEDDISPLAY_H
#include	"config.h"
/*************  IO口定义    **************/
sbit    HC595_DAT   = P2^3;   //    SER     data input
sbit    HC595_LCLK  = P2^0;   //    LCLk    store (latch) clock
sbit    HC595_SCLK  = P2^1;   //    SCLK   Shift data clock

extern unsigned char Flash;
extern unsigned char Point;
extern unsigned char DisBuf[8];//显示缓冲区

void LedDriver(void);  //需每隔1毫秒调用

void DisU16(unsigned int dat);
#endif