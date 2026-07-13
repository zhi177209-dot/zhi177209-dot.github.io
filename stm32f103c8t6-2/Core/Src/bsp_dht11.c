/**
  ******************************************************************************
  * @file    bsp_dht11.c
  * @brief   DHT11 单总线温湿度传感器驱动，硬件引脚PA5
  ******************************************************************************
  */

#include "tim.h"
#include "bsp_dht11.h"

/**
 * @brief  微秒级延时函数，基于TIM1定时器计数实现精准us延时
 * @param  us：需要延时的微秒数值
 * @retval 无
 */
void bsp_delay_us(uint16_t us)
{
	// 定时器计数器清零
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	// 开启定时器计数
	HAL_TIM_Base_Start(&htim1);

	// 循环等待计数器计到目标us值
	while (__HAL_TIM_GET_COUNTER(&htim1) < us)
	{
	}

	// 延时完成，关闭定时器
	HAL_TIM_Base_Stop(&htim1);
}

/**
 * @brief  等待DHT11数据线变为指定电平，带超时防卡死
 * @param  state：目标电平 GPIO_PIN_SET/GPIO_PIN_RESET
 * @param  timeout_us：最大等待超时时间(微秒)
 * @retval 1：成功等到目标电平；0：超时未等到
 */
static uint8_t DHT11_WaitForPinState(GPIO_PinState state, uint16_t timeout_us)
{
	while (timeout_us--)
	{
		// 读取引脚电平匹配目标则直接返回成功
		if (HAL_GPIO_ReadPin(BSP_DHT11_PORT, BSP_DHT11_PIN) == state)
		{
			return 1;
		}
		// 每次循环延时1us，降低CPU占用
		bsp_delay_us(1);
	}
	// 循环结束超时，返回失败
	return 0;
}

/**
 * @brief  将DHT11引脚配置为推挽输出模式，主机发送信号时使用
 * @retval 无
 */
static void DHT11_Mode_OUT_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = BSP_DHT11_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   // 推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 高速IO，适配时序

	HAL_GPIO_Init(BSP_DHT11_PORT, &GPIO_InitStruct);
}

/**
 * @brief  将DHT11引脚配置为浮空输入模式，读取传感器返回数据时使用
 * @retval 无
 */
static void DHT11_Mode_IN_NP(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = BSP_DHT11_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;       // 输入模式
	GPIO_InitStruct.Pull = GPIO_NOPULL;           // 无上下拉，浮空输入

	HAL_GPIO_Init(BSP_DHT11_PORT, &GPIO_InitStruct);
}

/**
 * @brief  从DHT11读取1字节数据(8bit)
 * @retval 成功返回读到的1字节数据；读取时序异常返回0
 */
uint8_t DHT11_ReadByte(void)
{
	uint8_t i;
	uint8_t temp = 0; // 存储单字节数据

	// 循环读取8位bit，组成1字节
	for (i = 0; i < 8; i++)
	{
		// 等待高电平起始位，超时直接返回0表示读取失败
		if (!DHT11_WaitForPinState(GPIO_PIN_SET, 100))
		{
			return 0;
		}

		// 延时40us区分0/1电平
		bsp_delay_us(40);

		// 高电平持续超过40us代表当前bit为1
		if (HAL_GPIO_ReadPin(BSP_DHT11_PORT, BSP_DHT11_PIN) == GPIO_PIN_SET)
		{
			// 将当前bit置1，高位先存
			temp |= (uint8_t)(0x01U << (7U - i));

			// 等待该bit高电平结束，拉低
			if (!DHT11_WaitForPinState(GPIO_PIN_RESET, 100))
			{
				return 0;
			}
		}
		// 若40us后引脚已拉低，代表bit为0，无需操作temp
	}

	return temp;
}

/**
 * @brief  DHT11启动一次完整数据读取，包含起始信号、应答、4字节数据+校验和
 * @param  DHT11_Data：温湿度数据结构体指针，存放读取结果
 * @retval 1：校验和正确，数据有效；0：时序超时/校验错误，数据无效
 */
uint8_t DHT11_ReadData(DHT11_Data_TypeDef *DHT11_Data)
{
	uint8_t sum; // 本地计算校验和
	uint32_t primask;

	// 1. 主机拉低数据线发送起始信号
	DHT11_Mode_OUT_PP();
	HAL_GPIO_WritePin(BSP_DHT11_PORT, BSP_DHT11_PIN, GPIO_PIN_RESET);
	HAL_Delay(18); // 保持低电平18ms，唤醒DHT11

	// 2. 关键读时序阶段禁止中断，避免被TIM2等中断打断
	primask = __get_PRIMASK();
	__disable_irq();

	// 3. 释放总线，切换为输入等待DHT11应答
	HAL_GPIO_WritePin(BSP_DHT11_PORT, BSP_DHT11_PIN, GPIO_PIN_SET);
	DHT11_Mode_IN_NP();
	bsp_delay_us(30);

	// 等待DHT11拉低总线应答
	if (!DHT11_WaitForPinState(GPIO_PIN_RESET, 100))
	{
		if (!primask)
		{
			__enable_irq();
		}
		return 0;
	}
	// 等待DHT11拉高应答脉冲
	if (!DHT11_WaitForPinState(GPIO_PIN_SET, 100))
	{
		if (!primask)
		{
			__enable_irq();
		}
		return 0;
	}
	// 应答脉冲结束，总线拉低，准备传输数据
	if (!DHT11_WaitForPinState(GPIO_PIN_RESET, 100))
	{
		if (!primask)
		{
			__enable_irq();
		}
		return 0;
	}

	// 依次读取5字节数据：湿度整数、湿度小数、温度整数、温度小数、校验和
	DHT11_Data->humi_int = DHT11_ReadByte();
	DHT11_Data->humi_deci = DHT11_ReadByte();
	DHT11_Data->temp_int = DHT11_ReadByte();
	DHT11_Data->temp_deci = DHT11_ReadByte();
	DHT11_Data->check_sum = DHT11_ReadByte();

	// 读取完成，切回输出拉高总线，释放单总线
	DHT11_Mode_OUT_PP();
	HAL_GPIO_WritePin(BSP_DHT11_PORT, BSP_DHT11_PIN, GPIO_PIN_SET);

	if (!primask)
	{
		__enable_irq();
	}

	// 计算本地校验和：前四字节相加
	sum = (uint8_t)(DHT11_Data->humi_int + DHT11_Data->humi_deci +
		DHT11_Data->temp_int + DHT11_Data->temp_deci);

	// 对比传感器返回校验和，相等则数据合法
	return (DHT11_Data->check_sum == sum) ? 1U : 0U;
}

