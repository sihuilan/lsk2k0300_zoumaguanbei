#ifndef CODE_INC_PARAMETER_H_
#define CODE_INC_PARAMETER_H_

#pragma once

void DefaultInit();
float ValueLimitfloat(float value, float max, float min);
int16_t ValueLimitint(int16_t value, int16_t max, int16_t min);
float ChangeLimit(float value, float lastvalue, float increaseLimit, float decreaseLimit);
int ChangeLimitint(int value, int lastvalue, int increaseLimit, int decreaseLimit);

#endif