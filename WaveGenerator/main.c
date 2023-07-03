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
#include <math.h>
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LOOKUP_SIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
const uint16_t sine_lookup[LOOKUP_SIZE] = {
	0x8000,0x8324,0x8647,0x896a,0x8c8b,0x8fab,0x92c7,0x95e1,
	0x98f8,0x9c0b,0x9f19,0xa223,0xa527,0xa826,0xab1f,0xae10,
	0xb0fb,0xb3de,0xb6b9,0xb98c,0xbc56,0xbf17,0xc1cd,0xc47a,
	0xc71c,0xc9b3,0xcc3f,0xcebf,0xd133,0xd39a,0xd5f5,0xd842,
	0xda82,0xdcb3,0xded7,0xe0eb,0xe2f1,0xe4e8,0xe6cf,0xe8a6,
	0xea6d,0xec23,0xedc9,0xef5e,0xf0e2,0xf254,0xf3b5,0xf504,
	0xf641,0xf76b,0xf884,0xf989,0xfa7c,0xfb5c,0xfc29,0xfce3,
	0xfd89,0xfe1d,0xfe9c,0xff09,0xff61,0xffa6,0xffd8,0xfff5,
	0xffff,0xfff5,0xffd8,0xffa6,0xff61,0xff09,0xfe9c,0xfe1d,
	0xfd89,0xfce3,0xfc29,0xfb5c,0xfa7c,0xf989,0xf884,0xf76b,
	0xf641,0xf504,0xf3b5,0xf254,0xf0e2,0xef5e,0xedc9,0xec23,
	0xea6d,0xe8a6,0xe6cf,0xe4e8,0xe2f1,0xe0eb,0xded7,0xdcb3,
	0xda82,0xd842,0xd5f5,0xd39a,0xd133,0xcebf,0xcc3f,0xc9b3,
	0xc71c,0xc47a,0xc1cd,0xbf17,0xbc56,0xb98c,0xb6b9,0xb3de,
	0xb0fb,0xae10,0xab1f,0xa826,0xa527,0xa223,0x9f19,0x9c0b,
	0x98f8,0x95e1,0x92c7,0x8fab,0x8c8b,0x896a,0x8647,0x8324,
	0x8000,0x7cdb,0x79b8,0x7695,0x7374,0x7054,0x6d38,0x6a1e,
	0x6707,0x63f4,0x60e6,0x5ddc,0x5ad8,0x57d9,0x54e0,0x51ef,
	0x4f04,0x4c21,0x4946,0x4673,0x43a9,0x40e8,0x3e32,0x3b85,
	0x38e3,0x364c,0x33c0,0x3140,0x2ecc,0x2c65,0x2a0a,0x27bd,
	0x257d,0x234c,0x2128,0x1f14,0x1d0e,0x1b17,0x1930,0x1759,
	0x1592,0x13dc,0x1236,0x10a1,0xf1d,0xdab,0xc4a,0xafb,
	0x9be,0x894,0x77b,0x676,0x583,0x4a3,0x3d6,0x31c,
	0x276,0x1e2,0x163,0xf6,0x9e,0x59,0x27,0xa,
	0x0,0xa,0x27,0x59,0x9e,0xf6,0x163,0x1e2,
	0x276,0x31c,0x3d6,0x4a3,0x583,0x676,0x77b,0x894,
	0x9be,0xafb,0xc4a,0xdab,0xf1d,0x10a1,0x1236,0x13dc,
	0x1592,0x1759,0x1930,0x1b17,0x1d0e,0x1f14,0x2128,0x234c,
	0x257d,0x27bd,0x2a0a,0x2c65,0x2ecc,0x3140,0x33c0,0x364c,
	0x38e3,0x3b85,0x3e32,0x40e8,0x43a9,0x4673,0x4946,0x4c21,
	0x4f04,0x51ef,0x54e0,0x57d9,0x5ad8,0x5ddc,0x60e6,0x63f4,
	0x6707,0x6a1e,0x6d38,0x7054,0x7374,0x7695,0x79b8,0x7cdb};

const uint16_t abs_sine_lookup[LOOKUP_SIZE] = {
	0x8000,0x8324,0x8647,0x896a,0x8c8b,0x8fab,0x92c7,0x95e1,
	0x98f8,0x9c0b,0x9f19,0xa223,0xa527,0xa826,0xab1f,0xae10,
	0xb0fb,0xb3de,0xb6b9,0xb98c,0xbc56,0xbf17,0xc1cd,0xc47a,
	0xc71c,0xc9b3,0xcc3f,0xcebf,0xd133,0xd39a,0xd5f5,0xd842,
	0xda82,0xdcb3,0xded7,0xe0eb,0xe2f1,0xe4e8,0xe6cf,0xe8a6,
	0xea6d,0xec23,0xedc9,0xef5e,0xf0e2,0xf254,0xf3b5,0xf504,
	0xf641,0xf76b,0xf884,0xf989,0xfa7c,0xfb5c,0xfc29,0xfce3,
	0xfd89,0xfe1d,0xfe9c,0xff09,0xff61,0xffa6,0xffd8,0xfff5,
	0xffff,0xfff5,0xffd8,0xffa6,0xff61,0xff09,0xfe9c,0xfe1d,
	0xfd89,0xfce3,0xfc29,0xfb5c,0xfa7c,0xf989,0xf884,0xf76b,
	0xf641,0xf504,0xf3b5,0xf254,0xf0e2,0xef5e,0xedc9,0xec23,
	0xea6d,0xe8a6,0xe6cf,0xe4e8,0xe2f1,0xe0eb,0xded7,0xdcb3,
	0xda82,0xd842,0xd5f5,0xd39a,0xd133,0xcebf,0xcc3f,0xc9b3,
	0xc71c,0xc47a,0xc1cd,0xbf17,0xbc56,0xb98c,0xb6b9,0xb3de,
	0xb0fb,0xae10,0xab1f,0xa826,0xa527,0xa223,0x9f19,0x9c0b,
	0x98f8,0x95e1,0x92c7,0x8fab,0x8c8b,0x896a,0x8647,0x8324,
	0x8000,0x8324,0x8647,0x896a,0x8c8b,0x8fab,0x92c7,0x95e1,
	0x98f8,0x9c0b,0x9f19,0xa223,0xa527,0xa826,0xab1f,0xae10,
	0xb0fb,0xb3de,0xb6b9,0xb98c,0xbc56,0xbf17,0xc1cd,0xc47a,
	0xc71c,0xc9b3,0xcc3f,0xcebf,0xd133,0xd39a,0xd5f5,0xd842,
	0xda82,0xdcb3,0xded7,0xe0eb,0xe2f1,0xe4e8,0xe6cf,0xe8a6,
	0xea6d,0xec23,0xedc9,0xef5e,0xf0e2,0xf254,0xf3b5,0xf504,
	0xf641,0xf76b,0xf884,0xf989,0xfa7c,0xfb5c,0xfc29,0xfce3,
	0xfd89,0xfe1d,0xfe9c,0xff09,0xff61,0xffa6,0xffd8,0xfff5,
	0xffff,0xfff5,0xffd8,0xffa6,0xff61,0xff09,0xfe9c,0xfe1d,
	0xfd89,0xfce3,0xfc29,0xfb5c,0xfa7c,0xf989,0xf884,0xf76b,
	0xf641,0xf504,0xf3b5,0xf254,0xf0e2,0xef5e,0xedc9,0xec23,
	0xea6d,0xe8a6,0xe6cf,0xe4e8,0xe2f1,0xe0eb,0xded7,0xdcb3,
	0xda82,0xd842,0xd5f5,0xd39a,0xd133,0xcebf,0xcc3f,0xc9b3,
	0xc71c,0xc47a,0xc1cd,0xbf17,0xbc56,0xb98c,0xb6b9,0xb3de,
	0xb0fb,0xae10,0xab1f,0xa826,0xa527,0xa223,0x9f19,0x9c0b,
	0x98f8,0x95e1,0x92c7,0x8fab,0x8c8b,0x896a,0x8647,0x8324,
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
int sine(uint32_t duration, uint32_t frequency);
int square(uint32_t duration, uint32_t frequency);
int triangle(uint32_t duration, uint32_t frequency);
int abs_sine(uint32_t duration, uint32_t frequency);
int step(uint32_t duration, uint32_t frequency);
int sawtooth(uint32_t duration, uint32_t frequency);
void Set_Send_Data(uint32_t data);

/* USER CODE BEGIN PFP */

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
  MX_SPI3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	uint8_t Wave_completed = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		uint8_t packet[6];	
		HAL_SPI_Receive(&hspi3, packet, 6, HAL_MAX_DELAY);
		
		uint16_t signal_type = (((uint16_t)packet[1]) << 8) + packet[0];
		uint16_t duration = (((uint16_t)packet[3]) << 8) + packet[2];
		uint16_t frequency = (((uint16_t)packet[5]) << 8) + packet[4];

		//signal_type = 2;
		
		//uint32_t signal_type = packet[0];
		//uint32_t duration = (uint32_t)(packet[1] * 9500 + 500);
		//uint32_t frequency = (uint32_t) (packet[2] * 999 + 1);
		//uint32_t signal_type = 1;
		//uint32_t duration = 1000;
		//uint32_t frequency = 500;
		
		switch (signal_type){
			case 1:
				Wave_completed = sine(duration, frequency);
				break;
			case 2:
				Wave_completed = square(duration, frequency);
				break;
			case 3:
				Wave_completed = triangle(duration, frequency);
				break;
			case 4:
				Wave_completed = abs_sine(duration, frequency);
				break;
			case 5:
				Wave_completed = step(duration, frequency);
				break;
			case 6:
				Wave_completed = sawtooth(duration, frequency);
				break;
			default:
				Wave_completed = 1;
				break;
			
		}
    /* USER CODE BEGIN 3 */
		HAL_SPI_Transmit(&hspi3, &Wave_completed, 1, HAL_MAX_DELAY);
  }
  /* USER CODE END 3 */
}

int sine(uint32_t duration, uint32_t frequency){
	int bound = LOOKUP_SIZE*duration*frequency / 10000;
	while (bound > 0){
		for(int i=0; i<LOOKUP_SIZE; i++){
			if(bound <= 0)
				return 0;
			Set_Send_Data(sine_lookup[i]/2);
			bound--;
			HAL_Delay(1);
		}
	}
	return 0;
}

int square(uint32_t duration, uint32_t frequency){
	//int16_t Real_Time = duration;	
	//int16_t t = 1000 / frequency;
	//int16_t half_t = t / 2;
	//while(Real_Time > 0){
		//if (Real_Time <= 0)
			//return 0;
		//Set_Send_Data(0);
		//HAL_Delay(half_t);
		//Real_Time -= half_t;
		//if (Real_Time <= 0)
			//return 0;
		//Set_Send_Data(0xFFFF);
		//HAL_Delay(half_t);
		//Real_Time -= half_t;}
	int bound = (256*duration*frequency)/10000;
	while(bound > 0){
		for (int i=0; i<127; i++){
			Set_Send_Data(0xFFFF);
			HAL_Delay(1);
			bound--;
			if(bound <= 0)
				return 0;
		}
		for(int i=127; i>0; i++){
			Set_Send_Data(0);
			HAL_Delay(1);
			bound--;
			if(bound <= 0)
				return 0;
		}
	}
	return 0;
}

int triangle(uint32_t duration, uint32_t frequency){
	int bound = (256*duration*frequency)/10000;
	while(bound > 0){
		for(int i=0; i<127; i++){
			bound--;
			if(bound <=0)
				return 0;
			Set_Send_Data(i*256);
			HAL_Delay(1);
		}
		for(int i=127; i>0; i--){
			bound--;
			if(bound <=0)
				return 0;
			Set_Send_Data(i*256);
			HAL_Delay(1);
		}
	}
	return 0;
}

int abs_sine(uint32_t duration, uint32_t frequency){
	int bound = LOOKUP_SIZE*duration*frequency / 10000;
	while (bound > 0){
		for(int i=0; i<LOOKUP_SIZE; i++){
			if(bound <= 0)
				return 0;
			Set_Send_Data(abs_sine_lookup[i]/2);
			bound--;
			HAL_Delay(1);
		}
	}
	return 0;
}

int step(uint32_t duration, uint32_t frequency){
	for(int i=0; i < 10; i++){
		Set_Send_Data(i*3000);
		HAL_Delay(50);
	}
	
	for(int i=10; i > 0; i--){
		Set_Send_Data(i*3000);
		HAL_Delay(50);
	}
	return 0;
}

int sawtooth(uint32_t duration, uint32_t frequency){
	int bound = (256*duration*frequency)/10000;
	while(bound > 0){
		for(int i=0; i<127; i++){
			bound--;
			if(bound <=0)
				return 0;
			Set_Send_Data(i*256);
			HAL_Delay(1);
		}
	}
	return 0;
}

void Set_Send_Data(uint32_t data){
	uint32_t lsb = data & 0x00FF;
	uint32_t msb = data >> 8;
	GPIOB->ODR = lsb;
	GPIOC->ODR = msb;
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
