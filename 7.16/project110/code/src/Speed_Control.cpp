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

    // 坡道后行走距离
    if(ramptime==1)          RoadFlag.outrampdis += dis;
    else                     RoadFlag.outrampdis = 0.0f;
    
    if(RoadFlag.AI)          RoadFlag.AIDis += dis;
    else                     RoadFlag.AIDis = 0.0f;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数（增量式安全且极限性能版）
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
    // 引入局部静态变量，专门缓存起步状态与步进基础速度
    static bool is_starting_up = true;
    static float base_speed_ramp = 0.0f;
    // 1. 读取原子变量（跨线程安全）
    int16 current_turn_speed = atomic_turn_speed.load(std::memory_order_relaxed);
    float turn_speed_f = (float)current_turn_speed;

    // 【核心安全保护：静态防爆冲】
    if (Start_Flag == false) 
    {
        SL_output = 0.0f;
        SR_output = 0.0f;
        L_offset_last = 0.0f; L_offset_prev = 0.0f;
        R_offset_last = 0.0f; R_offset_prev = 0.0f;
        speedL_filter_last = 0.0f;
        speedR_filter_last = 0.0f;

        // 【关键改动】：每次关闭发车开关时，自动将起步状态和速度步进归零，为下一次发车做好准备
        is_starting_up = true;
        base_speed_ramp = 0.0f;
        return; 
    }

    float diff_L_ratio = 0.5f; // 默认 turn_speed / 4
    float diff_R_ratio = 0.5f;

    if(RoadFlag.straight && RoadFlag.inside_CrossRound == 0 && !RoadFlag.circle)
    {
        speed = (float)DataInfo.image_straightSpeed;
        diff_L_ratio = 0.25f; // 同样给强力差速 /2
        diff_R_ratio = 0.250f;
    }
    else if(fabs(CarInfo.dirOffset) > 70 && RoadFlag.inside_CrossRound == 0 && !RoadFlag.circle) 
    {
        speed = (float)DataInfo.image_BigCurSpeed;  
        if(CarInfo.dirOffset < 0) // 右转
        {
            diff_L_ratio = 0.5f; // 左轮加一点速 (/3)
            diff_R_ratio = 0.70f; // 右轮大幅减速 (/2)
        }
        else // 左转
        {
            diff_L_ratio = 0.70f; // 左轮大幅减速 (/2)
            diff_R_ratio = 0.5f; // 右轮加一点速 (/3)
        }
        
    }
    else if(fabs(CarInfo.dirOffset) >= 20 && fabs(CarInfo.dirOffset) <= 70 && RoadFlag.inside_CrossRound == 0 && !RoadFlag.circle) 
    {
        speed = (float)DataInfo.image_curSpeed;  
        diff_L_ratio = 0.500f; // 同样给强力差速 /2
        diff_R_ratio = 0.500f;
    }
    else if(RoadFlag.circle)
    {
        // speed = (float)DataInfo.Ring_speed;
        speed = (float)DataInfo.AI_speed;
    }
    else if(RoadFlag.inside_CrossRound == 1 && !RoadFlag.circle)
    {
        if(  fabs(CarInfo.dirOffset) <= 50 && fabs(CarInfo.dirOffset) >= 20) 
        {
            speed = (float)DataInfo.image_BigCurSpeed;  
        }
        if(fabs(CarInfo.dirOffset) > 50) 
        {
            speed = (float)DataInfo.Ring_speed; 
        }       
        else
        {
            speed = (float)DataInfo.image_traditionSpeed;            
        }   
    }
    else 
    {
        speed = (float)DataInfo.image_traditionSpeed;
        diff_L_ratio = 0.25f; // 同样给强力差速 /2
        diff_R_ratio = 0.250f;
    }

    // 大弯道
    // else if(abs(CarInfo.dirOffset) >= 70 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
    // {
    //     speed = (float)DataInfo.image_curSpeed;  
    // }
    // else
    // {
    //     speed = (float)DataInfo.image_traditionSpeed;
    // }
/* ------------------- 【核心改动：纵向起步步进控制】 ------------------- */
    if (is_starting_up) 
    {
        // 步进加速度：每个控制周期（如2ms）允许增加的速度单位
        // 调参建议：如果发车仍有轻微翘头，调小此值；如果觉得起步过肉，调大此值
        const float START_ACCEL = 3.0f; 
        
        base_speed_ramp += START_ACCEL;
        
        // 当步进的速度追平或超过期望的目标速度时，彻底退出起步模式
        if (base_speed_ramp >= speed) 
        {
            base_speed_ramp = speed;
            is_starting_up = false; 
        }
    }
    else
    {
        // 正常行驶阶段：步进器完全退出，直接透传速度，避免多余的比较与计算开销
        base_speed_ramp = speed;
    }
    /* ----------------------------------------------------------------- */
    L_setspeed = (int16)(base_speed_ramp + current_turn_speed * 0.5);
    R_setspeed = (int16)(base_speed_ramp - current_turn_speed * 0.5);
    // if(RoadFlag.inside_CrossRound)
    // {  
    // L_setspeed = (speed + turn_speed_f) * 0.6;
    // R_setspeed = (speed - turn_speed_f) * 0.4;
    // }
    // else 
    // {
    // L_setspeed = (speed + turn_speed_f) * 0.8;
    // R_setspeed = (speed - turn_speed_f) * 0.8;      
    // }
    
    // 目标速度限幅（改用纯浮点内联限幅，拒绝向 int 隐式转换）
    L_setspeed = SafeLimitFloat(L_setspeed, 600.0f, 0.0f);
    R_setspeed = SafeLimitFloat(R_setspeed, 600.0f, 0.0f);


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