#ifndef	__SHT21_H__
#define	__SHT21_H__

#include    "config.h"




#define SHT21_WRITE         0x80
#define SHT21_READ          0x81
#define CONVERT_T_NO_HOLD   0xf3
#define CONVERT_RH_NO_HOLD  0xf5

extern uint16_t Humidity_SHT21; //存放湿度的结果 分辨力0.1%
extern uint16_t Temperture_SHT21;//存放温度的结果 分辨力0.1度
/*************  IO口定义    **************/
void SHT21_Task(void); //在以1毫秒的周期调用此函数，如果数码管出现闪烁，可将数码管驱动放入定时器中断中
#endif