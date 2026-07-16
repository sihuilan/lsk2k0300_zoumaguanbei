 #include "zf_common_headfile.h"
#include <chrono> 
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

    // pthread_mutex_lock(&mutex);
    Speed_Get();                        //读取编码器        
    Speed_Control();                    //速度控制  
    Mottor_Control();                   //电机控制     
    // pthread_mutex_unlock(&mutex);
    // pthread_mutex_lock(&mutex);   
    Attitude_Geting();                      //陀螺仪姿态获取    
    // pthread_mutex_unlock(&mutex);

    if(Negative_Pressure_Flag )      brushless_motor_control(); 

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

    // // 初始化TCP，用于连接逐飞助手
    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //    printf("tcp_client ok\r\n");
    // }
    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
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
    //     // HSV 远距离减速检测
    //     if (Start_Flag == 1 && RoadFlag.Red == 0 && RoadFlag.AI == 0) 
    //     {
    //     if (detect_red_slow_down(frame_rgb)) {
    //         RoadFlag.Red = 1;   
    //     // printf("Fing Red\n");
    //      //BuzzerMs(20);       // 立即减速
    //     }
    //    }
    // //   if (Start_Flag == 1 && RoadFlag.Red == 0 && RoadFlag.AI == 0) {
    // //     RedBlock_check();  // 检测红块，设置 RoadFlag.Red

    // // }
    //    if(Start_Flag == 1 && RoadFlag.Red == 1 && RoadFlag.AI == 0 ) 
    //    {
    //        AI_check();
    //    }
   // 定义变量
    cv::Rect roi;
    bool need_slow_down = false;
    bool found_red = false;

    if (Start_Flag && RoadFlag.BrickDetected == 0 && RoadFlag.AI == 0) {
        found_red = detect_red_unified(frame_rgb, roi, need_slow_down);
        if (need_slow_down) {
            RoadFlag.Red = 1;
        }
    }
    if (found_red && Start_Flag && RoadFlag.AI == 0 && RoadFlag.AI_Open ==1) {
        AI_check(roi);   // 只传一个参数
    }

      // 立即进行转向控制（不等待后续图像处理）
      if (Start_Flag)
      {
        imageDirectionPID();
       }

    if (RoadFlag.AI == 0) {
    // 十字处理等
    Cross_check();
    // CheckZebra();
    // // Check_Circle();

    }
        // LoseLineProtect();
        Protect(Bmp[0], 6);  //停车保护 
    if(RoadFlag.Red == 1 && RoadFlag.RedDis >= 3.0f)//红块减速清零标志位
       {
        RoadFlag.Red =0;
       }
      Key_Scan(); 

   if(   !Start_Flag &&!Start)//
 {
            IPS200_display();
   }
   }
     my_realtime_pit.stop();
    return 0;
    
}

