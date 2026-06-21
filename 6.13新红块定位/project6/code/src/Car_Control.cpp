#include "zf_common_headfile.h"
// #include "Car_Control.h"

uint8 Buzzer_Stop;
uint16 adc_reg = 0;
float adc_scale = 0;
uint16 battery_vol = 0;

void BuzzerMs(uint8_t time) { 
    Buzzer_Stop = time / PERIODMS;
    gpio_set_level(BEEP, 0x1);
}

void Voltage_Get(void)
{
   adc_reg = adc_convert(ADC_REG_PATH);
   adc_scale = adc_get_scale(ADC_SCALE_PATH);

   // adc_reg 为ADC的数字量

   // adc_reg * adc_scale 这两个相乘，得到的是实际的电压值，单位mv

   // 这里的11为硬件分压电阻。
   // 计算公式为: R37+R38/R38 = 11
   CarInfo.Voltage = adc_reg * adc_scale * 11* 0.001+0.3;

//    printf("adc_reg = %d\r\n", adc_reg);
//    printf("adc_scale = %f\r\n", adc_scale);
//    printf("battery vol = %d\r\n", CarInfo.Voltage);
}
