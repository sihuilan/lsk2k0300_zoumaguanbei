 #include "zf_common_headfile.h"
uint8 P[CROP_H];
 extern cv::Mat frame_rgb;
std::atomic<int16> atomic_turn_speed{0};  
// 【新增】真正在内存里定义它
std::atomic<float> atomic_yaw_rate{0.0f};
// 1. 实例化一个定时器对象（通常定义为全局变量，或在主控类的成员变量中）
zf_driver_pit my_realtime_pit;
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

    Speed_Get();                        //读取编码器        
    Speed_Control();                    //速度控制  
    Mottor_Control();                   //电机控制     
    Attitude_Geting();                      //陀螺仪姿态获取    

    if(Negative_Pressure_Flag )   brushless_motor_control(); 


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
    ncnn_init("/home/root/6tiny_classifier_fp32.ncnn.param", "/home/root/6tiny_classifier_fp32.ncnn.bin");

    if(uvc_camera_init("/dev/video0") < 0)
    {
        printf("Camera is error\r\n");
        return -1;
    }

    // // // 初始化TCP，用于连接逐飞助手
    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //    printf("tcp_client ok\r\n");
    // }
    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_inerface_init(tcp_client_send_data, tcp_client_read_data);
    system_delay_ms(10);
    // pit_ms_init(PERIODMS, pit_callback); 
    my_realtime_pit.init_ms(PERIODMS, pit_callback);
    //  推理初始化
    while (true)
    { 
        if(wait_image_refresh() < 0)
        {
            // pit_ms_deinit();
            my_realtime_pit.stop();
            cleanup();
            printf("wait_image_refresh error!\n");
            exit(0);
        }

        //逐飞助手数据传输
        // transfer_image();
        // system_delay_ms(10);

        image_process();
       if(Start_Flag)
        {// 仅在未被红砖标志锁定时进行 HSV 检测
        if (CheckTargetBlock_Fast() && RoadFlag.AI == 0 ) {
             RoadFlag.Red = 1;   // 触发减速
        } 
        cv::Rect roi;
        bool need_slow_down = false;
        bool found_red = false;

        if (RoadFlag.AI == 0 && RoadFlag.AI_Open == 1 && RoadFlag.Red ==1 ) {
            found_red = detect_red_unified(frame_rgb, roi, need_slow_down);
        }

        if (found_red) {
            AI_check(roi);
        }
    }
        LoseLineProtect();
        Protect(Bmp[0], 6);  //停车保护 
       
        if( Start_Flag == true)        
        {
            imageDirectionPID();   //转向控制   
            // CheckStraight();
            // CheckZebra();
            // 十字处理
            // dispose_line();
            Cross_check();
         
            Check_Circle();
            Obstacle_check();
           
        }

        Key_Scan();  

        //   if( !Start_Flag &&!Start)//
        // {
            IPS200_display();
        // }
    }
     my_realtime_pit.stop();
    return 0;
    
}

