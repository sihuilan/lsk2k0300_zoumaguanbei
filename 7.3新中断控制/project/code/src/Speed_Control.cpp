#include "zf_common_headfile.h"


#define ENCODER_1           "/dev/zf_encoder_1"
#define ENCODER_2           "/dev/zf_encoder_2"

float Speed_L,Speed_R;
float Speed_L_last,Speed_R_last;
float speedL,speedR;
float SL_output = 0, SR_output = 0;
float L_offset = 0, R_offset = 0, IL_offset = 0, IR_offset = 0;
float DL_offset = 0,DR_offset = 0;
float L_last_offset = 0, R_last_offset = 0;
float speed = 0, L_setspeed = 0 , R_setspeed = 0;
static float ramp_speed = 0; // 静态变量，用于记录平滑过渡的设定速度
static uint8_t last_start_flag_sc = 0; // 用于在速度环内部检测发车瞬间
    
static uint16_t test_speed = 100;
static uint16_t test_timer = 0;
int32_t cur_L;
int32_t cur_R;
uint8 stoptime;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器读取函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Get(void)
{
    float dis = 0.0;

    CarInfo.R_encoder =  encoder_get_count(ENCODER_2);      //读取左轮编码器数值
    CarInfo.L_encoder =  -encoder_get_count(ENCODER_1);      //读取右轮编码器数值

//    encoder_clear_count(ENCODER_1);                      //清除左轮编码器数值 (三轮库函数里的函数)
//    encoder_clear_count(ENCODER_2);                      //清除右轮编码器数值

    static int32_t last_L = 0, last_R = 0;

    // cur_L = -encoder_get_count(ENCODER_1);
    // cur_R =  encoder_get_count(ENCODER_2);

    // CarInfo.L_encoder = cur_L - last_L;
    // CarInfo.R_encoder = cur_R - last_R;

    // last_L = cur_L;
    // last_R = cur_R;

    dis = ((float)(CarInfo.L_encoder + CarInfo.R_encoder)) / (2*ONE_METER);
    // dis = ((float)(cur_L + cur_R)) / (2*ONE_METER);

    CarInfo.distance += dis;                   //平均距离累加

   //斑马线行驶距离
   if(RoadFlag.zebra)       RoadFlag.zebradis += dis;
   else                     RoadFlag.zebradis = 0;

    //十字行走距离
   if(RoadFlag.cross)       RoadFlag.crossDis += dis;
   else                     RoadFlag.crossDis = 0;

    //坡道行走距离
    if(RoadFlag.ramp)       RoadFlag.rampDis += dis;
    else                    RoadFlag.rampDis = 0;

    //圆环行走距离
    if(RoadFlag.circle)     RoadFlag.circleDis += dis;
    else                    RoadFlag.circleDis = 0;

    //避障行走距离
    if(RoadFlag.obstacle)     RoadFlag.obstDis += dis;
    else                      RoadFlag.obstDis = 0;

    //断路行走距离
    if(RoadFlag.breaker)      RoadFlag.breakDis += dis;
    else                      RoadFlag.breakDis = 0;
    //坡道后行走距离
    if(ramptime==1)           RoadFlag.outrampdis += dis;
    else                      RoadFlag.outrampdis = 0;
    
    if(intermtime)            RoadFlag.intermDis += dis;
    else                      RoadFlag.intermDis = 0;
    
    if(RoadFlag.AI)           RoadFlag.AIDis += dis;
    else                      RoadFlag.AIDis = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
//     static auto last_time = std::chrono::steady_clock::now();
//     auto now = std::chrono::steady_clock::now();
//     float dt = std::chrono::duration<float>(now - last_time).count();
//     last_time = now;
//     speedL = CarInfo.L_encoder * 100 / dt / ONE_METER; // 用真实 dt 换 0.005

    /* ========== ? 新增：高精度 dt 计算 ========== */
    static struct timespec last_time = {0};
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    float dt = 0.005f;   // 默认值，防止第一次调用为 0
    if (last_time.tv_sec != 0) {
        dt = (now.tv_sec - last_time.tv_sec)
           + (now.tv_nsec - last_time.tv_nsec) * 1e-9f;
        
        // ? 防抖：LoongOS 下 dt 偶尔会跳变，限制范围
        if (dt < 0.004f) dt = 0.004f;
        if (dt > 0.006f) dt = 0.006f;
        // printf("%f\n",dt);
    }
    last_time = now;
    /* ============================================ */
    uint16 setspeed;
    // 定义差速系数（用浮点数乘法代替除法，效率更高，也更灵活）
    float diff_L_ratio = 0.25f; // 默认 turn_speed / 4
    float diff_R_ratio = 0.25f;

    // 直道高速
    if(RoadFlag.straight)
    {
        speed = DataInfo.image_straightSpeed;
        diff_L_ratio = 0.20f; // 相当于除以 4
        diff_R_ratio = 0.20f;
    }
    // 中度弯道
    else if(abs(CarInfo.dirOffset) > 25 && abs(CarInfo.dirOffset) < 45 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
    {
        speed = DataInfo.image_curSpeed + 15;  
        diff_L_ratio = 0.28f; // 相当于除以 3
        diff_R_ratio = 0.28f;
    }
    // 大弯道（内侧轮多减速，加强差速）
    else if(abs(CarInfo.dirOffset) >= 45 && abs(CarInfo.dirOffset) <= 90 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)
    {
        speed = DataInfo.image_curSpeed;  
        if(turn_speed > 0) // 右转
        {
            diff_L_ratio = 0.40f; // 左轮加一点速 (/3)
            diff_R_ratio = 0.500f; // 右轮大幅减速 (/2)
        }
        else // 左转
        {
            diff_L_ratio = 0.500f; // 左轮大幅减速 (/2)
            diff_R_ratio = 0.40f; // 右轮加一点速 (/3)
        }
    }
    // 丢线速度
    else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)
    {
        speed = DataInfo.image_curSpeed - 30;
        diff_L_ratio = 0.333f; // 补上丢线时的差速控制
        diff_R_ratio = 0.333f;
    }
    else if(RoadFlag.circle && Circle.CircleSize == 2)
    {
        speed = DataInfo.image_curSpeed;
        diff_L_ratio = 0.500f; // 圆环需要强力差速，用 /2
        diff_R_ratio = 0.500f;
    }
    else if(RoadFlag.circle && Circle.CircleSize == 1)
    {
        speed = DataInfo.image_curSpeed - 10; // Size 1 较小，可以适当比 Size 2 再慢一点点
        diff_L_ratio = 0.500f; // 同样给强力差速 /2
        diff_R_ratio = 0.500f;
    }
    else
    {
    speed = DataInfo.image_traditionSpeed;
    diff_L_ratio = 0.25f;
    diff_R_ratio = 0.25f;
    }
    // if(Start_Flag)
    // {
    //     test_timer++;
    //     if(test_timer >= 100)        // 到达间隔时间
    //     {
    //         test_timer = 0;
    //         if(test_speed < 640)
    //         {
    //             test_speed += 200;
    //             if(test_speed > 640)
    //                 test_speed = 640;
    //         }
    //     }
    //     // speed = test_speed;                    // 使用自动递增的速度
    //     ramp_speed = test_speed;
    // }
    // else 
    // {
    //     ramp_speed = 0; 
    //     last_start_flag_sc = 0; 
    //     test_speed = 0;      // 重新从 100 开始
    //     test_timer = 0;        // 计时清零
    // }


    if(Start_Flag) 
    {
        if(last_start_flag_sc == 0) 
        {
            ramp_speed = 50; // 发车瞬间基准速度
            last_start_flag_sc = 1;
        }
        if(ramp_speed < speed) 
        {
            ramp_speed += 4.0f; // 步进
            if(ramp_speed > speed) ramp_speed = speed;
        } 
        else 
        {
            ramp_speed = speed; 
        }
    } 
    else 
    {
        ramp_speed = 0; 
        last_start_flag_sc = 0; 
    }

    // ==========================================
    // 3. 最终融合：把【实际基准速度】和【差速】赋给左右轮全局变量
    // ==========================================
    L_setspeed = (int16)(ramp_speed + turn_speed * diff_L_ratio);
    R_setspeed = (int16)(ramp_speed - turn_speed * diff_R_ratio);

    // setspeed = speed;//speed
    // L_setspeed = setspeed + turn_speed/2;//speed
    // R_setspeed = setspeed - turn_speed/2;
    L_setspeed = ValueLimitint(L_setspeed, 700, 0);
    R_setspeed = ValueLimitint(R_setspeed, 700, 0);
/******************************速度环的左轮控制*************************************/
/*********************************************************************************/
    // speedL = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 1m/s，放大一百倍
    speedL = CarInfo.L_encoder * 100 / dt / ONE_METER;   // 1m/s，放大一百倍
    // Speed_L =0.3 * speedL + 0.7* Speed_L_last;
    // Speed_L_last = Speed_L;

    L_offset = L_setspeed - speedL;

    IL_offset += CarInfo.S_LI * L_offset;
    IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);
    DL_offset = (L_offset - L_last_offset);     

    L_last_offset = L_offset;
    SL_output = (int16)(CarInfo.S_LP * L_offset + IL_offset + DL_offset * CarInfo.S_LD);   //PID控制
    // SL_output = (int16)(L_setspeed * FF_Ratio + CarInfo.S_LP * L_offset + IL_offset + DL_offset * CarInfo.S_LD); 
/******************************速度环的右轮控制*************************************/
/**********************************************************************************/
    speedR = CarInfo.R_encoder * 100 / dt / ONE_METER;
    // Speed_R =  0.4 * speedR + 0.6 * Speed_R_last; 
    // Speed_R_last = Speed_R;
    // Speed_R = speedR;

    R_offset = R_setspeed - speedR;
   
    IR_offset += CarInfo.S_RI * R_offset;
    IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);
    DR_offset = (R_offset - R_last_offset) ;

    R_last_offset =R_offset;  
    SR_output = (int16)(CarInfo.S_RP * R_offset + IR_offset + DR_offset * CarInfo.S_RD);
/****************************** 分段 PWM 限幅 (你的这一段写得很好，保留) *************************************/
    if (speedL > 100) {
        if (SL_output < -600) SL_output = -600; 
    } else {
        if (SL_output <= -150) SL_output = -150; 
    }
    
    if (speedR > 100) {
        if (SR_output < -400) SR_output = -400; 
    } else {
        if (SR_output <= -100) SR_output = -100; 
    }
    // SR_output = (int16)(R_setspeed * FF_Ratio + CarInfo.S_RP * R_offset + IR_offset + DR_offset * CarInfo.S_RD);   

    //输出限幅
    SL_output = ValueLimitint(SL_output, MOTOR_MAX, MOTOR_MIN);
    SR_output = ValueLimitint(SR_output, MOTOR_MAX, MOTOR_MIN);

}
