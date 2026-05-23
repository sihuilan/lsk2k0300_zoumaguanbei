 #include "zf_common_headfile.h"
int a =0,b=0,c=0;
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
  ///  推理初始化
   ncnn_init("/home/root/tiny_classifier_fp32.ncnn_2.param", "/home/root/tiny_classifier_fp32.ncnn_2.bin");
     
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


//     // // --- AI 识别部分：只有发车后且每 2 帧才跑一次，防止卡死转向控制 ---
//     if(Start_Flag && (++ai_skip % 2 == 0))
//     {
//     //     if (ncnn_process_frame(frame_rgb, id, conf)) 
//     //     {
//     //         if (id == last_id) 
//     //         stable_cnt++;
//     //         else stable_cnt = 1;
//     //         last_id = id;

//     //         if (stable_cnt >= 3) 
//     //         {
//     //             printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);
//     //             stable_cnt = 0; 
//     //             if(id==0||id==1)
//     //             {
//     //                 CarInfo.diffoffset=65;
//     //                 printf("识别到武器，绕行\n");
//     //             }
//     //         }
//     //     } 
//     //     else {
//     //         stable_cnt = 0; 
//     //     }
//     // }
    
//         if (ncnn_process_frame(frame_rgb, id, conf)) 
//         {
//             if (id == last_id) 
//             stable_cnt++;
//             else stable_cnt = 1;
//             last_id = id;

//     if (stable_cnt >= 2) 
//  {
//     printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);
//     stable_cnt = 0; 

//     // if(id == 0 || id == 1) // 识别到武器
//     // {
//     //     printf("识别到武器，开始绕行...\n");
//     //     // 设置绕行参数
//     //     // 假设正常路中心 error 是 0。给一个 40 的偏差会强制让车向一侧大幅偏转
//     //     bypass_offset = 45;  
//     //     bypass_timer = 60;   // 绕行持续时间，根据车速调整。比如跑过障碍物需要 0.6 秒，则给 60 次循环
//     // }
// }
// }
//  else {
//             stable_cnt = 0; 
//         }
//     }



// --- AI 识别部分 ---
if(Start_Flag)
{
    if (RoadFlag.AI == 0)
    {
        if (ncnn_process_frame(frame_rgb, id, conf)) 
        {
            if (id == last_id) stable_cnt++;
            else stable_cnt = 1;
            last_id = id;

            // 只有连续识别稳定时才触发
            if (stable_cnt >= 2) 
            {  
                printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);

                // if(id == 0 || id == 1) 
                // {
                //     // RoadFlag.AIDis = 0;
                //     // RoadFlag.AI = 1;
                //     // // RoadFlag.AILockFlag = 0;    // 进避障前，确保锁是开着的
                //     // // RoadFlag.AI_LockLeft = 0; 
                //     // printf("识别到武器，开始左转\n");
                // } 
            }
        }
        else
        {
            stable_cnt = 0; // 只有在常规行驶且真没识别到时，才清零计数器
        }
    }
}
        pthread_mutex_lock(&mutex);
        // clock_t clock_start = clock(); //2180 us

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
         obstacle_CountRowWhite();        
        // 前瞻行处理
        Prospect_forwardLook();
         Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
        LoseLineProtect();
        pthread_mutex_unlock(&mutex); 

        // clock_t clock_end = clock();
        //  printf("timesue: %ld us\n", clock_end - clock_start);
 
        if(Start_Flag == true)
        {
        // Cross_check();
        imageDirectionPID();                //转向控制      
        }
        // clock_t clock_end = clock();        
        //if(!Start_Flag)
       // {
        //    Voltage_Get();
        //}
        Key_Scan();  
   if( !Start_Flag &&!Start && !IsGyroOffsetReset)//
    {
            IPS200_display();
    }
    
}  
      return 0;
}

  