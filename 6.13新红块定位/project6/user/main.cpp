 #include "zf_common_headfile.h"
int a =0,b=0,c=0;
int AI_id = -1;
float AI_conf = 0.0f;
extern cv::Mat frame_rgb;
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
    //   if(Start_Flag == true)
    //     {
    //         a+=CarInfo.L_encoder;
    //         b+=CarInfo.R_encoder;

    //     printf("L= %d\n",CarInfo.L_encoder);
    //     printf("R= %d\n",CarInfo.R_encoder);
    //     printf("LSUM= %d\n",a);
    //     printf("RSUM= %d\n",b);
    //     printf("dis= %f\n",CarInfo.distance);
    //     }     
    //clock_t clock_start = clock();   
    Speed_Get();                        //读取编码器        
    Speed_Control();                    //速度控制   
    Mottor_Control();                   //电机控制     
    // brushless_motor_control();              //无刷电调控制
    // pthread_mutex_lock(&mutex);
    Attitude_Geting();                      //陀螺仪姿态获取
    // pthread_mutex_unlock(&mutex);   
    //clock_t clock_end = clock();
    //printf("timesue: %ld us\n", clock_end - clock_start);
    // wave_oscilloscope(); 
    //transfer_image();

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
//     //推理初始化
   ncnn_init("/home/root/tiny_classifier_fp32.ncnn.param", "/home/root/tiny_classifier_fp32.ncnn.bin");
//     // //初始化TCP，用于连接逐飞助手
    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //    printf("tcp_client ok\r\n");
    // }
    // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
    // system_delay_ms(10);
    pit_ms_init(PERIODMS, pit_callback); 
   //   int stable_cnt = 0;
   //   int last_id = -1;
   int frame_cnt =0;
    //  推理初始化
    while (true)
    { 
        if(wait_image_refresh() < 0)
        {
            pit_ms_deinit();
            cleanup();
            printf("wait_image_refresh error!\n");
            exit(0);
        }

if (Start_Flag) {
    if (RoadFlag.AI == 0) {
        clock_t clock_start = clock();
        int target = Get_AIid(frame_rgb);
        clock_t clock_end = clock();
        printf("AI check time: %ld us\n", clock_end - clock_start);
        
        if (target >= 0) {
            // 根据类别设置绕行标志
            if (target == 0 || target == 1) {      // 0_gun, 0_shoulei
                RoadFlag.AI = 1;                    // 启动绕行状态机
                RoadFlag.AIturn = 1;                // 左转标志
            } else if (target == 2 || target == 3) { // 1_first_aid_kit, 1_yanjing
                RoadFlag.AI = 1;
                RoadFlag.AIturn = 3;                // 右转标志
            }
        }
    }
    
   // if (RoadFlag.AI == 1) {
   //     AIControl();   // 绕行控制
   // }
}

        // clock_t clock_start = clock();
        pthread_mutex_lock(&mutex); 
        image_process();
        // Prospect_Border1();
        pthread_mutex_unlock(&mutex); 
        LoseLineProtect();
        Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样 
    //     // clock_t clock_end = clock();

        if(Start_Flag == true)        
        {
    //先执行推理 再绕行
    // CheckStraight();
        Cross_check();
        imageDirectionPID();                //转向控制      
        }
    //     // clock_t clock_end = clock();        
    //     //if(!Start_Flag)
    //    // {
    //     //    Voltage_Get();
    //     //}
        Key_Scan();  
        if(   !Start_Flag &&!Start && !IsGyroOffsetReset)//
        {
            IPS200_display();
     }

    }
    return 0;
    
}
