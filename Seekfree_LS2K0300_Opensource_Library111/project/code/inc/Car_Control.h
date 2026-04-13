#ifndef CODE_INC_CAR_CONTROL_H
#define CODE_INC_CAR_CONTROL_H



#define BEEP "/dev/zf_driver_gpio_beep"
#define  ADC_REG_PATH "/sys/bus/iio/devices/iio:device0/in_voltage7_raw"
#define  ADC_SCALE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage_scale"


#pragma once


typedef struct
{
        float distance;           //平均行驶距离

        float Pitch;            //俯仰角
        float PitchRate;        //俯仰角速度
        float Yaw;              //偏航角
        float YawRate;          //偏航角速度
        float Last_YawRate;
        float expectCurvature;  //期望曲率
float Voltage;          //电池电压

        int16 L_encoder;         //左轮编码器读数
        int16 R_encoder;         //右轮编码器读数

        float speed_L;           //左轮速度
        float speed_R;           //右轮速度

        float SERVO_PWM;         //舵机PWM

        float SL_P;
        float SR_P;
        float SL_D;
        float SR_D;
        float SL_I;
        float SR_I;     


        int16 dirOffset;        //摄像头转向偏差
        int16 lastDirOffset;    //摄像头历史转向偏差

        int16 turn_PWM;
        float I_offset;
        float Last_Yaw;

        float SL_EP;
        float SR_EP;
        float SL_ED;
        float SR_ED;
        float SL_EI;
        float SR_EI;

        float Last_Pitch;

        float servo;
        float   last_Lk;
        float   last_Rk;

        float SL_WP;
        float SR_WP;
        float SL_WD;
        float SR_WD;
        float SL_WI;
        float SR_WI;     

        float S_LP;             //速度环左轮pid
        float S_LI;
        float S_LD; 

        float S_RP;             //速度环右轮pid
        float S_RI;
        float S_RD
}CarInfomationType;

extern CarInfomationType CarInfo;

extern uint8 Buzzer_Stop;
extern float adc_scale;

void BuzzerMs(uint8_t time);
void Voltage_Get(void);
#endif