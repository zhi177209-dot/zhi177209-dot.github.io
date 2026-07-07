/*
根据湿度与湿度阀值对比决定电机的启动
*/
#include "Motor.h"
#include "Control.h"
#include "SHT21.h"

sbit M1 = P0^0;
sbit M2 = P0^1;

void Motor_Stop(void)
{
    M1 = 0;
    M2 = 0;
}

void Motor_Forward(void)
{
    M1 = 1;
    M2 = 0;
}

void Motor_Reverse(void)
{
    M1 = 0;
    M2 = 1;
}

void Motor_Task(void)
{
    if(Humidity_SHT21 > Humidity_Set)
    {
        Motor_Forward();
    }
    else
    {
        Motor_Stop();
    }
}