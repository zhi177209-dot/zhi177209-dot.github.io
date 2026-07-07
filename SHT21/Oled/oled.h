#ifndef __OLED_H
#define __OLED_H

#include	"config.h"	  	 

#define  u8 unsigned char 
#define  u16 unsigned int
#define  u32 unsigned int
	
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

sbit OLED_SCL=P5^1;//SCL
sbit OLED_SDA=P5^0;//SDA
sbit OLED_RES =P1^2;//RES

//-----------------OLED端口定义----------------

#define OLED_SCL_Clr() OLED_SCL=0
#define OLED_SCL_Set() OLED_SCL=1

#define OLED_SDA_Clr() OLED_SDA=0
#define OLED_SDA_Set() OLED_SDA=1

#define OLED_RES_Clr() OLED_RES=0
#define OLED_RES_Set() OLED_RES=1



//OLED控制用函数
void delay_ms(unsigned int ms);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey);
void OLED_ShowChinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[]);
void OLED_Init(void);
void OLED_Display(void);
void show_temp(uint16_t volt,uint8_t a);
#endif  
	 



