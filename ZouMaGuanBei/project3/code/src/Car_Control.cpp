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
   // printf("battery vol = %d\r\n", CarInfo.Voltage);
}
// void Voltage_Get(void)
// {
//     uint32_t adc_reg = adc_convert(ADC_REG_PATH);
//     float adc_scale = adc_get_scale(ADC_SCALE_PATH);

//     // 1. 计算当前ADC引脚的真实电压 (单位: mV)
//     float v_adc = (float)adc_reg * adc_scale; 

//     // 2. 乘以分压系数 11
//     float v_bat_raw = v_adc * 11.0f; 

//     // 3. 软件一阶低通滤波
//     // 第一次运行时，直接用当前的 v_bat_raw 赋值，防止滤波器从错误初始值起追赶
//     static bool is_first = true;
//     static float filtered_vol = 0.0f;
//     if (is_first) {
//         filtered_vol = v_bat_raw;
//         is_first = false;
//     }
//     filtered_vol = 0.92f * filtered_vol + 0.08f * v_bat_raw;

//     // 4. 将 mV 转换为 V 并存入结构体
//     CarInfo.Voltage = filtered_vol / 1000.0f;

//     // 5. 打印最终结果（由于乱码，删掉了中文提示，方便查看）
//     printf("RAW_mV = %.2f mV | Battery = %.3f V\r\n", v_bat_raw, CarInfo.Voltage);
// }