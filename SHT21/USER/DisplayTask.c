#include "LedDisplay.h"
#include "SHT21.h"
#include "Control.h"
extern unsigned char DisBuf[8];
extern unsigned char Point;

/************************************************
函数名：Display_Task
功能：显示温度和湿度
格式：
60.0 80.5
************************************************/
void Display_Task(void)
{
    unsigned int temp;
    unsigned int humi;

    temp = Temperture_SHT21;   // 300 -> 30.0
    humi = Humidity_SHT21;     // 805 -> 80.5

    /* 温度 */

    DisBuf[7] = temp / 100 % 10;
    DisBuf[6] = temp / 10 % 10;
    DisBuf[5] = temp % 10;

    /* 中间空一位 */

    DisBuf[4] = 23;

    /* 湿度 */

    DisBuf[3] = humi / 100 % 10;
    DisBuf[2] = humi / 10 % 10;
    DisBuf[1] = humi % 10;

    DisBuf[0] = 23;

    /*
       DisBuf[7] DisBuf[6] DisBuf[5] DisBuf[4]
       DisBuf[3] DisBuf[2] DisBuf[1] DisBuf[0]

       3         0         0         空
       8         0         5         空

       显示:
       30.0 80.5
    */

    Point = 0x44;
}

void Display_Tst(void) //湿度显示测试
{
    unsigned int temp;
    unsigned int set;

    temp = Temperture_SHT21;
    set  = Humidity_Set;

    /* 温度 */

    DisBuf[7] = temp / 100 % 10;
    DisBuf[6] = temp / 10 % 10;
    DisBuf[5] = temp % 10;

    /* 中间空位 */

    DisBuf[4] = 23;

    /* 阈值 */

    DisBuf[3] = set / 100 % 10;
    DisBuf[2] = set / 10 % 10;
    DisBuf[1] = set % 10;

    DisBuf[0] = 23;

    Point = 0x44;
}