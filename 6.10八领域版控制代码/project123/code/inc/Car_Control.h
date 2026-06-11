#ifndef CODE_INC_CAR_CONTROL_H
#define CODE_INC_CAR_CONTROL_H



#define BEEP "/dev/zf_driver_gpio_beep"
#define  ADC_REG_PATH "/sys/bus/iio/devices/iio:device0/in_voltage7_raw"
#define  ADC_SCALE_PATH "/sys/bus/iio/devices/iio:device0/in_voltage_scale"


#pragma once

extern uint8 Buzzer_Stop;
extern float adc_scale;

void BuzzerMs(uint8_t time);
void Voltage_Get(void);
#endif