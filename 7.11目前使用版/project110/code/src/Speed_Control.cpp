// 【核心新增】定义原子变量，用于跨线程安全传递数据
// std::atomic 保证了读写这个变量是一条机器指令，绝对不会被打断或撕裂
// std::atomic<int16> atomic_turn_speed{0};  
// std::atomic<float> atomic_yaw_rate{0.0f}; // 如果陀螺仪数据也要跨线程，也建议加上

// #define ENCODER_1           "/dev/zf_encoder_1"
// #define ENCODER_2           "/dev/zf_encoder_2"

// float Speed_L,Speed_R;
// float Speed_L_last,Speed_R_last;
// float speedL,speedR;
// float SL_output = 0, SR_output = 0;
// float L_offset = 0, R_offset = 0, IL_offset = 0, IR_offset = 0;
// float DL_offset = 0,DR_offset = 0;
// float L_last_offset = 0, R_last_offset = 0;
// float speed = 0, L_setspeed = 0 , R_setspeed = 0;
// static float ramp_speed = 0; // 静态变量，用于记录平滑过渡的设定速度
// static uint8_t last_start_flag_sc = 0; // 用于在速度环内部检测发车瞬间
    
// static float L_offset_last = 0, L_offset_prev = 0;   // 左轮 e(k-1), e(k-2)
// static float R_offset_last = 0, R_offset_prev = 0;   // 右轮 e(k-1), e(k-2)
// float delta_L = 0;
// float delta_R = 0;
// // static int16_t SL_output = 0;                        // 左轮上一次输出值
// // static int16_t SR_output = 0;                        // 右轮上一次输出值

// int a,b;
#include "zf_common_headfile.h"

#define ENCODER_1           "/dev/zf_encoder_1"
#define ENCODER_2           "/dev/zf_encoder_2"

// --- 变量定义极限优化：统一单精度浮点，杜绝频繁类型转换 ---
float Speed_L = 0.0f, Speed_R = 0.0f;
float Speed_L_last = 0.0f, Speed_R_last = 0.0f;
float speedL = 0.0f, speedR = 0.0f;
float SL_output = 0.0f, SR_output = 0.0f; // 保持 float，直到写入 PWM 寄存器前再强转
float L_offset = 0.0f, R_offset = 0.0f;
float IL_offset = 0.0f, IR_offset = 0.0f; // 增量式实际不需要这俩，若别处没用可删
float DL_offset = 0.0f, DR_offset = 0.0f; // 增量式实际不需要这俩，若别处没用可删
float L_last_offset = 0.0f, R_last_offset = 0.0f; // 增量式实际不需要这俩，若别处没用可删
float speed = 0.0f, L_setspeed = 0.0f, R_setspeed = 0.0f;
static float ramp_speed = 0.0f; 
static uint8_t last_start_flag_sc = 0; 
    
static float L_offset_last = 0.0f, L_offset_prev = 0.0f;   // 左轮 e(k-1), e(k-2)
static float R_offset_last = 0.0f, R_offset_prev = 0.0f;   // 右轮 e(k-1), e(k-2)
static float speedL_filter_last = 0.0f;
static float speedR_filter_last = 0.0f;
float delta_L = 0.0f;
float delta_R = 0.0f;

int a = 0, b = 0;

// 自定义高效单精度浮点限幅内联函数（比调用外部函数更快，减少函数调用开销）
static inline float SafeLimitFloat(float val, float max, float min) {
    if (val > max) return max;
    if (val < min) return min;
    return val;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器读取函数（CPU 优化版）
//-------------------------------------------------------------------------------------------------------------------
void Speed_Get(void)
{
    CarInfo.R_encoder =  encoder_get_count(ENCODER_2);      //读取左轮编码器数值
    CarInfo.L_encoder =  -encoder_get_count(ENCODER_1);     //读取右轮编码器数值

    // 【CPU优化】：将原来的除以 (2*ONE_METER) 修改为乘以预计算的倒数常数，直接消灭除法指令
    const float inv_2_one_meter = 1.0f / (2.0f * (float)ONE_METER);
    float dis = (float)(CarInfo.L_encoder + CarInfo.R_encoder) * inv_2_one_meter;
    CarInfo.distance += dis;                   //平均距离累加

    // 斑马线行驶距离
    if(RoadFlag.zebra)       RoadFlag.zebradis += dis;
    else                     RoadFlag.zebradis = 0.0f;

    // 十字行走距离
    if(RoadFlag.cross)       RoadFlag.crossDis += dis;
    else                     RoadFlag.crossDis = 0.0f;

    // 坡道行走距离
    if(RoadFlag.ramp)        RoadFlag.rampDis += dis;
    else                     RoadFlag.rampDis = 0.0f;

    // 圆环行走距离
    if(RoadFlag.circle)      RoadFlag.circleDis += dis;
    else                     RoadFlag.circleDis = 0.0f;

    // 避障行走距离
    if(RoadFlag.obstacle)    RoadFlag.obstDis += dis;
    else                     RoadFlag.obstDis = 0.0f;

    // 断路行走距离
    if(RoadFlag.breaker)     RoadFlag.breakDis += dis;
    else                     RoadFlag.breakDis = 0.0f;
    
    // 坡道后行走距离
    if(ramptime==1)          RoadFlag.outrampdis += dis;
    else                     RoadFlag.outrampdis = 0.0f;
    
    if(intermtime)           RoadFlag.intermDis += dis;
    else                     RoadFlag.intermDis = 0.0f;
    
    if(RoadFlag.AI)          RoadFlag.AIDis += dis;
    else                     RoadFlag.AIDis = 0.0f;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数（增量式安全且极限性能版）
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
    // 1. 读取原子变量（跨线程安全）
    int16 current_turn_speed = atomic_turn_speed.load(std::memory_order_relaxed);

    // 【核心安全保护：静态防爆冲】
    if (Start_Flag == false) 
    {
        SL_output = 0.0f;
        SR_output = 0.0f;
        L_offset_last = 0.0f; L_offset_prev = 0.0f;
        R_offset_last = 0.0f; R_offset_prev = 0.0f;
        speedL_filter_last = 0.0f;
        speedR_filter_last = 0.0f;
        return; 
    }

    // // 定义差速系数
    // float diff_L_ratio = 0.25f; 
    // float diff_R_ratio = 0.25f;

    // // 直道高速
    if(RoadFlag.straight)
    {
        speed = (float)DataInfo.image_straightSpeed;
    //     diff_L_ratio = 0.20f; 
    //     diff_R_ratio = 0.20f;
    }
    // // 中度弯道
    if(abs(CarInfo.dirOffset) >= 30 && abs(CarInfo.dirOffset) <= 90 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker && RoadFlag.straight == 0)
    {
        speed = (float)DataInfo.image_curSpeed;  
        // diff_L_ratio = 0.28f; 
        // diff_R_ratio = 0.28f;
    }
    // // 大弯道
    // else if(abs(CarInfo.dirOffset) >= 70 && abs(CarInfo.dirOffset) <= 100 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
    // {
    //     speed = (float)DataInfo.image_curSpeed;  
        
    //     if(current_turn_speed > 0) // 右转
    //     {
    //         diff_L_ratio = 0.40f; 
    //         diff_R_ratio = 0.50f; 
    //     }
    //     else // 左转



    //     {
    //         diff_L_ratio = 0.50f; 
    //         diff_R_ratio = 0.40f; 
    //     }
    // }
    // // 丢线速度
    // else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)
    // {
    //     speed = (float)DataInfo.image_curSpeed - 30.0f;
    //     diff_L_ratio = 0.333f; 
    //     diff_R_ratio = 0.333f;
    // }
    // else if(RoadFlag.circle && Circle.CircleSize == 2)
    // {
    //     speed = (float)DataInfo.image_curSpeed;
    //     diff_L_ratio = 0.50f; 
    //     diff_R_ratio = 0.50f;
    // }
    // else if(RoadFlag.circle && Circle.CircleSize == 1)
    // {
    //     speed = (float)DataInfo.image_curSpeed - 10.0f; 
    //     diff_L_ratio = 0.50f; 
    //     diff_R_ratio = 0.50f;
    // }
    else
    {
        speed = (float)DataInfo.image_traditionSpeed;
        // diff_L_ratio = 0.25f;
        // diff_R_ratio = 0.25f;
    }

    // 【优化】：去掉了原先具有安全隐患且消耗算力的 (int16) 强转，全链路保持 float 空间计算
    // speed = (float)DataInfo.image_traditionSpeed;
    float turn_speed_f = (float)current_turn_speed;
    L_setspeed = speed + turn_speed_f * 0.5;//diff_L_ratio
    R_setspeed = speed - turn_speed_f * 0.5;//diff_R_ratio

    // 目标速度限幅（改用纯浮点内联限幅，拒绝向 int 隐式转换）
    L_setspeed = SafeLimitFloat(L_setspeed, 600.0f, 0.0f);
    R_setspeed = SafeLimitFloat(R_setspeed, 600.0f, 0.0f);

    /****************************** 速度当前值计算（极限性能打磨点） **********************/
    // 【终极降噪与计算优化】：
    // 原式: 100 / 0.005 / ONE_METER = 20000 / ONE_METER。
    // 龙芯2K0300上，我们将它预计算为一个乘法系数。彻底消除两轮每5ms一次的浮点除法。
    const float encoder_to_speed_factor = 20000.0f / (float)ONE_METER;
    // speedL = (float)CarInfo.L_encoder * encoder_to_speed_factor;   
    // speedR = (float)CarInfo.R_encoder * encoder_to_speed_factor;
    // 1. 计算当前原始速度
    float raw_speedL = (float)CarInfo.L_encoder * encoder_to_speed_factor;   
    float raw_speedR = (float)CarInfo.R_encoder * encoder_to_speed_factor;

    // 2. 一阶低通滤波 (EMA)
    // 滤波系数 ALPHA 范围 [0.0, 1.0]。
    // ALPHA 越接近 1，跟随越快，滤波越弱；越接近 0，滤波越平滑，延迟越大。
    const float ALPHA = 0.8f; 

    // 公式优化版：仅需一次乘法和两次加减法，完美压榨单精度 FPU 算力
    speedL = speedL_filter_last + ALPHA * (raw_speedL - speedL_filter_last);
    speedR = speedR_filter_last + ALPHA * (raw_speedR - speedR_filter_last);

    // 3. 更新历史值，供下个周期使用
    speedL_filter_last = speedL;
    speedR_filter_last = speedR;

    /****************************** 增量式 PID — 左轮 *********************************/
    L_offset = L_setspeed - speedL;                                         // e(k)
    
    // 增量公式纯单精度浮点运算，完美触发 2K0300 硬件 FPU 的单周期乘加指令
    delta_L = CarInfo.S_LP * (L_offset - L_offset_last)
            + CarInfo.S_LI * L_offset
            + CarInfo.S_LD * (L_offset - 2.0f * L_offset_last + L_offset_prev);

    SL_output += delta_L;                                                   // 历史输出 + 本次增量

    // 更新左轮误差历史
    L_offset_prev = L_offset_last;
    L_offset_last = L_offset;

    /****************************** 增量式 PID — 右轮 *********************************/
    R_offset = R_setspeed - speedR;                                         // e(k)

    delta_R = CarInfo.S_RP * (R_offset - R_offset_last)
            + CarInfo.S_RI * R_offset
            + CarInfo.S_RD * (R_offset - 2.0f * R_offset_last + R_offset_prev);

    SR_output += delta_R;                                                   // 历史输出 + 本次增量

    // 更新右轮误差历史
    R_offset_prev = R_offset_last;
    R_offset_last = R_offset;

    /****************************** PWM 输出控制限幅 *************************************/
    // 保持纯 float 安全限幅，绝对不越界，绝对不发生类型截断
    SL_output = SafeLimitFloat(SL_output, (float)MOTOR_MAX, (float)MOTOR_MIN);
    SR_output = SafeLimitFloat(SR_output, (float)MOTOR_MAX, (float)MOTOR_MIN);

    // 【提示】：在你们最终把 SL_output/SR_output 送进底层硬件底层驱动（比如写到 PWM 寄存器）的那行代码里
    // 记得做最后一次强转即可，例如：set_pwm_left((int16_t)SL_output);
}
// void Speed_Control(void)
// {
//     int16 current_turn_speed = atomic_turn_speed.load(std::memory_order_relaxed);

//     uint16 setspeed;
//     // 定义差速系数（用浮点数乘法代替除法，效率更高，也更灵活）
//     float diff_L_ratio = 0.25f; // 默认 turn_speed / 4
//     float diff_R_ratio = 0.25f;

//     // 直道高速
//     if(RoadFlag.straight)
//     {
//         speed = DataInfo.image_straightSpeed;
//         diff_L_ratio = 0.20f; // 相当于除以 4
//         diff_R_ratio = 0.20f;
//     }
//     // 中度弯道
//     else if(abs(CarInfo.dirOffset) > 20 && abs(CarInfo.dirOffset) < 70 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
//     {
//         speed = DataInfo.image_curSpeed;  
//         diff_L_ratio = 0.28f; // 相当于除以 3
//         diff_R_ratio = 0.28f;
//     }
//     // 大弯道（内侧轮多减速，加强差速）
//     else if(abs(CarInfo.dirOffset) >= 70 && abs(CarInfo.dirOffset) <= 100 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
//     {
//         speed = DataInfo.image_curSpeed;  
//         if(turn_speed > 0) // 右转
//         {
//             diff_L_ratio = 0.40f; // 左轮加一点速 (/3)
//             diff_R_ratio = 0.500f; // 右轮大幅减速 (/2)
//         }
//         else // 左转
//         {
//             diff_L_ratio = 0.500f; // 左轮大幅减速 (/2)
//             diff_R_ratio = 0.40f; // 右轮加一点速 (/3)
//         }
//     }
//     // 丢线速度
//     else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)
//     {
//         speed = DataInfo.image_curSpeed - 30;
//         diff_L_ratio = 0.333f; // 补上丢线时的差速控制
//         diff_R_ratio = 0.333f;
//     }
//     else if(RoadFlag.circle && Circle.CircleSize == 2)
//     {
//         speed = DataInfo.image_curSpeed;
//         diff_L_ratio = 0.500f; // 圆环需要强力差速，用 /2
//         diff_R_ratio = 0.500f;
//     }
//     else if(RoadFlag.circle && Circle.CircleSize == 1)
//     {
//         speed = DataInfo.image_curSpeed - 10; // Size 1 较小，可以适当比 Size 2 再慢一点点
//         diff_L_ratio = 0.500f; // 同样给强力差速 /2
//         diff_R_ratio = 0.500f;
//     }
//     else
//     {
//     speed = DataInfo.image_traditionSpeed;
//     diff_L_ratio = 0.25f;
//     diff_R_ratio = 0.25f;
//     }
//     // L_setspeed = (int16)(speed + turn_speed*0.8 );//* diff_L_ratio
//     // R_setspeed = (int16)(speed - turn_speed*0.8 );//* diff_R_ratio

//     L_setspeed = (int16)(speed + current_turn_speed * 0.8);
//     R_setspeed = (int16)(speed - current_turn_speed * 0.8);
//     // setspeed = speed;//speed
//     // L_setspeed = setspeed + turn_speed/2;//speed
//     // R_setspeed = setspeed - turn_speed/2;
//     L_setspeed = ValueLimitint(L_setspeed, 600, 10);
//     R_setspeed = ValueLimitint(R_setspeed, 600, 10);

// // /******************************速度环的左轮控制*************************************/
// /*********************************************************************************/
//     speedL = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 1m/s，放大一百倍

//     L_offset = L_setspeed - speedL;

//     IL_offset += CarInfo.S_LI * L_offset;
//     IL_offset = ValueLimitfloat(IL_offset, IMAX, IMIN);
//     DL_offset = (L_offset - L_last_offset);     

//     L_last_offset = L_offset;
//     SL_output = (int16)(CarInfo.S_LP * L_offset + IL_offset + DL_offset * CarInfo.S_LD);   //PID控制
//     // SL_output = (int16)(L_setspeed * FF_Ratio + CarInfo.S_LP * L_offset + IL_offset + DL_offset * CarInfo.S_LD); 
// /******************************位置式速度环的右轮控制*************************************/
// /**********************************************************************************/
//     speedR = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;

//     R_offset = R_setspeed - speedR;
   
//     IR_offset += CarInfo.S_RI * R_offset;
//     IR_offset = ValueLimitfloat(IR_offset, IMAX, IMIN);
//     DR_offset = (R_offset - R_last_offset) ;

//     R_last_offset =R_offset;  
//     SR_output = (int16)(CarInfo.S_RP * R_offset + IR_offset + DR_offset * CarInfo.S_RD);
// /****************************** 分段 PWM 限幅 (你的这一段写得很好，保留) *************************************/
//     // if (speedL > 100) {
//     //     if (SL_output < -600) SL_output = -600; 
//     // } else {
//     //     if (SL_output <= -150) SL_output = -150; 
//     // }
    
//     // if (speedR > 100) {
//     //     if (SR_output < -400) SR_output = -400; 
//     // } else {
//     //     if (SR_output <= -100) SR_output = -100; 
//     // }
//     // SR_output = (int16)(R_setspeed * FF_Ratio + CarInfo.S_RP * R_offset + IR_offset + DR_offset * CarInfo.S_RD);   

//     //输出限幅
//     SL_output = ValueLimitint(SL_output, MOTOR_MAX, MOTOR_MIN);
//     SR_output = ValueLimitint(SR_output, MOTOR_MAX, MOTOR_MIN);

// }



    /****************************** 速度计算（必须放在 PID 之前）**********************/
    // speedL = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 左轮速度（放大100倍）
    // speedR = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;   // 右轮速度
    // /****************************** 增量式PID — 左轮 *********************************/
    // L_offset = L_setspeed - speedL;                     // e(k)
    // printf("1 = %d\n",L_offset);
    // // 增量计算 Δu = Kp*(e(k)-e(k-1)) + Ki*e(k) + Kd*(e(k)-2*e(k-1)+e(k-2))
    // delta_L = CarInfo.S_LP * (L_offset - L_offset_last)
    //                 + CarInfo.S_LI *  L_offset
    //                 + CarInfo.S_LD * (L_offset - 2*L_offset_last + L_offset_prev);

    // printf("2 = %f\n",delta_L);
    // SL_output += (int16)delta_L;                        // 累加至输出

    // // 更新误差历史
    // L_offset_prev = L_offset_last;
    // L_offset_last = L_offset;

    // /****************************** 增量式PID — 右轮 *********************************/
    // R_offset = R_setspeed - speedR;

    
    // delta_R = CarInfo.S_RP * (R_offset - R_offset_last)
    //                 + CarInfo.S_RI *  R_offset
    //                 + CarInfo.S_RD * (R_offset - 2*R_offset_last + R_offset_prev);

    // SR_output += (int16)delta_R;

    // R_offset_prev = R_offset_last;
    // R_offset_last = R_offset;
