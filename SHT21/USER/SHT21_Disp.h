#ifndef __LedDisplay_H
#define __LedDisplay_H

#include "config.h"

#define DARK 23   // DARK宏定义，解决未定义报错
extern unsigned char DisBuf[8];
extern unsigned char Point;

void Send_595(unsigned char dat);
void SendHc595TwoByte(unsigned char sel,unsigned char seg);
void LedDriver(void);
void DisU16(unsigned int dat);

#endif