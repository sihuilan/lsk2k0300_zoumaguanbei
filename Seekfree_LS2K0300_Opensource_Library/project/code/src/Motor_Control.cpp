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
    if (RPWM > 0) {
        gpio_set_level(MOTOR1_DIR, 1);        // DIR输出高电平
        pwm_set_duty(MOTOR1_PWM, RPWM);       // 计算占空比
    } else {
        gpio_set_level(MOTOR1_DIR, 0);        // DIR输出低电平
        pwm_set_duty(MOTOR1_PWM, -RPWM);      // 计算占空比
    }

    if (LPWM > 0) {
        gpio_set_level(MOTOR2_DIR, 1);        // DIR输出高电平
        pwm_set_duty(MOTOR2_PWM, LPWM);       // 计算占空比
    } else {
        gpio_set_level(MOTOR2_DIR, 0);        // DIR输出低电平
        pwm_set_duty(MOTOR2_PWM, -LPWM);      // 计算占空比
    }


    //IR2184驱动代码
    // uint16 L_forward, L_backward;
    // uint16 R_forward, R_backward;

    // if(LPWM > 0)
    // {
    //     L_forward = LPWM;
    //     L_backward = 0;
    // }
    // else
    // {
    //     L_forward = 0;
    //     L_backward = -LPWM;
    // }

    // if(RPWM > 0)
    // {
    //     R_forward = RPWM;
    //     R_backward = 0;
    // }
    // else
    // {
    //     R_forward = 0;
    //     R_backward = -RPWM;
    // }
    // pwm_set_duty(PWM_1, R_forward);        // 计算占空比 R_forward
    // pwm_set_duty(MOTOR2_PWM, L_forward);        // 计算占空比
    // pwm_set_duty(PWM_2, R_backward);       // 计算占空比 R_backward
    // pwm_set_duty(MOTOR1_PWM, L_backward);       // 计算占空比
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
    // if(std::fabs(CarInfo.dirOffset) >= 0  && std::fabs(CarInfo.dirOffset) < 10   )
    // {
    //     if(CarInfo.dirOffset > 0)
    //     {
    //         EL_PWM = 600-(DataInfo.brushless_P*CarInfo.dirOffset); //基础值60% 看情况改动 //现在为70%
    //         ER_PWM = 600+(DataInfo.brushless_P*CarInfo.dirOffset);
    //     }
    //     else if(CarInfo.dirOffset < 0)
    //     {
    //         EL_PWM = 600+(DataInfo.brushless_P*CarInfo.dirOffset);
    //         ER_PWM = 600-(DataInfo.brushless_P*CarInfo.dirOffset);
    //     }else
    //     {
    //         EL_PWM = 600;
    //         ER_PWM = 600;
    //     }
    // }            
    // if(RoadFlag.ramp)
    //         {
    //             EL_PWM = 750;
    //             ER_PWM = 750;
    //         }
    //         else
    //         {
            // EL_PWM = DataInfo.fan_speed+(DataInfo.brushless_P*CarInfo.dirOffset);
            // ER_PWM = DataInfo.fan_speed-(DataInfo.brushless_P*CarInfo.dirOffset);        
            // }



    // if((L_offset+SR_output)/2 >= 300)
    //    { 
    //         EL_PWM=600;
    //         ER_PWM=600;
    //    }
    //    else if((SL_output+SR_output)/2 < 300)
    //    {
    //         EL_PWM=550;
    //         ER_PWM=550;
    //    }

    // if (Start_Flag)
    // {
    //     EL_PWM=600;
    //     ER_PWM=600;
    // }else
    // {
    //     EL_PWM=0;
    //     ER_PWM=0;
    // }
    //风扇版，但是出现了在弯道时风扇响应过慢，出弯了才加到目标速的问题。
    // if(std::fabs(CarInfo.dirOffset) >= 10  ) //先试试用同一组能不能行  不行再将圆环和大弯独立出来给&& std::fabs(CarInfo.dirOffset) <= 50&& !RoadFlag.circle
    // {
        if(CarInfo.dirOffset > 25)
        {
            EL_PWM = DataInfo.fan_speed-(DataInfo.brushless_P*CarInfo.dirOffset);
            ER_PWM = DataInfo.fan_speed+(DataInfo.brushless_P*CarInfo.dirOffset); 
        }
        else if(CarInfo.dirOffset < -25)
        {
            EL_PWM = DataInfo.fan_speed+(DataInfo.brushless_P*CarInfo.dirOffset);
            ER_PWM = DataInfo.fan_speed-(DataInfo.brushless_P*CarInfo.dirOffset); 
        }else
        {
            EL_PWM = DataInfo.strfan_speed;
            ER_PWM = DataInfo.strfan_speed;
        }
    // }
        // if(std::fabs(CarInfo.dirOffset) >= 25  )
        // {
        //     EL_PWM = 700;
        //     ER_PWM = 700;
        // }
    // // if(LongLeft>LongRight)
    // // {
    // //     if(CarInfo.dirOffset > 0 && CarInfo.dirOffset < 15)
    // //     {
    // //         EL_PWM = 600-(DataInfo.brushless_P*CarInfo.dirOffset); //基础值60% 看情况改动 //现在为70%
    // //         ER_PWM = 600+(DataInfo.brushless_P*CarInfo.dirOffset);
    // //     }
    // //     else if( CarInfo.dirOffset > 25)
    // //     {
    // //         EL_PWM = 850-(DataInfo.brushless_P*CarInfo.dirOffset); //基础值60% 看情况改动 //现在为70%
    // //         ER_PWM = 850+(DataInfo.brushless_P*CarInfo.dirOffset);
    // //     }
    // // }
    // // else if(LongLeft>LongRight)
    // // {
    // //     if(CarInfo.dirOffset < 0 && CarInfo.dirOffset > -15)
    // //     {
    // //         EL_PWM = 600+(DataInfo.brushless_P*CarInfo.dirOffset); //基础值60% 看情况改动 //现在为70%
    // //         ER_PWM = 600-(DataInfo.brushless_P*CarInfo.dirOffset);
    // //     }
    // //     else if( CarInfo.dirOffset < -25)
    // //     {
    // //         EL_PWM = 850+(DataInfo.brushless_P*CarInfo.dirOffset); //基础值60% 看情况改动 //现在为70%
    // //         ER_PWM = 850-(DataInfo.brushless_P*CarInfo.dirOffset);
    // //     }
    // // }
    // // else if(LongLeft==LongRight)
    // // {
    // //     EL_PWM=600;
    // //     ER_PWM=600;
    // // }
    // if(RoadFlag.ramp == 1)
    // {
    //     EL_PWM = 510;
    //     ER_PWM = 510;
    // }
    // else if(RoadFlag.ramp == 2)
    // {
    //     EL_PWM = 850; 
    //     ER_PWM = 850;
    // }
    //     if (RoadFlag.zebra == 2 && !RoadFlag.protect/*&& RoadFlag.zebradis >= 0.02*/) 
    // {
    //     EL_PWM = 900; // 500
    //     ER_PWM = 900;
    // }
    //     if (RoadFlag.protect) {
    //     EL_PWM = 0;
    //     ER_PWM = 0;
    // }
    ValueLimitint(EL_PWM,MAX_DUTY,MIN_DUTY);
    ValueLimitint(ER_PWM,MAX_DUTY,MIN_DUTY);
    E_PWMSet(EL_PWM, ER_PWM);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      电机控制函数
//-------------------------------------------------------------------------------------------------------------------
void Mottor_Control() {
    if(Start_Flag)
    {
        L_PWM = SL_output;
        R_PWM = SR_output;

        if(DataInfo.MODE==0) //摄像头模式
        {
            //一级
//            L_PWM -=(int16) CarInfo.expectCurvature;
//            R_PWM +=(int16) CarInfo.expectCurvature;
            //二级
            L_PWM +=(int16) turn_PWM;
            R_PWM -=(int16) turn_PWM;
        }
        if(DataInfo.MODE==1) //电磁模式
        {
            //一级
//            L_PWM -=(int16) Expect_curvature;
//            R_PWM +=(int16) Expect_curvature;
            //二级
//            L_PWM +=(int) Turn_PWM;
//            R_PWM -=(int) Turn_PWM;
        }
    }

    else
    {
        L_PWM = 0;
        R_PWM = 0;
    }

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

    if(RoadFlag.zebra==3 && RoadFlag.zebradis >= 0.02)// && RoadFlag.zebradis >= 0.02
    {
        PWMSet(-999,-999);
    }
    //输出限幅
    //L_PWM = ValueLimitint(L_PWM, MOTOR_MAX, MOTOR_MIN);
    //R_PWM = ValueLimitint(R_PWM, MOTOR_MAX, MOTOR_MIN);


    PWMSet(L_PWM, R_PWM);
}    
