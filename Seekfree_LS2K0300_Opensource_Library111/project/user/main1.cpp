//    #include "zf_common_headfile.h"



// //测板代码
// void pit_callback()
// {
//     if(Buzzer_Stop)
//     {
//         Buzzer_Stop--;

//         if(Buzzer_Stop == 0)
//             gpio_set_level(BEEP, 0x0);
//     }

//     //发车延时
//     if(Start && !Start_Flag)
//     {
//         CountStartTime ++;
//         if(CountStartTime == 100)
//         {
//             CountStartTime = 0;
//             Start = false;
//             Start_Flag = true;
//         }
//     }
//     Speed_Get();                            //读取编码器
//     Speed_Control();                        //速度控制
//     Mottor_Control();                       //电机控制
//     wave_oscilloscope(); 
//     // Attitude_Geting();                      //陀螺仪姿态获取
// }

// int main(int, char**) 
// {

//     // 注册清理函数
//     atexit(cleanup);

//     // 注册SIGINT信号的处理函数
//     signal(SIGINT, sigint_handler);


//     Peripheral_Init();


//     if(uvc_camera_init("/dev/video0") < 0)
//     {
//         return -1;
//     }
//     if(tcp_client_init(SERVER_IP, PORT) == 0)
//     {
//         printf("tcp_client ok\r\n");
//     }
//     else
//     {
//         printf("tcp_client error\r\n");
//         return -1;
//     }

//     DefaultInit();
    
//     FlashData_Init();
//     pit_ms_init(5, pit_callback);
    
//     while(1)
//      {
//         // if(wait_image_refresh() < 0)
//         // {
//         //     cleanup();
//         //     printf("wait_image_refresh error!\n");
//         //     exit(0);
//         // }
//         // Key_Scan();  

//         // pwm_set_duty(MOTOR1_PWM, 0);               // L_forward           GPIO65
//         // pwm_set_duty(MOTOR2_PWM, 200);         // R_forward           GPIO66
//         // pwm_set_duty(PWM_1, 200);                      // R_forward L_backward          GPIO88
//         // pwm_set_duty(PWM_2, 0);             // R_backward          GPIO89

//         // if( !Start)//!Start_Flag&&
//         // {
//         //     IPS200_display();
//         // }     

// //         // //DRV驱动板电机测试
//         gpio_set_level(MOTOR1_DIR, 0x1);        // DIR输出低电平 
//         pwm_set_duty(MOTOR1_PWM, 300);      // 计算占空比
//         gpio_set_level(MOTOR2_DIR, 0x1);        // DIR输出高电平
//         pwm_set_duty(MOTOR2_PWM, 100);       // 计算占空比
// //         // //IR2184驱动板电机测试
//         // pwm_set_duty(MOTOR1_PWM, 0);               // L_forward           GPIO65
//         // pwm_set_duty(MOTOR2_PWM, 400);         // R_forward           GPIO66
//         // pwm_set_duty(PWM_1, 400);                      // R_forward L_backward          GPIO88
//         // pwm_set_duty(PWM_2, 0);             // R_backward          GPIO89
//            //舵机测试
//pwm_set_duty(SERVO_MOTOR1_PWM, 3700);
// // //         // //蜂鸣器测试
// // //         // BuzzerMs(10);
// // //         // //编码器测试
//         // printf("zf_encoder_left = %d.\r\n", CarInfo.L_encoder);
//         // printf("zf_encoder_right = %d.\r\n", CarInfo.R_encoder);
//         // //陀螺仪测试
//         // printf("zf_gyro_x = %d.\r\n", ICM20602.GYROX);
//         // printf("zf_gyro_y = %d.\r\n", ICM20602.GYROY);
//         // printf("zf_gyro_z = %d.\r\n", ICM20602.GYROZ);
//         // //加速度测试
//         // printf("zf_acc_x = %d.\r\n", ICM20602.ACCX);
//         // printf("zf_acc_y = %d.\r\n", ICM20602.ACCY);
//         // printf("zf_acc_z = %d.\r\n", ICM20602.ACCZ);
// // //         //屏幕和按键检测
//         // if(wait_image_refresh() < 0)
//         // {
//         // // 摄像头未采集到图像，这里需要关闭电机，关闭电调等。
//         //     exit(0);
//         // }

//         // Key_Scan();  

//         // // Key_Text();  //按键检测
        
//         //         if(!Start_Flag&&!Start)//
//         //         {
//         //             IPS200_display();
//         //         }
                
// //                 ips200_show_stringE(0,  6, "Left:", RGB565_RED);
// //                 ips200_showuint8E(40, 6, Left, RGB565_RED);


// // //     //    ips200_displayimage032_zoomB(Bmp[0], UVC_WIDTH, UVC_HEIGHT, 320, 240);  //这里放摄像头捕捉到的图像即（灰度图）
 



// // //         //  system_delay_ms(100);
//        }
//  }


// /************************************************************************************************/
// /******双线程方案*****/
// /************************************************************************************************/

// #include "zf_common_headfile.h"

// // 图像处理线程
// void* imageProcessingThread(void* arg) {
    
//     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//     while (true) {       
//         pthread_testcancel();  // 添加取消点
//         if(wait_image_refresh() < 0) {
//             cleanup();
//             printf("wait_image_refresh error!\n");
//             exit(0);
//         }

//         //边缘提取
//         Prospect_Border();      
//         // 拟合中线
//         Fitting_MiddleLine();
       
//         // 统计列白点
//         CountColWhite();
//         // 统计行白点
//         CountRowWhite();
//         //统计十字白点
//         Cross_CountColWhite();
//         // 前瞻行处理
//         Prospect_forwardLook();


//         Protect(Bmp[0], 5);
//         LoseLineProtect();
//         //舵机控制
//         ImageTurningOutput();
//         if(Start_Flag == true)
//         {
//             Ramp_check();
//             if(!RoadFlag.ramp)
//             {
//                 Check_Circle();
//                 Cross_check();
//                 CheckZebra();
//             }
//         }
//         pthread_rwlock_unlock(&rwlock);
//         system_delay_ms(1);
//     }
//     return NULL;
// }

// // 控制线程
// void* controlProcess(void* arg) {
//     pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
//     pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
//     while (true) {
//         pthread_testcancel();  // 添加取消点
//         Key_Scan();  
//         //        key_test();  //按键检测
        
//         if(wait_image_refresh() < 0) {
//             cleanup();
//             printf("wait_image_refresh error!\n");
//             exit(0);
//         }
//                 if(!Start_Flag &&!Start)//
//                 {
//                     // pthread_rwlock_rdlock(&rwlock); 
//                     IPS200_display();
//                     // pthread_rwlock_unlock(&rwlock);
//                 }
//                 Speed_Get();                            //读取编码器
//                 Speed_Control();                        //速度控制
//                 Mottor_Control();                       //电机控制
//                 Attitude_Geting();                      //陀螺仪姿态获取
            
//         system_delay_ms(5);
//     }
//     return NULL;
// }

// //中断函数
// void pit_callback()
// {
//     if(Buzzer_Stop)
//     {
//         Buzzer_Stop--;

//         if(Buzzer_Stop == 0)
//             gpio_set_level(BEEP, 0);
//     }

//     //发车延时
//     if(Start && !Start_Flag)
//     {
//         CountStartTime ++;
//         if(CountStartTime == 100)
//         {
//             CountStartTime = 0;
//             Start = false;
//             Start_Flag = true;
//         }
//     }
// }

// // 主函数
// int main(int, char**) {
    
//     // 注册清理函数
//     atexit(cleanup);


//     // 注册SIGINT信号的处理函数
//     signal(SIGINT, sigint_handler);

//     //初始化代码
//     Peripheral_Init();

//     if(uvc_camera_init("/dev/video0") < 0)
//     {
//         printf("Camera is error\r\n");
//         return -1;
//     }

//     DefaultInit();
//     FlashData_Init();

//     system_delay_ms(10);

//     pit_ms_init(PERIODMS, pit_callback);

//     // 创建图像处理线程
//     pthread_t img_thread;
//     if(pthread_create(&img_thread, NULL, imageProcessingThread, NULL) != 0) {
//         std::cerr << "Failed to create img_thread" << std::endl;
//         return -1;
//     }
//     //创建控制线程
//     pthread_t control_thread;
//     if(pthread_create(&control_thread, NULL, controlProcess, NULL) != 0) {
//         std::cerr << "Failed to create control_thread" << std::endl;
//         // 终止图像处理线程
//         pthread_cancel(img_thread);
//         return -1;
//     }
//     // 等待线程结束
//     pthread_join(img_thread, NULL);
//     pthread_join(control_thread, NULL);
//     return 0;
// }

// /************************************************************************************************/
// /******视觉主程序，控制中断*****/
// /************************************************************************************************/
/*
#include "zf_common_headfile.h"

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
//    brushless_motor_control();              //无刷电调控制
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
    signal(SIGINT, sigint_handler);  //可以在程序收到中断信号时执行自定义操作

    //初始化代码    
    DefaultInit();
    //FlashData_Init();
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
        Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
        LoseLineProtect();
        pthread_mutex_unlock(&mutex); 
        // //         clock_t clock_end = clock();
        // // printf("timesue: %ld us\n", clock_end - clock_start);
        
                // Ramp_check();
                // Check_Circle();
                // // Cross_check(); 
        //         // // CheckZebra();
                // lesser_curvature_check();
                // printf("RoadFlag.lcur=%d\n",RoadFlag.lcur);
        //         ImageTurningOutput(); 
        if(Start_Flag == true)
        {
        //         // wave_oscilloscope(); 

            Ramp_check();
            if(!RoadFlag.ramp)
            {
                Check_Circle();
           
                // lesser_curvature_check();
                Cross_check();                 
                // Obstacle_check();
                CheckZebra();

                // Obstacle_check();
            } 

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

*/