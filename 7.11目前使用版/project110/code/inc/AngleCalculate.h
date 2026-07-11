#ifndef CODE_INC_ANGLECALCULATE_H
#define CODE_INC_ANGLECALCULATE_H

#pragma once


#include "Attitude_Calculation.h"
//#include "zf_device_icm20602.h"

//一阶低通滤波权重
#define new_weight           0.35f
#define old_weight           0.65f

extern std::atomic<bool> IsGyroOffsetReset;
void Attitude_Geting(void);
uint8 imu_self_check(void);

#endif
