#ifndef __USER_H__
#define __USER_H__

#include "main.h"
#include "stdbool.h"
#include "tim.h"

bool MF_PWM_Fingers(int select_finger, int action);
bool MF_PWM_RPS(int action);

void MF_PWM_handGesture(int gesture);
void MF_PWM_Stop();
void MF_PWM_Start();

#endif /* __USER_H__ */
