#ifndef      __UART_H__
#define      __UART_H__
#include    "config.h"
#include    <stdio.h>

//头文件包含
//宏定义
#define   BAUD                   115200UL
#define   TIMER2_ROLOAD  (65536-MAIN_Fosc/BAUD/4)



//内部函数声明
//外部函数声明
void uart1_init (void);
void uart1_send_byte(uint8_t  dat);


//外部变量声明

#endif
