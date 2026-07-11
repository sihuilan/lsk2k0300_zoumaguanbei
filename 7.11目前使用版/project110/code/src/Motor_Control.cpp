#include "zf_common_headfile.h"
// #include "Motor_Control.h"

std::atomic<bool> Start_Flag(false);
bool Voltage_Start_Flag = false;
bool Negative_Pressure_Flag = false;

uint8_t Restart_flag = 0;
int16_t L_PWM, R_PWM;
int16_t EL_PWM, ER_PWM;
struct pwm_info motor_1_pwm_info;
struct pwm_info motor_2_pwm_info;
struct pwm_info pwm_1_info;
struct pwm_info pwm_2_info;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM输出配置函数
//-------------------------------------------------------------------------------------------------------------------
static void PWMSet(int16_t LPWM, int16_t RPWM) {
    // //DRV驱动代码
    if (LPWM > 0) {
        gpio_set_level(MOTOR1_DIR, 0);        // DIR输出高电平
        pwm_set_duty(MOTOR1_PWM,LPWM);       // 计算占空比
    } else {
        gpio_set_level(MOTOR1_DIR, 1);        // DIR输出低电平
        pwm_set_duty(MOTOR1_PWM,  -LPWM);      // 计算占空比
    }

    if (RPWM > 0) {
        gpio_set_level(MOTOR2_DIR, 0);        // DIR输出高电平
        pwm_set_duty(MOTOR2_PWM, RPWM);       // 计算占空比
    } else {
        gpio_set_level(MOTOR2_DIR, 1);        // DIR输出低电平
        pwm_set_duty(MOTOR2_PWM, -RPWM);      // 计算占空比
    }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      电调PWM输出配置函数
//-------------------------------------------------------------------------------------------------------------------
static void E_PWMSet(int16_t EL_PWM, int16_t ER_PWM) {
        pwm_set_duty(PWM_1, EL_PWM);   
        pwm_set_duty(PWM_2, ER_PWM);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      无刷电调控制函数
//-------------------------------------------------------------------------------------------------------------------
void brushless_motor_control() 
{   
   
    ValueLimitint(EL_PWM,MAX_DUTY,MIN_DUTY);
    ValueLimitint(ER_PWM,MAX_DUTY,MIN_DUTY);
    // ValueLimitint(ER_PWM,-MIN_DUTY,-MAX_DUTY);
    E_PWMSet(650, 650);
}

void PID_Reset(void) {
    IL_offset = 0;          // 左轮积分清零
    IR_offset = 0;          // 右轮积分清零
    L_last_offset = 0;      // 左轮上次误差清零
    R_last_offset = 0;      // 右轮上次误差清零
    Speed_L_last = 0;       // 左轮滤波历史清零
    Speed_R_last = 0;       // 右轮滤波历史清零
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机控制函数
//-------------------------------------------------------------------------------------------------------------------
void Mottor_Control() {
    // static uint8_t last_start_flag = 0; // 用于检测 Start_Flag 的跳变沿
    if(Start_Flag)
    {
        // // 检测到从 0 切换到 1 的瞬间（即刚发车的一瞬间）
        // if(last_start_flag == 0) {
        //     PID_Reset(); // 清空历史残余，防止发车暴冲
        // }
        // last_start_flag = 1; // 保持状态

        L_PWM = (int16_t)SL_output;
        R_PWM = (int16_t)SR_output;  
        // L_PWM = -SR_output;
        // R_PWM = SL_output;  

        if(RoadFlag.protect)
        {
            L_PWM = 0;
            R_PWM = 0;
        }
        //    else if(!Restart_flag)
        //    {
        //        PWMSet(L_PWM,R_PWM);
        //    }
        //    else if(Restart_flag && !RoadFlag.zebra && !StopCar)
        //    {
        //         PWMSet(L_PWM,R_PWM);
        //    }

        // if(RoadFlag.zebra==3 && RoadFlag.zebradis >= 0.02)// && RoadFlag.zebradis >= 0.02
        // {
        //     PWMSet(-999,-999);
        // }
        //输出限幅
        L_PWM = ValueLimitint(L_PWM, 800, -800);
        R_PWM = ValueLimitint(R_PWM, 800, -800);
        // L_PWM = ValueLimitint(L_PWM, MOTOR_MAX, MOTOR_MIN);
        // R_PWM = ValueLimitint(R_PWM, MOTOR_MAX, MOTOR_MIN);
    PWMSet(L_PWM, R_PWM);
    }
    // if(Start_Flag)
    // {
    //     L_PWM = SL_output;
    //     R_PWM = SR_output;
 
    //     L_PWM +=(int) turn_PWM;
    //     R_PWM -=(int) turn_PWM;
       
    // }
    // else
    // {
    //     L_PWM = 0;
    //     R_PWM = 0;
    // }

    // if(RoadFlag.protect)
    // {
    //     L_PWM = 0;
    //     R_PWM = 0;
    // }
    //     //输出限幅
    // L_PWM = ValueLimitint(L_PWM, MOTOR_MAX, MOTOR_MIN);
    // R_PWM = ValueLimitint(R_PWM, MOTOR_MAX, MOTOR_MIN);
    
    // PWMSet(L_PWM, R_PWM);
}    

