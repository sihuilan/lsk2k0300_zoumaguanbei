#ifndef CODE_INC_SPEED_CONTROL_H_
#define CODE_INC_SPEED_CONTROL_H_


#include "zf_common_headfile.h"
//编码器一米读数
#define ONE_METER     11600//5800

//速度控制参数
#define S_PL             6//15
#define S_IL             3//1
#define S_PR             6//15
#define S_IR             2//1

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

extern int16 SL_output;
extern int16 SR_output;
extern float Speed_L;
extern float Speed_R;


void Speed_Get(void);
void Speed_Control(void);

#endif
