#include "zf_common_headfile.h"
// #include "ImageDirection_Control.h"
//#include "Motor_cont"
//#incude "Init.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void Flag_clear(void)
{
    RoadFlag.protect=0;
    RoadFlag.blind=0;
    RoadFlag.zebra=0;
    RoadFlag.cross=0;
//    RoadFlag.Last_cross=0;
    RoadFlag.ramp=0;
    RoadFlag.circle=0;
    RoadFlag.obstacle=0;
//    RoadFlag.lcur = 0;
    DataInfo.Help_Flag=0;
    CarInfo.distance = 0;
    RoadFlag.zebraLine = 6; // 6 8 5
    CarInfo.Pitch=0;
    CarInfo.Yaw=0;
//    zebraNum=0;
    crosstime=0;
//    crossnum=0;

}

void Peripheral_Init(void)
{
    //初始化互斥锁
    pthread_mutex_init(&mutex, NULL);
    // 初始化屏幕
    ips200_init("/dev/fb0");
    // 获取PWM设备信息
    // pwm_get_dev_info(MOTOR1_PWM, &motor_1_pwm_info);
    // pwm_get_dev_info(MOTOR2_PWM, &motor_2_pwm_info);
    // // 获取电调PWM信息
    // pwm_get_dev_info(PWM_1, &pwm_1_info);
    // pwm_get_dev_info(PWM_2, &pwm_2_info);
    //获取舵机PWM设备信息
 //   pwm_get_dev_info(SERVO_MOTOR1_PWM, &servo_pwm_info);
//    pwm_set_duty(SERVO_MOTOR1_PWM, MOTOR_INIT);
    //初始化陀螺仪 
//     imu_get_dev_info();
//     imu_self_check();
//    CarInfo.Last_Yaw=0.0;
    
}
void cleanup()
{
    printf("程序异常退出，执行清理操作\n");
    // 处理程序退出！！！
    // 这里需要关闭电机，关闭电调等。
    //电机关闭
    pwm_set_duty(MOTOR1_PWM, 0);   
    pwm_set_duty(MOTOR2_PWM, 0);
    // //无刷电调的关闭
    pwm_set_duty(PWM_1, 500);   
    pwm_set_duty(PWM_2, 500);
    //舵机归置起始位
//    pwm_set_duty(SERVO_MOTOR1_PWM, 0);
    //蜂鸣器关闭
    gpio_set_level(BEEP, 0x0);   
    ips200_clear();
    Flag_clear();
    pthread_mutex_destroy(&mutex);  // 销毁互斥锁    
}
void sigint_handler(int signum) 
{
    printf("收到Ctrl+C，程序即将退出\n");
    pit_ms_deinit();
    
    exit(0);
}

