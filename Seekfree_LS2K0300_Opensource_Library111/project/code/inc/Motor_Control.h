#ifndef CODE_INC_MOTOR_CONTROL_H_
#define CODE_INC_MOTOR_CONTROL_H_



#define MOTOR1_DIR   "/dev/zf_driver_gpio_motor_1" //右电机
#define MOTOR1_PWM   "/dev/zf_device_pwm_motor_1"

#define MOTOR2_DIR   "/dev/zf_driver_gpio_motor_2" //左电机
#define MOTOR2_PWM   "/dev/zf_device_pwm_motor_2"

#define PWM_1           "/dev/zf_device_pwm_esc_1"
#define PWM_2           "/dev/zf_device_pwm_esc_2"

// 在设备树中，设置的10000。如果要修改，需要与设备树对应。
#define MOTOR1_PWM_DUTY_MAX    (motor_1_pwm_info.duty_max)       
// 在设备树中，设置的10000。如果要修改，需要与设备树对应。 
#define MOTOR2_PWM_DUTY_MAX    (motor_2_pwm_info.duty_max)        

#define MAX_DUTY        900    
#define MIN_DUTY        500    


#pragma once

extern struct pwm_info motor_1_pwm_info;
extern struct pwm_info motor_2_pwm_info;

extern struct pwm_info pwm_1_info;
extern struct pwm_info pwm_2_info;

extern std::atomic<bool> Start_Flag;
extern uint8_t Restart_flag;
extern int16_t L_PWM, R_PWM;
void Mottor_Control();
void brushless_motor_control(); 
#endif