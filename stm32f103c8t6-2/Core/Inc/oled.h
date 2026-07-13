#ifndef __OLED_H
#define __OLED_H

// 引入I2C底层驱动头文件与STM32 HAL库核心头文件
#include "i2c.h"
#include "stm32f1xx_hal.h"

// OLED I2C设备地址定义
#define OLED_DEV_ADDR_GND     0x78    // OLED SA0引脚接GND时的I2C通信地址
#define OLED_DEV_ADDR_VCC     0x7A    // OLED SA0引脚接VCC时的I2C通信地址
#define OLED_CMD_CTRL         0x00    // I2C发送控制字节：0代表后续数据为指令
#define OLED_DATA_CTRL        0x40    // I2C发送控制字节：0x40代表后续数据为显示像素数据

// 字库与位图尺寸宏定义
#define OLED_ZH16X16_COUNT    10U      // 内置16*16点阵汉字总数量
#define OLED_DEMO_BMP_WIDTH   114U    // 内置示例图片的像素宽度
#define OLED_DEMO_BMP_HEIGHT  64U     // 内置示例图片的像素高度

// 外部全局变量，记录最近一次I2C通信返回状态
extern volatile HAL_StatusTypeDef oled_last_i2c_status;

/**
 * @brief 向OLED写入控制指令
 * @param cmd SSD1306驱动芯片寄存器指令码
 */
void OLED_WriteCmd(uint8_t cmd);

/**
 * @brief 向OLED写入显示像素数据
 * @param dat 单字节8bit点阵像素数据
 */
void OLED_WriteData(uint8_t dat);

/**
 * @brief OLED硬件初始化，自动识别I2C地址并下发初始化序列
 * @retval uint8_t 1=初始化成功，0=未检测到OLED设备，初始化失败
 */
uint8_t OLED_Init(void);

/**
 * @brief 全屏清屏，将所有像素点置为熄灭状态
 */
void OLED_Clear(void);

/**
 * @brief 屏幕测试函数，在第0页绘制一条全亮横线
 */
void OLED_TestLine(void);

/**
 * @brief 通过字库索引显示16*16点阵汉字
 * @param x 汉字左上角横向起始坐标(0~127)
 * @param page 汉字纵向起始页号，单个汉字占用连续2个page
 * @param index 汉字在oled_zh16x16字库数组中的下标
 */
void OLED_ShowZh16x16ByIndex(uint8_t x, uint8_t page, uint8_t index);

/**
 * @brief 通用位图显示函数，支持自定义尺寸图片绘制
 * @param x 图片左上角横向起始坐标
 * @param page 图片左上角起始页号
 * @param width 图片像素宽度
 * @param height 图片像素高度
 * @param bmp 点阵图片数据数组指针
 */
void OLED_ShowBMP(uint8_t x, uint8_t page, uint8_t width, uint8_t height, const uint8_t *bmp);

/**
 * @brief 快速显示内置示例位图
 * @param x 图片横向起始坐标
 * @param page 图片起始页号
 */
void OLED_ShowDemoBMP(uint8_t x, uint8_t page);


void OLED_ShowNum8x16(uint8_t x, uint8_t page, int16_t num, uint8_t dot_en);
void OLED_ShowSingleNum8x16(uint8_t x, uint8_t page, uint8_t ch);


#endif
