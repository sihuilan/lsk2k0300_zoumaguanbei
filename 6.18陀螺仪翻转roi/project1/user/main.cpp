#include "zf_common_headfile.h"
int a =0,b=0,c=0;
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
        // {
        //     a+=CarInfo.L_encoder;
        //     b+=CarInfo.R_encoder;

        // printf("L= %d\n",CarInfo.L_encoder);
        // printf("R= %d\n",CarInfo.R_encoder);
        // printf("LSUM= %d\n",a);
        // printf("RSUM= %d\n",b);
        // printf("dis= %f\n",CarInfo.distance);
        // }  
    Speed_Get();                        //读取编码器        
    Speed_Control();                    //速度控制   
    Mottor_Control();                   //电机控制     
    // brushless_motor_control();              //无刷电调控制
    // pthread_mutex_lock(&mutex);
    Attitude_Geting();                      //陀螺仪姿态获取
    // pthread_mutex_unlock(&mutex);
           
 if(Negative_Pressure_Flag )      brushless_motor_control(); 

    // transfer_image();
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
   ncnn_init("/home/root/btiny_classifier_fp32.ncnn.param", "/home/root/btiny_classifier_fp32.ncnn.bin");
   
    if(uvc_camera_init("/dev/video0") < 0)
    {
        printf("Camera is error\r\n");
        return -1;
    }

    // //初始化TCP，用于连接逐飞助手
    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //    printf("tcp_client ok\r\n");
    // }
    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
    system_delay_ms(10);
    pit_ms_init(PERIODMS, pit_callback); 
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

        pthread_mutex_lock(&mutex); 
        image_process();
        pthread_mutex_unlock(&mutex); 
       //static uint8 hsv_slow_check_cnt = 0;
    //   if(Start_Flag == 1 && RoadFlag.Red == 0)
    // {
    //     if(++red_check_frame_cnt >= 3) 
    //     {
    //     red_check_frame_cnt = 0;
    //   //  clock_t clock_start = clock();
    //     RedBlock_check();
    //   //   clock_t clock_end = clock();

    //   //  printf("red: %ld us\n", clock_end - clock_start);
    //     }
    // }
    if (Start_Flag == 1 && RoadFlag.Red == 0 && RoadFlag.AI == 0) {
    // // 每 5 帧执行一次 HSV 远距离减速检测
    //if (++hsv_slow_check_cnt >= 1) {
       // hsv_slow_check_cnt = 0;
        if (detect_red_slow_down(frame_rgb)) {
            RoadFlag.Red = 1;   
         //BuzzerMs(20);       // 立即减速
        }
    }
    // }
    if(Start_Flag == 1 && RoadFlag.Red == 1 )
    {
        AI_check();
    if(RoadFlag.Red ==1)
    {
        RoadFlag.RedDis >= 3.0;
        RoadFlag.Red =0;
    }
    }
    
       LoseLineProtect();
       Protect(Bmp[0], 6);  //停车保护 

       if(Start_Flag == true)        
       {
    //    Check_Circle();
       Cross_check();
      imageDirectionPID();                 //转向控制      
       } 
    //     if(!Start_Flag)
    //    {
    //        Voltage_Get();
    //     }
        Key_Scan();  
   if(!Start_Flag &&!Start)//
    {
            IPS200_display();
    }
}   
return 0;

}