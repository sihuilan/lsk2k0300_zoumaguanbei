#include "zf_common_headfile.h"
// #include "Motor_Control.h"

std::atomic<bool> Start_Flag(false);
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
    E_PWMSet(300, 300);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机控制函数
//-------------------------------------------------------------------------------------------------------------------
void Mottor_Control() {
    if(Start_Flag)
    {
        L_PWM = SL_output;
        R_PWM = SR_output;  
            //一级
        //    L_PWM -=(int16) CarInfo.expectCurvature;
        //    R_PWM +=(int16) CarInfo.expectCurvature;
         //   二级
        //  L_PWM +=(int16) turn_PWM;
        //  R_PWM -=(int16) turn_PWM;

        //     else
        //     {
        //         L_PWM = 0;
        //         R_PWM = 0;
        //     }

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
        // L_PWM = ValueLimitint(L_PWM, MOTOR_MAX, MOTOR_MIN);
        // R_PWM = ValueLimitint(R_PWM, MOTOR_MAX, MOTOR_MIN);
    L_PWM = ValueLimitint(L_PWM, 800, -700);
    R_PWM = ValueLimitint(R_PWM, 800, -700);
    PWMSet(L_PWM, R_PWM);
     }
}    
