/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "../Inc/display.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
int map(int x, int in_min, int in_max, int out_min, int out_max);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int pwm=0;
int adc=0;
int state = 0;

int La = 3635;
int Do = 6109;
int Re = 5455;
int Mi = 4848;

int multiplier = 1;

int gamme= 0;

uint16_t fq = 0;
int psc = 0;
int psc2 = 0;
int adc1,adc2;
uint8_t data[5];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void turnOnLED()
{
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, 1);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 1);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 1);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1);
	HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, 1);
	HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, 1);
}

void turnOffLED()
{
	HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, 0);
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, 0);
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, 0);
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0);
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0);
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0);
	HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, 0);
	HAL_GPIO_WritePin(LED7_GPIO_Port, LED7_Pin, 0);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if (htim->Instance == TIM4)
	{
//		printf("TIMER4\n");
		HAL_TIM_Base_Start_IT(&htim5);
		turnOnLED();
//		printf("TIMER4\n");
	}

	if (htim->Instance == TIM5)
	{
//		printf("TIMER5\n");
		turnOffLED();
//		__HAL_TIM_SET_PRESCALER(&htim3,665);
//		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
//		printf("TIMER5\n");
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
	MX_GPIO_Init();
	MX_ADC_Init();
	MX_TIM3_Init();
	MX_USART2_UART_Init();
	MX_SPI1_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	/* USER CODE BEGIN 2 */
	MAX7219_Config();
	HAL_TIM_Base_Start_IT(&htim4);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{

		while (HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin) == 0)
		{
			SPI_write(0x0C, 0x01);//Shutdown à 1 (Mode activé)

			HAL_ADC_Start(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc2 = HAL_ADC_GetValue(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc1 = HAL_ADC_GetValue(&hadc);

			HAL_Delay(50);

			HAL_ADC_Stop(&hadc);

			gamme = map(adc1,0,4096,3,13);
			psc2 = map(adc2,0,4096,500,250);

			switch (gamme)
			{

			case 12:
				La = 3635/64;
				break;

			case 11:
				La = 3635/32;
				break;

			case 10:
				La = 3635/16;
				break;

			case 9:
				La = 3635/8;
				break;

			case 8:
				La = 3635/4;
				multiplier = 4;
				break;

			case 7:
				La = 3635/2;
				multiplier = 2;
				break;

			case 6:
				La = 3635/1;
				multiplier = 1;
				break;

			case 5:
				La = 3635*2;
				multiplier = 2;
				break;

			case 4:
				La = 3635*4;
				multiplier = 4;
				break;

			case 3:
				La = 3635*8;
				multiplier = 8;
				break;

			case 2:
				La = 3635*16;
				multiplier = 16;
				break;

			case 1:
				La = 3635*32;
				multiplier = 32;
				break;

			case 0:
				La = 3635*64;
				multiplier = 64;
				break;

			default:

				La = 3635;

			}

			fq = (16000000/(La+1))/10;


			Segment_Display(fq);

			printf("PSC Value : %d\tFrequence : %d\n",gamme,fq);

			__HAL_TIM_SET_PRESCALER(&htim3,La);
			__HAL_TIM_SET_AUTORELOAD(&htim4,psc2);

			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

			HAL_Delay(50);
		}

		while (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 0)
		{
			SPI_write(0x0C, 0x01);//Shutdown à 1 (Mode activé)

			HAL_ADC_Start(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc2 = HAL_ADC_GetValue(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc1 = HAL_ADC_GetValue(&hadc);

			HAL_Delay(50);

			HAL_ADC_Stop(&hadc);

			gamme = map(adc1,0,4096,3,13);
//			psc = map(adc1,0,4096,200,8000);
			psc2 = map(adc2,0,4096,500,250);

			switch (gamme)
			{

			case 12:
				Do = 6109/64;
				break;

			case 11:
				Do = 6109/32;
				break;

			case 10:
				Do = 6109/16;
				break;

			case 9:
				Do = 6109/8;
				break;

			case 8:
				Do = 6109/4;
				multiplier = 4;
				break;

			case 7:
				Do = 6109/2;
				multiplier = 2;
				break;

			case 6:
				Do = 6109/1;
				multiplier = 1;
				break;

			case 5:
				Do = 6109*2;
				multiplier = 2;
				break;

			case 4:
				Do = 6109*4;
				multiplier = 4;
				break;

			case 3:
				Do = 6109*8;
				multiplier = 8;
				break;

			case 2:
				Do = 6109*16;
				multiplier = 16;
				break;

			case 1:
				Do = 6109*32;
				multiplier = 32;
				break;

			case 0:
				Do = 6109*64;
				multiplier = 64;
				break;

			default:

				Do = 6109;

			}

			fq = (16000000/(Do+1))/10;

//			fq = (16000000/(psc+1))/10;

			Segment_Display(fq);

//			printf("PSC Value : %d\tFrequence : %d\n",psc,fq);
			__HAL_TIM_SET_PRESCALER(&htim3,Do);
			__HAL_TIM_SET_AUTORELOAD(&htim4,psc2);

			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

			HAL_Delay(50);
		}

		while (HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == 0)
		{
			SPI_write(0x0C, 0x01);//Shutdown à 1 (Mode activé)

			HAL_ADC_Start(&hadc);

			HAL_ADC_PollForConversion(&hadc, 10);
			adc2 = HAL_ADC_GetValue(&hadc);

			HAL_ADC_PollForConversion(&hadc, 10);
			adc1 = HAL_ADC_GetValue(&hadc);

			HAL_Delay(50);

			HAL_ADC_Stop(&hadc);

			gamme = map(adc1,0,4096,3,13);
			psc2 = map(adc2,0,4096,500,250);

			switch (gamme)
			{

			case 12:
				Re = 5455/64;
				break;

			case 11:
				Re = 5455/32;
				break;

			case 10:
				Re = 5455/16;
				break;

			case 9:
				Re = 5455/8;
				break;

			case 8:
				Re = 5455/4;
				multiplier = 4;
				break;

			case 7:
				Re = 5455/2;
				multiplier = 2;
				break;

			case 6:
				Re = 5455/1;
				multiplier = 1;
				break;

			case 5:
				Re = 5455*2;
				multiplier = 2;
				break;

			case 4:
				Re = 5455*4;
				multiplier = 4;
				break;

			case 3:
				Re = 5455*8;
				multiplier = 8;
				break;

			case 2:
				Re = 5455*16;
				multiplier = 16;
				break;

			case 1:
				Re = 5455*32;
				multiplier = 32;
				break;

			case 0:
				Re = 5455*64;
				multiplier = 64;
				break;

			default:

				Re = 5455;

			}

			fq = (16000000/(Re+1))/10;

			Segment_Display(fq);

//			printf("PSC Value : %d\tFrequence : %d\n",psc,fq);
			__HAL_TIM_SET_PRESCALER(&htim3,Re);
			__HAL_TIM_SET_AUTORELOAD(&htim4,psc2);

			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

			HAL_Delay(50);
		}

		while (HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == 0)
		{
			SPI_write(0x0C, 0x01);//Shutdown à 1 (Mode activé)

			HAL_ADC_Start(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc2 = HAL_ADC_GetValue(&hadc);

			HAL_ADC_PollForConversion(&hadc, 50);
			adc1 = HAL_ADC_GetValue(&hadc);

			HAL_Delay(50);

			HAL_ADC_Stop(&hadc);

			gamme = map(adc1,0,4096,3,13);
			psc2 = map(adc2,0,4096,500,250);

			switch (gamme)
			{

			case 12:
				Mi = 4848/64;
				break;

			case 11:
				Mi = 4848/32;
				break;

			case 10:
				Mi = 4848/16;
				break;

			case 9:
				Mi = 4848/8;
				break;

			case 8:
				Mi = 4848/4;
				multiplier = 4;
				break;

			case 7:
				Mi = 4848/2;
				multiplier = 2;
				break;

			case 6:
				Mi = 4848/1;
				multiplier = 1;
				break;

			case 5:
				Mi = 4848*2;
				multiplier = 2;
				break;

			case 4:
				Mi = 4848*4;
				multiplier = 4;
				break;

			case 3:
				Mi = 4848*8;
				multiplier = 8;
				break;

			case 2:
				Mi = 4848*16;
				multiplier = 16;
				break;

			case 1:
				Mi = 4848*32;
				multiplier = 32;
				break;

			case 0:
				Mi = 4848*64;
				multiplier = 64;
				break;

			default:

				Mi = 4848;

			}

			fq = (16000000/(Mi+1))/10;

			Segment_Display(fq);

//			printf("PSC Value : %d\tFrequence : %d\n",psc,fq);
			__HAL_TIM_SET_PRESCALER(&htim3,Mi);
			__HAL_TIM_SET_AUTORELOAD(&htim4,psc2);

			HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

			HAL_Delay(50);
		}

		if ((HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin) == 1) | (HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_Pin) == 1) | (HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_Pin) == 1) | (HAL_GPIO_ReadPin(BTN4_GPIO_Port, BTN4_Pin) == 1))
		{
			SPI_write(0x0C, 0x00);//Shutdown à 1 (Mode activé)
			HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);

			HAL_ADC_Start(&hadc);

			HAL_ADC_PollForConversion(&hadc, 10);
			adc2 = HAL_ADC_GetValue(&hadc);

			HAL_ADC_PollForConversion(&hadc, 10);
			adc1 = HAL_ADC_GetValue(&hadc);

			HAL_Delay(50);

			HAL_ADC_Stop(&hadc);

//			printf("adc1 : %d \tadc2 : %d\n",adc1,adc2);

			psc2 = map(adc2,0,4096,500,250);

//			printf("psc2 : %d\n",psc2);

//			__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,psc2/2);
			__HAL_TIM_SET_AUTORELOAD(&htim4,psc2);

		}

		/* USER CODE END WHILE */

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

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
int map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
