#include "Control.h"
#include "Key.h"
#include "SHT21.h"
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;

/* 简单按键扫描（10ms调用一次） 
通过按键来调节湿度阀值
*/
void Key_Task(void)
{
    static unsigned char k1_old = 1;
    static unsigned char k2_old = 1;

    /* KEY1：加 */
    if(KEY1 == 0 && k1_old == 1)
    {
        Humidity_Set += 100;   // +1.0%
    }
    k1_old = KEY1;

    /* KEY2：减 */
    if(KEY2 == 0 && k2_old == 1)
    {
        if(Humidity_Set >= 100)
            Humidity_Set -= 100;
    }
    k2_old = KEY2;
}