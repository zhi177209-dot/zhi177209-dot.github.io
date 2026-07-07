
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.GXWMCU.com -----------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/


/*************	功能说明	**************

本文件为STC15W4K系列的PWM程序.

******************************************/

#include	"PWM.h"



//========================================================================
// 函数: void	PWM_SourceClk_Duty(u8 PWM_SourceClk, u16 init_duty)
// 描述: 设置PWM时钟源和周期函数。
// 参数: PWM_SourceClk: PWM时钟源, 0~15对应 主时钟/(PWM_SourceClk + 1), 16为选择定时器2的溢出做时钟.
//                      PwmClk_1T,PwmClk_2T, ... PwmClk_16T, PwmClk_T2.
//       init_duty: PWM周期长度, 1~32767.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注: 
//========================================================================
void	PWM_SourceClk_Duty(u8 PWM_SourceClk, u16 init_duty)
{
	u8	xdata	*px;

	EAXSFR();		// 访问XFR
	px  = PWMCKS;	// PWM时钟选择
	*px = PWM_SourceClk;
	px  = PWMCH;	// PWM计数器的高字节
	*px = (u8)(init_duty >> 8);
	px++;
	*px = (u8)init_duty;	// PWM计数器的低字节
	EAXRAM();	//恢复访问XRAM
}


//========================================================================
// 函数: void	PWMx_Configuration(u8 PWM_id, PWMx_InitDefine *PWMx)
// 描述: PWM配置函数。
// 参数: PWM_id: PWM通道, PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID.
//       *PWMx:  配置结构指针.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注: 
//========================================================================
void	PWMx_Configuration(u8 PWM_id, PWMx_InitDefine *PWMx)
{
	u8	xdata	*px;

	EAXSFR();		//访问XFR
	px = PWM2T1H + (PWM_id << 4);
	*px = (u8)(PWMx->FirstEdge >> 8);	//第一个翻转计数高字节
	px++;
	*px = (u8)PWMx->FirstEdge;			//第一个翻转计数低字节
	px++;
	*px = (u8)(PWMx->SecondEdge >> 8);	//第二个翻转计数高字节
	px++;
	*px = (u8)PWMx->SecondEdge;			//第二个翻转计数低字节
	px++;
	*px = (PWMx->PWMx_IO_Select & 0x08)					// PWM输出IO选择
	    | ((PWMx->PWMx_Interrupt << 2) & 0x04)			// 中断允许
		| ((PWMx->SecondEdge_Interrupt << 1) & 0x02)	// 第二个翻转中断允许
		| (PWMx->FirstEdge_Interrupt & 0x01);			// 第一个翻转中断允许
	PWMCR |= (1 << PWM_id);	// 相应PWM通道的端口为PWM输出口，受PWM波形发生器控制
	PWMCFG = (PWMCFG & ~(1 << PWM_id)) | ((PWMx->Start_IO_Level & 1) << PWM_id);	//设置PWM输出端口的初始电平
	EAXRAM();		//恢复访问XRAM
}

//========================================================================
// 函数: void	PWMx_SetPwmWide(u8 PWM_id, u16 FirstEdge, u16 SecondEdge)
// 描述: PWM设置脉冲宽度函数。
// 参数: PWM_id: PWM通道, PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID.
//       FirstEdge:   第一个翻转计数.
//       SecondEdge:  第一个翻转计数.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注: FirstEdge若等于SecondEdge则占宽比为50%方波，频率为PWM频率的一半
//  若计数值为0~255,则占空比只能在1/256~255/256之间调节
//========================================================================

void	PWMx_SetPwmWide(u8 PWM_id, u16 FirstEdge, u16 SecondEdge)
{
	u8	xdata	*px;

	EAXSFR();		//访问XFR
	px = PWM2T1H + (PWM_id << 4);
	*px = (u8)(FirstEdge >> 8);	//第一个翻转计数高字节
	px++;
	*px = (u8)FirstEdge;			//第一个翻转计数低字节
	px++;
	*px = (u8)(SecondEdge >> 8);	//第二个翻转计数高字节
	px++;
	*px = (u8)SecondEdge;			//第二个翻转计数低字节
	EAXRAM();		//恢复访问XRAM
}


/********************* PWM中断函数************************/
void PWM_int (void) interrupt PWM_VECTOR
{
	if(PWMIF & CBIF)	//PWM计数器归零中断标志
	{
		PWMIF &= ~CBIF;	//清除中断标志
	}
	if(PWMIF & C2IF)	//PWM2中断标志
	{
		PWMIF &= ~C2IF;	//清除中断标志
	}
	if(PWMIF & C3IF)	//PWM3中断标志
	{
		PWMIF &= ~C3IF;	//清除中断标志
	}
	if(PWMIF & C4IF)	//PWM4中断标志
	{
		PWMIF &= ~C4IF;	//清除中断标志
	}
	if(PWMIF & C5IF)	//PWM5中断标志
	{
		PWMIF &= ~C5IF;	//清除中断标志
	}
	if(PWMIF & C6IF)	//PWM6中断标志
	{
		PWMIF &= ~C6IF;	//清除中断标志
	}
	if(PWMIF & C7IF)	//PWM7中断标志
	{
		PWMIF &= ~C7IF;	//清除中断标志
	}
}

/********************* PWM失效中断函数************************/
void PWMFD_int (void) interrupt PWMFD_VECTOR
{
	if(PWMFDCR & FDIF) 		//PWM异常检测中断标志位
	{
		PWMFDCR &= ~FDIF;	//清除中断标志
	}
}

//========================================================================
// 函数: void	PWM_config(void)
// 描述: PWM配置函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注: 
//========================================================================
void	PWM_config(void)
{
	PWMx_InitDefine		PWMx_InitStructure;					//结构定义
//PWM2设置 要开启只需将最后两行的注释取消	
	PWMx_InitStructure.PWMx_IO_Select       = PWM2_P27;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	//PWMx_Configuration(PWM2_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	//P2n_standard(1<<7);										//IO初始化, 上电时为高阻

	
//PWM2设置  P4.5接RGB的蓝色LED	
	PWMx_InitStructure.PWMx_IO_Select       = PWM3_P45;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 0;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 1;			//第二个翻转计数, 1~32767
	PWMx_Configuration(PWM3_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	P4n_standard(1<<5);										//IO初始化, 上电时为高阻

	
	
//PWM2设置 要开启只需将最后两行的注释取消	
	PWMx_InitStructure.PWMx_IO_Select       = PWM4_P44;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	//PWMx_Configuration(PWM4_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	//P4n_standard(1<<4);										//IO初始化, 上电时为高阻
//PWM2设置 要开启只需将最后两行的注释取消	
	PWMx_InitStructure.PWMx_IO_Select       = PWM5_P42;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	//PWMx_Configuration(PWM5_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	//P4n_standard(1<<2);										//IO初始化, 上电时为高阻
//PWM2设置 要开启只需将最后两行的注释取消	
	PWMx_InitStructure.PWMx_IO_Select       = PWM6_P07;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	//PWMx_Configuration(PWM6_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	//P0n_standard(1<<7);										//IO初始化, 上电时为高阻
//PWM2设置 要开启只需将最后两行的注释取消	
	PWMx_InitStructure.PWMx_IO_Select       = PWM7_P06;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = ENABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = ENABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	//PWMx_Configuration(PWM7_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	//P0n_standard(1<<6);										//IO初始化, 上电时为高阻

	
	PWM_SourceClk_Duty(PwmClk_16T, 255);	//时钟源: PwmClk_1T,PwmClk_2T, ... PwmClk_16T, PwmClk_T2,   PWM周期: 1~32767
	//PWM时钟等于系统时钟除于16   计数值0~255 因此PWM的频率为： 11.0592M/16  /256=2.7KHz
	
	PWMCR |= ENPWM;		// 使能PWM波形发生器，PWM计数器开始计数
	PWMCR &= ~ECBI;		//禁止PWM计数器归零中断
//	PWMCR |=  ECBI;		//允许PWM计数器归零中断
  
//	PWMFDCR = ENFD | FLTFLIO | FDIO;	//PWM失效中断控制,  ENFD | FLTFLIO | EFDI | FDCMP | FDIO

    PWMx_SetPwmWide(PWM3_ID, 0, 50);
}
