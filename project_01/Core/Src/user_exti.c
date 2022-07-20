/* USER CODE END Header */
#define __PRINT__


/* Includes ------------------------------------------------------------------*/
#include "user_exti.h"
#include "stdio.h"
/* USER CODE BEGIN PV */
ClickInfoDef click[3];

int current_time = 0;
int time_interval = 0;
int last_time = 0;

uint8_t longClick = 0;
uint8_t flag = 0;
/* USER CODE END PV */

/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	GPIO_PinState pin;

	if (GPIO_Pin == GPIO_PIN_13) {
		current_time = HAL_GetTick();
		time_interval = current_time - last_time;
		last_time = current_time;

		pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

		if (time_interval <= 4) {
#ifndef __PRINT__
			printf("Noise %d, %d\r\n", pin, time_interval);
#endif
		} else {

			click[1].time = click[0].time;
			click[1].level = click[0].level;

			click[0].time = time_interval;
			click[0].level = pin;
		}
         // long click
		if (click[0].level == GPIO_PIN_RESET && click[0].time >= LONG_CLICK_MIN)
		{
#ifndef __PRINT__
			printf("\r\nLong Key\r\n");
#endif
			longClick = 1;

		} else if (click[0].level == GPIO_PIN_RESET
				&& click[1].level == GPIO_PIN_SET) {
#ifndef __PRINT__
			printf("\r\nSelect Key, %d\r\n", click[0].time);
#endif
			flag++;
		}
	}
}
// HAL_GetTick(); 함수를 이용해서 상승/하강 에지 발생 시간을 측정하였다.
// 측정한 시간으로 버튼의 동작을 Nomal Click과 longClick으로 나누었다.
// double Click은 제거
// 측정된 시간을 이용해서 Noise를 제거, 오작동을 줄인다.
/* USER CODE END 0 */


