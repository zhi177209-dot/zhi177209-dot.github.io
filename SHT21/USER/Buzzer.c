/*
温度超过预设值时蜂鸣器报警
*/
#include "SHT21.h"

sbit BEEP = P3^7;

#define TEMP_ALARM 350

void Buzzer_Task(void)
{
    if(Temperture_SHT21 > TEMP_ALARM)
    {
        BEEP = 0;
    }
    else
    {
        BEEP = 1;
    }
}