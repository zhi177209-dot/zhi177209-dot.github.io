/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "bsp_dht11.h"
#include "command.h"
#include <stdio.h>
#include "err.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t readBuffer[10];
DHT11_Data_TypeDef DHT11_Data;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
	if(huart == &huart1){
		Command_Write(readBuffer,Size);
		HAL_UARTEx_ReceiveToIdle_IT(&huart1,readBuffer,sizeof(readBuffer));
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
	HAL_Delay(2000);//Œ»∂®…œµÁ
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
	OLED_Init();
	OLED_Clear();
  /* USER CODE BEGIN 2 */
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,readBuffer,sizeof(readBuffer));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		uint8_t command[50];
		uint8_t commandLength = 0;
		CommandStatus_t status;
		status = Command_GetCommand(command, &commandLength);

		switch (status)
		{
		case CMD_NONE:
				break;

		case CMD_OK:
				if (commandLength == 0)
				{
						Send_Error(ERR_INVALID_LENGTH);
						Log_Print("[ERR] Empty command\r\n");
						break;
				}

				switch (command[0])
				{
				case 0x01:
						Log_Print("[CMD] Read DHT11\r\n");

						HAL_Delay(2000);

						if (DHT11_ReadData(&DHT11_Data))
						{
								char message[50];
								uint16_t len = (uint16_t)snprintf(message, sizeof(message),
										"Humidity:%d.%d%% Temperature:%d.%dC\r\n",
										DHT11_Data.humi_int, DHT11_Data.humi_deci,
										DHT11_Data.temp_int, DHT11_Data.temp_deci);

								HAL_UART_Transmit(&huart1, (uint8_t *)message, len, 1000);

								OLED_Clear();
								OLED_ShowZh16x16ByIndex(0, 2, 0);
								OLED_ShowZh16x16ByIndex(16, 2, 1);
								OLED_ShowZh16x16ByIndex(32, 2, 2);
								OLED_ShowNum8x16(48, 2, (int16_t)(DHT11_Data.humi_int * 10 + DHT11_Data.humi_deci), 1);

								OLED_ShowZh16x16ByIndex(0, 6, 3);
								OLED_ShowZh16x16ByIndex(16, 6, 4);
								OLED_ShowZh16x16ByIndex(32, 6, 5);
								OLED_ShowNum8x16(48, 6, (int16_t)(DHT11_Data.temp_int * 10 + DHT11_Data.temp_deci), 1);
						}
						else
						{
								Send_Error(ERR_SENSOR_FAIL);
								Log_Print("[ERR] DHT11 read failed\r\n");
						}
						break;

				case 0x02:
						Log_Print("[CMD] OLED clear\r\n");
						OLED_Clear();
						break;

				case 0x03:
				{
						Log_Print("[CMD] Return last DHT11 data\r\n");

						char message[50];
						uint16_t len = (uint16_t)snprintf(message, sizeof(message),
								"Humidity:%d.%d%% Temperature:%d.%dC\r\n",
								DHT11_Data.humi_int, DHT11_Data.humi_deci,
								DHT11_Data.temp_int, DHT11_Data.temp_deci);

						HAL_UART_Transmit(&huart1, (uint8_t *)message, len, 1000);
						break;
				}

				default:
						Send_Error(ERR_UNKNOWN_CMD);
						Log_Print("[ERR] Unknown command\r\n");
						break;
				}
				break;

		case CMD_ERR_CHECKSUM:
				Send_Error(ERR_CHECKSUM);
				Log_Print("[ERR] Checksum error\r\n");
				break;

		case CMD_ERR_LENGTH:
				Send_Error(ERR_INVALID_LENGTH);
				Log_Print("[ERR] Length error\r\n");
				break;

		case CMD_ERR_TIMEOUT:
				Send_Error(ERR_TIMEOUT);
				Log_Print("[ERR] Frame timeout\r\n");
				break;

		case CMD_ERR_HEAD:
				// Parser already dropped one byte; avoid log spam for line noise.
				break;

		default:
				Send_Error(0xFF);
				Log_Print("[ERR] Unknown parser status\r\n");
				break;
		}
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
