#include "user.h"
#include "stdio.h"

#define FLEX_THUMB 134
#define SPREAD_THUMB 60

#define FLEX_INDEX 135
#define SPREAD_INDEX 25

#define FLEX_MIDDLE 25
#define SPREAD_MIDDLE 115

#define FLEX_RING 25
#define SPREAD_RING 105

#define FLEX_PINKY 120
#define SPREAD_PINKY 48
// CCR 값 이동에 대한 대책 설정하기

// 액션(0) : 손가락을 편다. 액션(1) : 손가락을 구부린다.
bool MF_PWM_Fingers(int select_finger, int action)
{
	uint8_t value;
	// thumb == 1
	if (select_finger == 1) {

		if (action == 1)  value = FLEX_THUMB;

		else if (action == 0) value = SPREAD_THUMB;

		TIM4->CCR3 = value;

		return true;
	}

	// index == 2
	if (select_finger == 2) {

		if (action == 1) value = FLEX_INDEX;

		else if (action == 0)	value = SPREAD_INDEX;

		TIM3->CCR3 = value;

		return true;
	}

	if (select_finger == 3) { // 중지

		if (action == 0) value = SPREAD_MIDDLE;

		else if (action == 1)	value = FLEX_MIDDLE;

		TIM2->CCR3 = value;

		return true;
	}

	if (select_finger == 4) { // 약지
		if (action == 0) value = SPREAD_RING;

		else if (action == 1) value = FLEX_RING;

		TIM2->CCR1 = value;

		return true;
	}

	if (select_finger == 5) { // 소지
		if (action == 1) value = FLEX_PINKY;

		else if(action == 0) value = 48;

		TIM2->CCR4 = value;

		return true;
	}
	return false;
}


// action(1) : 가위, action(2) : 바위, action(3) : 보
bool MF_PWM_RPS(int action)
{
	if (action == 1) { // 바위
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = FLEX_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE; //
		TIM2->CCR1 = FLEX_RING; // 방향 반대
		TIM2->CCR4 = FLEX_PINKY;
		return true;

	} else if (action == 2) { // 가위
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
		return true;

	} else if (action == 3) { // 보
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE; //
		TIM2->CCR1 = SPREAD_RING; // 방향 반대
		TIM2->CCR4 = 48;
		return true;
	}

	return false;
}

void MF_PWM_handGesture(int gesture) {

	if (gesture == 0) // 바위
	{
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = FLEX_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE; //
		TIM2->CCR1 = FLEX_RING; // 방향 반대
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 1) // 1
	{
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 2) // V
	{

		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 3) // 3
	{
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = SPREAD_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 4) // 4
			{
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = SPREAD_RING;
		TIM2->CCR4 = SPREAD_PINKY;
	}

	if (gesture == 5) { // 보
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = SPREAD_RING;
		TIM2->CCR4 = SPREAD_PINKY;
	}

	if (gesture == 6) { // good
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = FLEX_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}
	if (gesture == 7) { // 가위
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 8) {
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = SPREAD_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}

	if (gesture == 9) // 락앤롤
			{
		TIM4->CCR3 = SPREAD_THUMB;
		TIM3->CCR3 = FLEX_INDEX;
		TIM2->CCR3 = FLEX_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = SPREAD_PINKY;
	}

	if (gesture == 10) { // 인사
		TIM4->CCR3 = FLEX_THUMB;
		TIM3->CCR3 = FLEX_INDEX;
		TIM2->CCR3 = SPREAD_MIDDLE;
		TIM2->CCR1 = FLEX_RING;
		TIM2->CCR4 = FLEX_PINKY;
	}
}
