#ifndef __USER_H__
#define __USER_H__

#include "main.h"
#include "stdbool.h"


bool MF_PWM_Fingers(int select_finger, int action);
bool MF_PWM_RPS(int action);

bool MF_PWM_handGesture(int gesture);

#endif /* __USER_H__ */
