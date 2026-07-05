#include "zf_common_headfile.h"
// #include "Car_Control.h"
// 全局变量，存储放大10倍的电压值
uint8 voltage_display = 0;
uint8 Buzzer_Stop;
uint16 adc_reg = 0;
float adc_scale = 0;
float battery_vol = 0;

void BuzzerMs(uint8_t time) { 
    Buzzer_Stop = time / PERIODMS;
    gpio_set_level(BEEP, 0x1);
}

void Voltage_Get(void)
{
        adc_reg = adc_convert(ADC_REG_PATH);
        //adc_scale = adc_get_scale(ADC_SCALE_PATH);

        // adc_reg 为ADC的数字量
        // adc_reg * adc_scale 这两个相乘，得到的是实际的电压值，单位mv

        // 这里的11为硬件分压电阻。
        // 计算公式为: R37+R38/R38 = 11
        battery_vol = adc_reg  * 1.8 /4095 * 11;

        printf("adc_reg = %d\r\n", adc_reg);
        //printf("adc_scale = %f\r\n",adc_scale);
        printf("battery vol = %f\r\n", battery_vol);

}


// uint8_t Get_Battery_Percentage(uint16_t adc_raw) {
    
//     // 1. 满电保护：只要达到你刚测出的峰值附近，无脑返回 100
//     if (adc_raw >= Battery_ADC_FULL) return 100;
    
//     // 2. 亏电保护：低于最低要求，无脑返回 0
//     if (adc_raw <= Battery_ADC_EMPTY) return 0;
    
//     // 3. 纯整数线性计算：单片机处理速度极快，且没有浮点数误差
//     uint32_t percentage = ((adc_raw - Battery_ADC_EMPTY) * 100) / (Battery_ADC_FULL - Battery_ADC_EMPTY);
    
//     return (uint8_t)percentage;
// }