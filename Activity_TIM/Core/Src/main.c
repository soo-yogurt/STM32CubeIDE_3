/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

// 옥타브 5
#define C 956 // 도
#define D 852
#define E 758
#define F 716
#define G 638 // 솔
#define A 568
#define B 506
#define C_ 902 // C#
#define D_ 804
#define F_ 676
#define A_ 536
#define M 0 // mute

#define VOLUME 300

#define C4 1911
#define C5 956
#define A3 2272
#define AS3 2241
#define A4 1136 // 솔
#define AS4 1272
#define F3 2863
#define F4 1431
#define D3 3405
#define D4 1702
#define DS3 3214
#define DS4 1607
#define CS4 1803
#define GS3 2408
#define G3 2551
#define E3 3034
#define FS4 1351
#define B3 2024
#define GS4 1204
// 375
// 4 >> 250
// 8 >> 125

uint16_t bicycle[] = {E, G, G, M, E, G, G, M, A, A, A, A, A, M}; //, G, G, M, A, A, A, A, A};
uint16_t interval[] = {125, 125, 250, 250, 125, 125, 250, 0, 125, 125, 125, 125, 375, 625};
uint16_t mute[] =    {125, 125, 0, 0, 125, 125, 0, 0, 125, 125, 125, 125, 125, 0};

uint16_t bicycle_2[] = {G, G, G, G, F, F, F, F, E, E, E, E, E, M};
uint16_t interval_2[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 375, 625};
uint16_t mute_2[] = {125, 125, 125 ,125, 125, 125, 125 ,125, 125, 125, 125 ,125, 125, 0};

uint16_t bicycle_3[] = {E, G, G, G, E, G, G, M, A, A, E, E, G, M};
uint16_t interval_3[] = {125, 125, 125, 125, 125, 125, 250, 250, 200, 75, 125, 125, 375, 625};
uint16_t mute_3[] = {125, 125, 125 ,125, 125, 125 ,0, 0, 125, 50, 75, 125, 125, 125, 0};


uint16_t bicycle_4[] = {F, F, F, F, E, E, E, E, D, D, G, G, C, M};
uint16_t interval_4[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 375, 625};
uint16_t mute_4[] = {125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 0};

uint8_t bell_length = sizeof(bicycle)/sizeof(uint16_t);
uint8_t bell_length_2 = sizeof(bicycle_2)/sizeof(uint16_t);
uint8_t bell_length_3 = sizeof(bicycle_3)/sizeof(uint16_t);
uint8_t bell_length_4 = sizeof(bicycle_4)/sizeof(uint16_t);
//uint8_t interval[] = {125, 125, 250, 250, 125, 125, 250, 250 , 125, 125, 125, 125, 375}



uint16_t underworld_melody[] = {
  C4, C5, A3, A4,
  AS3, AS4, M,
  M,
  C4, C5, A3, A4,
  AS3, AS4, M,
  M,
  F3, F4, D3, D4,
  DS3, DS4, M,
  M,
  F3, F4, D3, D4,
  DS3, DS4, M,
  M, DS4, CS4, D4,
  CS4, DS4,
  DS4, GS3,
  G3, CS4,
  C4, FS4, F4, E3, AS4, A4,
  GS4, DS4, B3,
  AS3, A3, GS3,
  M, M, M
};
	//Underwolrd tempo
uint16_t underworld_tempo[] = {
	  12, 12, 12, 12,
	  12, 12, 6,
	  3,
	  12, 12, 12, 12,
	  12, 12, 6,
	  3,
	  12, 12, 12, 12,
	  12, 12, 6,
	  3,
	  12, 12, 12, 12,
	  12, 12, 6,
	  6, 18, 18, 18,
	  6, 6,
	  6, 6,
	  6, 6,
	  18, 18, 18, 18, 18, 18,
	  10, 10, 10,
	  10, 10, 10,
	  3, 3, 3
};

uint8_t underworld_length =  sizeof(underworld_melody) / sizeof(uint16_t);
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
uint8_t data;
uint8_t rx;
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
  MX_USART3_UART_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  uint32_t set_CCR = TIM2 -> ARR / 300;
		  //300;
  //uint8_t value=25;

   HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

/*	 for(int i = 0; i < bell_length; i++){
		  TIM2 -> ARR = bicycle[i];
		  TIM2 -> CCR1 = set_CCR;
		  HAL_Delay(interval[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute[i]);
	  }
	  for(int i = 0; i < bell_length_2; i++){
		  TIM2 -> ARR = bicycle_2[i];
		  TIM2 -> CCR1 = set_CCR;
		  HAL_Delay(interval_2[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_2[i]);
	  }
	  for(int i = 0; i < bell_length_3; i++){
		  TIM2 -> ARR = bicycle_3[i];
		  TIM2 -> CCR1 = set_CCR;
		  HAL_Delay(interval_3[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_3[i]);
	  }
	  for(int i = 0; i < bell_length_4; i++){
		  TIM2 -> ARR = bicycle_4[i];
		  TIM2 -> CCR1 = set_CCR;
		  HAL_Delay(interval_4[i]);
		  TIM2 -> CCR1 = 0;
		  HAL_Delay(mute_4[i]);
	  }*/
	  	 for(int i = 0; i < underworld_length; i++){
	  		  TIM2 -> ARR = underworld_melody[i];
	  		  TIM2 -> CCR1 = TIM2 -> ARR / VOLUME;
	  		  HAL_Delay(1500/underworld_tempo[i]);
	  		  //TIM2 -> CCR1 = 0;
	  		  //HAL_Delay(mute[i]);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/*void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
		HAL_GPIO_TogglePin(GPIOB, LD1_Pin);
	if(htim->Instance == TIM3)
		HAL_GPIO_TogglePin(GPIOB, LD2_Pin);
	if(htim->Instance == TIM4)
		HAL_GPIO_TogglePin(GPIOB, LD3_Pin);
}*/
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
