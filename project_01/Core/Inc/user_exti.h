/* USER CODE END Header */
#ifndef __USER_EXTI__
#define __USER_EXTI__
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	int time;
	uint8_t level;
} ClickInfoDef;
/* USER CODE END PTD */

/* USER CODE BEGIN Private defines */
#define LONG_CLICK_MIN 700
#define LONG_CLICK_MAX 5000
#define DOUBLE_CLICK_MIN 40
#define DOUBLE_CLICK_MAX 120
/* USER CODE END Private defines */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* __USER_EXTI__ */
