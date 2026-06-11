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
    //   if(Start_Flag == true)
    //     {
    //         a+=CarInfo.L_encoder;
    //         b+=CarInfo.R_encoder;

    //     printf("L= %d\n",CarInfo.L_encoder);
    //     printf("R= %d\n",CarInfo.R_encoder);
    //     printf("LSUM= %d\n",a);
    //     printf("RSUM= %d\n",b);
    //     printf("dis= %f\n",CarInfo.distance);
    //     }                    //速度控制
    Speed_Get();                            //读取编码器
    Speed_Control();   
    Mottor_Control();                     //电机控制       
    //brushless_motor_control();              //无刷电调控制
    pthread_mutex_lock(&mutex);
    Attitude_Geting();                      //陀螺仪姿态获取
    pthread_mutex_unlock(&mutex);
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
    //初始化TCP，用于连接逐飞助手
    //if(tcp_client_init(SERVER_IP, PORT) == 0)
    //{
    //    printf("tcp_client ok\r\n");
    //}

    // // 逐飞助手初始化 设置回调函数
    //seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);
    system_delay_ms(10);
    pit_ms_init(PERIODMS, pit_callback); 
    while (true) {  

        if(wait_image_refresh() < 0)
        {
            pit_ms_deinit();
            cleanup();
            printf("wait_image_refresh error!\n");
            exit(0);
        }
        pthread_mutex_lock(&mutex); 

        //边缘提取
        Prospect_Border();        
        // 拟合中线
        Fitting_MiddleLine();
        // 统计列白点
        CountColWhite();
        // 统计行白点
        CountRowWhite();
        //统计十字白点
        //Cross_CountColWhite();
        // //统计出弯白点
        // obstacle_CountRowWhite();        
        // 前瞻行处理
        Prospect_forwardLook();
        // Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
        // LoseLineProtect();
        pthread_mutex_unlock(&mutex); 

        if(Start_Flag == true)
        {
                //  Mottor_Control();              //电机控制
            imageDirectionPID();  //转向控制     
        }
            // transfer_image();
        if(!Start_Flag)
        {
            Voltage_Get();
        }
        Key_Scan();  
        // if(   !Start_Flag &&!Start && !IsGyroOffsetReset)//
        // {
            IPS200_display();
        // }     
    }

    return 0;
}

