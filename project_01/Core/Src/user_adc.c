#include "user_adc.h"

int ADC_value;

// TIM3 인터럽트 발생시 ADC가 동작한다.
// 사용할 LCD 판에 부착되어있는 버튼이 ADC로 동작하고 있다.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
		HAL_ADC_Start(&hadc1);
		ADC_value = HAL_ADC_GetValue(&hadc1);
	}
}
