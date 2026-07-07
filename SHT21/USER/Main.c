#include	"config.h"
#include	"timer.h"
#include	"GPIO.h"

#include "SHT21.h"
#include "LedDisplay"
#include "Control.h"
#include "oled.h"
void Task(void);
void Task_1ms(void);
void Task_10ms(void);
void Task_100ms(void);
void Task_1000ms(void);

void Task_1ms(void)
{
//***********call every 1ms**************
P6^=0x01;  //500Hz
LedDriver();
	
//****************************************	
}
void Task_10ms(void)
{
//**********call every 10ms***************
P6^=0x02;  //50Hz


	
//****************************************
}
void Task_100ms(void)
{
//***********call every 100ms**************
P6^=0x04;  //5Hz
	
	SHT21_Task();//获取温度和湿度
	Display_Task();//数码管显示温湿度
	//Display_Tst();//湿度显示测试
	Motor_Task();//根据阀值以及湿度大小启动电机
	Key_Task();//按键调节阀值
	Buzzer_Task();//温度超过预设值时蜂鸣器报警
	/*
	数码管闪屏严重将下面的代码挪到1000ms去
	*/
//	show_temp(Temperture_SHT21,0);//oled显示温度
//	show_temp(Humidity_SHT21,2);//oled显示湿度
//	show_temp(Humidity_Set,4);//oled显示阀值
	
//****************************************
}
void Task_1000ms(void)
{

//********call every 1000ms*****************
P6^=0x08;    //0.5Hz
	show_temp(Temperture_SHT21,0);//oled显示温度
	show_temp(Humidity_SHT21,2);//oled显示湿度
	show_temp(Humidity_Set,4);//oled显示阀值
//**************************************** 

}
void main()
{
//*******初始化******************	
	Timer_config();
	GPIO_config();
	
	OLED_Init();
	OLED_Display();
	
	EA=1;

//********主循环*****************	
	while(1)
	{
		Task();	
	}

}

void Task(void)
{
	static u8 cnt1,cnt2,cnt3;
	if(1==FLAG_1ms)
	{
		FLAG_1ms=0;
		Task_1ms();
		cnt1++;
		if(cnt1>=10)
		{
			cnt1=0;
			Task_10ms();
			cnt2++;
			if(cnt2>=10)
			{
				cnt2=0;
				Task_100ms();
				cnt3++;
				if(cnt3>=10)
				{
					cnt3=0;
					Task_1000ms();
				}
			}
		}
	}
}
