#include "zf_common_headfile.h"
#include <chrono>
extern cv::Mat frame_rgb;
int AI_id = -1;
float AI_conf = 0.0f;

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
    //clock_t clock_start = clock();   
    Speed_Get();                        //读取编码器        
    Speed_Control();                    //速度控制   
    Mottor_Control();                   //电机控制     
    //brushless_motor_control();              //无刷电调控制
    // pthread_mutex_lock(&mutex);
    Attitude_Geting();                      //陀螺仪姿态获取
    // pthread_mutex_unlock(&mutex);
    //clock_t clock_end = clock();
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
    //  uint8 r = 0,i = 0;
    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
    system_delay_ms(10);
    pit_ms_init(PERIODMS, pit_callback); 
   //推理初始化
   ncnn_init("/home/root/tiny_classifier_fp32.ncnn.param", "/home/root/tiny_classifier_fp32.ncnn.bin");
     
   int stable_cnt = 0;
   int last_id = -1;
 
   while (true)
   { 
    if(wait_image_refresh() < 0)
    {
        pit_ms_deinit();
        cleanup();
        printf("wait_image_refresh error!\n");
        exit(0);
    }
    int id = -1;
    float conf = 0.0f;
    static int frame_cnt = 0;
    frame_cnt++;
     
//--- AI 识别部分 ---
   if(Start_Flag)
{  // static clock_t clock_start = clock();
    // if (RoadFlag.AI == 0 && (frame_cnt % 5 == 0) && is_red_present()) 
    // {
    //     RoadFlag.AI = 1;        // 进入减速准备状态
    //   //  clock_start = clock(); 
    //     RoadFlag.AIDis = 0.0f;  // 清零累积距离
    //     printf("Red block detected\n");
//     // }
   //if (RoadFlag.AI == 0 && (frame_cnt % 5 == 0) && detect_red_slow_down(frame_rgb)) 
   // {
   //     RoadFlag.AI = 1;
   //     RoadFlag.AIDis = 0.0f;
   //     printf("Red block detected, slow down\n");
   // }
    if (RoadFlag.AI == 0 &&  frame_cnt % 2 ==0)
    {  
        if (ncnn_process_frame(frame_rgb, id, conf))
        {
         if (id == last_id) stable_cnt++;
         else stable_cnt = 1;
         last_id = id;
         AI_id = id;
         AI_conf = conf;

            // 只有连续识别稳定时才触发
            if (stable_cnt >= 2 && conf > 0.7f) 
            {  
                printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);
                // if(id == 0 || id == 1)  
                //{
                    RoadFlag.AI = 0;
                    frame_cnt =0;
                   // RoadFlag.AIDis = 0;
               // }    
                //     RoadFlag.AIAngle= 0 ;
                //     printf("识别到武器，开始左转\n");
                // } 
            }
        }
        else
        {
            stable_cnt = 0; // 只有在常规行驶且真没识别到时，才清零计数器
            AI_id = -1;
            AI_conf = 0.0f;
        }
    }
}
// if (RoadFlag.AI == 0 && frame_cnt % 20 == 0)
// {
//     clock_t start = clock();                      // 开始计时
//     bool ok = ncnn_process_frame(frame_rgb, id, conf);
//     clock_t end = clock();                        // 结束计时
//   //  printf("infer time: %ld us\n", end - start);  // 打印完整耗时

//     if (ok)
//     {
//         if (id == last_id) stable_cnt++;
//         else stable_cnt = 1;
//         last_id = id;
//         AI_id = id;
//         AI_conf = conf;

//         if (stable_cnt >= 2 && conf > 0.7f) 
//         {  
//             printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);
//             RoadFlag.AI = 0;
//             frame_cnt = 0;
//         }
//     }
//     else
//     {
//         stable_cnt = 0;
//         AI_id = -1;
//         AI_conf = 0.0f;
//     }
// }
        pthread_mutex_lock(&mutex);
        // clock_t clock_start = clock(); //2180 us
        image_process(); 
        Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
        //LoseLineProtect();
        pthread_mutex_unlock(&mutex); 
        // clock_t clock_end = clock();
        // printf("timesue: %ld us\n", clock_end - clock_start);
        if(Start_Flag == true)
        {
      //  Cross_check();
        imageDirectionPID();                //转向控制      
        }
        // clock_t clock_end = clock();        
        //if(!Start_Flag)
       // {
        //    Voltage_Get();
        //}
       Key_Scan();  
      if( !Start_Flag &&!Start && !IsGyroOffsetReset)
   {
    IPS200_display();
   }  
 }
    return 0;
}

 