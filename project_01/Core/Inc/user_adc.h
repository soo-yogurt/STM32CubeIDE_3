/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_ADC_
#define __USER_ADC_
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */
// ADC 버튼의 동작 범위를 선언
#define UP_KEY_MIN 0
#define UP_KEY_MAX 15
#define DOWN_KEY_MIN 830
#define DOWN_KEY_MAX 870
#define LEFT_KEY_MIN 1910
#define LEFT_KEY_MAX 1960
#define RIGHT_KEY_MIN 2920
#define RIGHT_KEY_MAX 3010
/* USER CODE END Private defines */


/* Exported functions prototypes ---------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#endif /* __USER_ADC_ */
