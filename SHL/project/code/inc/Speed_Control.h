#ifndef CODE_INC_SPEED_CONTROL_H_
#define CODE_INC_SPEED_CONTROL_H_


#include "zf_common_headfile.h"
//编码器一米读数
#define ONE_METER     10921

//11600//5800

// #define L_METER 11331
// #define R_METER 10289

#define L_METER 11364
#define R_METER 10478

//速度控制参数

#define IMAX           (200)//300
#define IMIN           (-200)

//电机限幅
#define MOTOR_MAX       999
#define MOTOR_MIN       -999

#define  ADC_REG_L_PATH "/sys/bus/iio/devices/iio:device0/in_voltage5_raw"
#define  ADC_REG_R_PATH "/sys/bus/iio/devices/iio:device0/in_voltage6_raw"
//直道、弯道加减速参数
#define STRAIGHT_LINE   30
#define STRAIGHT_OFFSET 15

extern float SL_output;
extern float SR_output;
extern float Speed_L;
extern float Speed_R;
extern float L_offset , R_offset, IL_offset, IR_offset;
extern float speed, L_setspeed , R_setspeed;

void Speed_Get(void);
void Speed_Control(void);

#endif
