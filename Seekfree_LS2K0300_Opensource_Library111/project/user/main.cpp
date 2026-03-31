 #include "zf_common_headfile.h"

 
// // 主函数
// int main(int, char**) {

// ips200_init("/dev/fb0");

// while(1)
// {
//         ips200_clear();
//         ips200_full(RGB565_GRAY);
//         system_delay_ms(500);
//         ips200_full(RGB565_GREEN);
//         system_delay_ms(500);
//         ips200_full(RGB565_BLUE);
//         system_delay_ms(500);
#include "zf_common_headfile.h"
int a =0,b=0;
//中断函数
void pit_callback()
{
    if(Buzzer_Stop)
    {
        Buzzer_Stop--;

        if(Buzzer_Stop == 0)
            gpio_set_level(BEEP, 0);
    }

    //发车延时
    if(Start && !Start_Flag)
    {
        CountStartTime ++;
        if(CountStartTime == 100)
        { 
            CountStartTime = 0;
            Start = false;
            Start_Flag = true;
        }
    }
    // clock_t clock_start = clock();
    Speed_Get();                            //读取编码器
    Speed_Control();                        //速度控制
    Mottor_Control();                       //电机控制
    //brushless_motor_control();              //无刷电调控制
    pthread_mutex_lock(&mutex);
    Attitude_Geting();                      //陀螺仪姿态获取
    pthread_mutex_unlock(&mutex);
    // transfer_image();
    // clock_t clock_end = clock();
    // printf("timesue: %ld us\n", clock_end - clock_start);
            // wave_oscilloscope(); 
}

// 主函数
int main(int, char**) {
    
    // 注册清理函数
    atexit(cleanup);

    // 注册SIGINT信号的处理函数
    signal(SIGINT, sigint_handler);

    //初始化代码    
    DefaultInit();
    FlashData_Init();
    Peripheral_Init();

    if(uvc_camera_init("/dev/video0") < 0)
    {
        printf("Camera is error\r\n");
        return -1;
     }
    // //初始化TCP，用于连接逐飞助手
    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //     printf("tcp_client ok\r\n");
    // }

    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);



    system_delay_ms(10);

    pit_ms_init(PERIODMS, pit_callback); 

    while (true) {  
    // pwm_set_duty(SERVO_MOTOR1_PWM, 21903); //20075 22000 23925  
    // system_delay_ms(20);
        // clock_t clock_start = clock();
        if(wait_image_refresh() < 0)
        {
            pit_ms_deinit();
            cleanup();
            printf("wait_image_refresh error!\n");
            exit(0);
        }
        pthread_mutex_lock(&mutex); 

        // printf("RoadFlag.cross=%d\n",RoadFlag.cross);
        // printf("Cross_WhiteCol[60]=%d\n",Cross_WhiteCol[60]);
        //边缘提取
        Prospect_Border();        
        // 拟合中线
        Fitting_MiddleLine();
        // 统计列白点
        CountColWhite();
        // 统计行白点
        CountRowWhite();
        //统计十字白点
        Cross_CountColWhite();
        // //统计出弯白点
        // obstacle_CountRowWhite();        
        // //迷宫法巡线图像处理
        // image_process();   


        // 前瞻行处理
        Prospect_forwardLook();
        // Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
        // LoseLineProtect();
        pthread_mutex_unlock(&mutex); 
        // //         clock_t clock_end = clock();
        // // printf("timesue: %ld us\n", clock_end - clock_start);
        
                // Ramp_check();
                // Check_Circle();
                // // Cross_check(); 
        //         // // CheckZebra();
                // lesser_curvature_check();
                // printf("RoadFlag.lcur=%d\n",RoadFlag.lcur);
        // //         ImageTurningOutput(); 
        // a+=CarInfo.L_encoder;
        // b+=CarInfo.R_encoder;
        // printf("L= %d\n",CarInfo.L_encoder);
        // printf("R= %d\n",CarInfo.R_encoder);
        // printf("LSUM= %d\n",a);
        // printf("RSUM= %d\n",b);


        if(Start_Flag == true)
        {
                //  Mottor_Control();              //电机控制
                imageDirectionPID();  //转向控制     
                // ips200_showint8(50,10,L_PWM);
                //  ips200_showint8(0,10,R_PWM);

            // if(a<=15)
            // {
            //     a++;
            //     printf("L = %d\n",L_PWM);
            //     printf("R = %d\n",R_PWM);    
            //     if(a=15) a=0;
            // }
        



        //         // wave_oscilloscope(); 

            // Ramp_check();
            // if(!RoadFlag.ramp)
            // {
            //     Check_Circle();
           
            //     // lesser_curvature_check();
            //     Cross_check();                 
            //     // Obstacle_check();
            //     CheckZebra();

            //     // Obstacle_check();
            // } 
            // // 舵机控制
            // ImageTurningOutput(); 
        }
            // transfer_image();
        if(!Start_Flag)
        {
            Voltage_Get();
        }
        Key_Scan();  
        if(   !Start_Flag &&!Start && !IsGyroOffsetReset)//
        {

            IPS200_display();
        }     
        // clock_t clock_end = clock();
        // printf("timesue: %ld us\n", clock_end - clock_start);
    }

    return 0;
}

