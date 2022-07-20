#include "user_pwm.h"
#include "stm32f429xx.h"
#include "user_exti.h"

extern uint8_t longClick;

// bicycle[] => 곡 1 음높이
uint16_t bicycle[] = { E, G, G, M, E, G, G, M, A, A, A, A, A, M };
// interval[] + mute[] => 음의 길이와 무음 부분을 표현
uint16_t interval[] = { 125, 125, 250, 250, 125, 125, 250, 0, 125, 125, 125,
		125, 375, 625 };
uint16_t mute[] = { 125, 125, 0, 0, 125, 125, 0, 0, 125, 125, 125, 125, 125, 0 };

uint16_t bicycle_2[] = { G, G, G, G, F, F, F, F, E, E, E, E, E, M };
uint16_t interval_2[] = { 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
		125, 375, 625 };
uint16_t mute_2[] = { 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
		125, 125, 0 };

uint16_t bicycle_3[] = { E, G, G, G, E, G, G, M, A, A, E, E, G, M };
uint16_t interval_3[] = { 125, 125, 125, 125, 125, 125, 250, 250, 200, 75, 125,
		125, 375, 625 };
uint16_t mute_3[] = { 125, 125, 125, 125, 125, 125, 0, 0, 125, 50, 75, 125, 125,
		125, 0 };

uint16_t bicycle_4[] = { F, F, F, F, E, E, E, E, D, D, G, G, C, M };
uint16_t interval_4[] = { 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
		125, 375, 625 };
uint16_t mute_4[] = { 125, 125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
		125, 125, 0 };

uint8_t bell_length = sizeof(bicycle) / sizeof(uint16_t);
uint8_t bell_length_2 = sizeof(bicycle_2) / sizeof(uint16_t);
uint8_t bell_length_3 = sizeof(bicycle_3) / sizeof(uint16_t);
uint8_t bell_length_4 = sizeof(bicycle_4) / sizeof(uint16_t);

// 멜로디를 하나만 사용하기에 식상해서 하나 더 추가하였다.
// 해당 코드 출처
// 버저, 피에조 스피커로 음악 연주 (슈퍼마리오 배경음악) | Hard Copy World
// https://www.hardcopyworld.com/?p=2772

uint16_t underworld_melody[] = { C4, C5, A3, A4, AS3, AS4, M, M,
C4, C5, A3, A4, AS3, AS4, M, M, F3, F4, D3, D4, DS3, DS4, M, M,
F3, F4, D3, D4, DS3, DS4, M, M, DS4, CS4, D4, CS4, DS4, DS4, GS3,
G3, CS4, C4, FS4, F4, E3, AS4, A4, GS4, DS4, B3, AS3, A3, GS3, M, M, M };
//Underwolrd tempo
uint16_t underworld_tempo[] = { 12, 12, 12, 12, 12, 12, 6, 3, 12, 12, 12, 12,
		12, 12, 6, 3, 12, 12, 12, 12, 12, 12, 6, 3, 12, 12, 12, 12, 12, 12, 6,
		6, 18, 18, 18, 6, 6, 6, 6, 6, 6, 18, 18, 18, 18, 18, 18, 10, 10, 10, 10,
		10, 10, 3, 3, 3 };

uint8_t underworld_length = sizeof(underworld_melody) / sizeof(uint16_t);

void BicycleSong() {
	for (int i = 0; i < bell_length; i++) {
		if (longClick == 0) {
			TIM2->ARR = bicycle[i];
			TIM2->CCR1 = TIM2->ARR / VOLUME;
			HAL_Delay(interval[i]);
			TIM2->CCR1 = 0;
			HAL_Delay(mute[i]);
		}
	}
	for (int i = 0; i < bell_length_2; i++) {
		if (longClick == 0) {
			TIM2->ARR = bicycle_2[i];
			TIM2->CCR1 = TIM2->ARR / VOLUME;
			HAL_Delay(interval_2[i]);
			TIM2->CCR1 = 0;
			HAL_Delay(mute_2[i]);
		}
	}
	for (int i = 0; i < bell_length_3; i++) {
		if (longClick == 0) {
			TIM2->ARR = bicycle_3[i];
			TIM2->CCR1 = TIM2->ARR / VOLUME;
			HAL_Delay(interval_3[i]);
			TIM2->CCR1 = 0;
			HAL_Delay(mute_3[i]);
		}
	}
	for (int i = 0; i < bell_length_4; i++) {
		if (longClick == 0) {
			TIM2->ARR = bicycle_4[i];
			TIM2->CCR1 = TIM2->ARR / VOLUME;
			HAL_Delay(interval_4[i]);
			TIM2->CCR1 = 0;
			HAL_Delay(mute_4[i]);
		}
	}
}

void underworld() {
	for (int i = 0; i < underworld_length; i++) {
		TIM2->ARR = underworld_melody[i];
		TIM2->CCR1 = TIM2->ARR / VOLUME;
		HAL_Delay(1500 / underworld_tempo[i]);
		//TIM2 -> CCR1 = 0;
		//HAL_Delay(mute[i]);
	}
}
