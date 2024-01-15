/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "LCD.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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
int get_keypad_number(int keypad_num);
int keypad_output(int column, int in1, int in2, int in3, int in4);
int enter_keypad(char str[32], int gradient, int intercept);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_SPI3_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	LCD_Puts(11, 0, "99243009");
	LCD_Puts(11, 1, "99243076");
	HAL_Delay(200);
	LCD_Clear();

	
	uint16_t keypad_num = 20;
	uint8_t recieve = 0;
	
	char str[32];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		LCD_Puts(0, 10, "Choose your wave:");
		HAL_Delay(200);
		LCD_Clear();
		LCD_Puts(2, 0, "1:Sine  2:Square 3:Triangle");
		LCD_Puts(2, 1, "4:AbsSine 5:Step 6:Sawtooth");
		HAL_Delay(200);
		keypad_num = get_keypad_number(keypad_num);
		LCD_Clear();
		sprintf(str, "%d", keypad_num);
		LCD_Puts(0, 5, str);
		HAL_Delay(200);
		LCD_Clear();
		
		
		
		LCD_Puts(0, 0, "Set the Durration:");
		//HAL_Delay(500);
		uint16_t duration = enter_keypad(str, 9500, 500);
		LCD_Clear();
		//sprintf(str, "%d", duration);
		//LCD_Puts(0, 0, str);
		//HAL_Delay(200);
		//LCD_Clear();
		LCD_Puts(0, 0, "Set the Frequency:");
		uint16_t frequency = enter_keypad(str, 999, 1);
		
		LCD_Clear();

		//LCD_Puts(0, 0, str);
		//HAL_Delay(500);
		switch (keypad_num){
			case 1:
				LCD_Puts(10, 0, "Sine");
				break;
			case 2:
				LCD_Puts(10, 0, "Square");
				break;
			case 3:
				LCD_Puts(10, 0, "Triangle");
				break;
			case 4:
				LCD_Puts(10, 0, "AbsSine");
				break;
			case 5:
				LCD_Puts(10, 0, "Step");
				break;
			case 6:
				LCD_Puts(10, 0, "Sawtooth");
				break;
			default:
				LCD_Puts(9, 0, "Invalid Number!");
				break;
		}
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
		uint8_t *packet = (uint8_t*) malloc(3* sizeof(uint8_t));
		//uint16_t packet[3] = {keypad_num, duration, frequency};
		packet[0] = (uint8_t)keypad_num;
		packet[1] = (uint8_t)keypad_num >> 8;
		packet[2] = (uint8_t)duration;
		packet[3] = (uint8_t)duration >> 8;
		packet[4] = (uint8_t)frequency;
		packet[5] = (uint8_t)frequency >> 8;
		HAL_SPI_Transmit(&hspi3, packet, 6, HAL_MAX_DELAY);
		//HAL_SPI_Receive(&hspi3, &recieve, 1, HAL_MAX_DELAY);
		HAL_Delay(500);
		LCD_Clear();
		//sprintf(str, "%d", recieve);
		//LCD_Puts(0,0, str);
		//HAL_Delay(1000);
		//if (recieve == 0)
			//LCD_Puts(0, 0, "Operation successful!");
		//HAL_SPI_Receive();
		//HAL_Delay(200);
		LCD_Clear();
		keypad_num = 20;
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

int enter_keypad(char str[32], int gradient, int intercept){
	float Voltage = 0;
	int keypad_num;
	do {
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		
		keypad_num = keypad_output(3, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));

		uint32_t ADC_Value;
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
		ADC_Value = HAL_ADC_GetValue(&hadc1);
		if (Voltage != (float)(ADC_Value) / (4095)){
			Voltage = (float)(ADC_Value) / (4095);
			float temp = Voltage * gradient + intercept;
			if (temp < 10000 && temp >= 1000)
				sprintf(str, "0%d", (int)temp);
			else if (temp < 1000 && temp >= 100)
				sprintf(str, "00%d", (int)temp);
			else if (temp < 100 && temp >= 10)
				sprintf(str, "000%d", (int)temp);
			else if (temp < 10 && temp >= 1)
				sprintf(str, "0000%d", (int)temp);
			else
				sprintf(str, "%d", (int)temp);

			LCD_Puts(0, 1, str);
		}
			
	}
	while(keypad_num != -1);
	return Voltage * gradient + intercept;
}

int get_keypad_number(int keypad_num)
{
	do {
	
	
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		
		keypad_num = keypad_output(1, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));

		if (keypad_num != 20)
			return keypad_num;
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
	
		
		keypad_num = keypad_output(2, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
		
		if (keypad_num != 20)
			return keypad_num;
		
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		
		keypad_num = keypad_output(3, HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5), HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6));
	}
	while(keypad_num == 20);
	return keypad_num;
}

int keypad_output(int column, int in1, int in2, int in3, int in4){
	if (in1 == 0)
		return column + 0;
	if (in2 == 0)
		return column + 3;
	if (in3 == 0)
		return column + 6;
	if (in4 == 0 && column == 2)
		return 0;
	if (in4 == 0 && column == 3)
		return -1;
	return 20;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

#ifdef  USE_FULL_ASSERT
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
