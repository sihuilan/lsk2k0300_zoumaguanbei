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
    ncnn_init("/home/root/tiny_classifier_fp32.ncnn.param", "/home/root/tiny_classifier_fp32.ncnn.bin");
   
    if(uvc_camera_init("/dev/video0") < 0)
    {
        printf("Camera is error\r\n");
        return -1;
    }

    // // //初始化TCP，用于连接逐飞助手
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
            // clock_t clock_start = clock();
        // if( Start_Flag == true)        
        // {

        // AI_check();                       //图像识别 

        // }
        // clock_t clock_end = clock();        
        // LoseLineProtect();
        Protect(Bmp[0], 6);  //停车保护 

        if( Start_Flag == true)        
        {
        // CheckStraight();
        // Check_Circle();
        Cross_check();
        imageDirectionPID();                 //转向控制      
        }
    //     //if(!Start_Flag)
    //    // {
    //     //    Voltage_Get();
    //     //}
        Key_Scan();  
        if(   !Start_Flag &&!Start && !IsGyroOffsetReset)//
        {
            IPS200_display();
        }
        // printf("timesue: %ld us\n", clock_end - clock_start);
    }

    return 0;
    
}

        // for (int i = 0; i < data_statics_r; i++)
        // {
        // // 严格按照表头的顺序，用逗号隔开
        // printf("%d,%d,%d,%d\n", 
        //        i, 
        //        points_r[i][0], 
        //        points_r[i][1], 
        //        dir_r[i]);
        // }
    //     for (int i = 0; i < data_statics_l; i++)
    // {
    //     // 严格按照表头的顺序，用逗号隔开
    //     printf("%d,%d,%d,%d\n", 
    //            i, 
    //            points_l[i][0], 
    //            points_l[i][1], 
    //            dir_l[i]);
    // }
        // for(int g=0;g<ROW;g++)
        // {
        //     P[g] =  (L_boundary[g] - R_boundary[g])/2;
        //     printf("%d = %d\n",g,P[g]);
        // }
        // printf("%d\n",Circle.CircleSize);

        
      // printf("timesue: %ld us\n", clock_end - clock_start);
        // printf("lx_d = %d\n", l_down[0]);
        // printf("ly_d = %d\n", l_down[1]);
        // printf("rx_d = %d\n", r_down[0]);
        // printf("ry_d = %d\n", r_down[1]);
        // printf("------------------\n");
        // printf("lx_up = %d\n", l_up[0]);
        // printf("ly_up = %d\n", l_up[1]);
        // printf("rx_up = %d\n", r_up[0]);
        // printf("ry_up = %d\n", r_up[1]);
        // printf("---------END---------\n");

// printf("lx_d = %d\n", l_down[0]);
        // printf("ly_d = %d\n", L_boundary[0] - R_boundary[0]);
        // printf("rx_d = %d\n", L_boundary[1] - R_boundary[1]);
        // printf("ry_d = %d\n", L_boundary[2] - R_boundary[2]);
        // printf("------------------\n");
        // printf("lx_up = %d\n", l_up[0]);
        // printf("ly_up = %d\n", l_up[1]);
        // printf("rx_up = %d\n", r_up[0]);
        // printf("ry_up = %d\n", r_up[1]);
        // printf("---------END---------\n");
      // clock_t clock_start = clock();
    //     // clock_t clock_end = clock();




    //         printf("Index,Point_X,Point_Y,Direction\n");

    // // // 2. 循环打印你爬到的所有点
    // for (int i = 0; i < ROW; i++)
    // {
    //     P[i] = L_boundary[i] - R_boundary[i];
    //     printf("第%d行 = %d\n", i , P[i]);
    // }
    
    // // 3. 打印一个结束标志，方便你复制
    // printf("--- END OF DATA ---\n");
    // printf("Index,Point_X,Point_Y,Direction\n");

    // 2. 循环打印你爬到的所有点
    // for (int i = 0; i < data_statics_r; i++)
    // {
    //     // 严格按照表头的顺序，用逗号隔开
    //     printf("%d,%d,%d,%d\n", 
    //            i, 
    //            points_r[i][0], 
    //            points_r[i][1], 
    //            dir_r[i]);
    // }
    
    // // 3. 打印一个结束标志，方便你复制
    // printf("--- END OF DATA ---\n");
        // }
        // printf("1 = %d\n", dir_l[69]);
        // printf("2 = %d\n", dir_l[70]);
        // printf("2.5 = %d\n", dir_l[71]);
        // printf("3 = %d\n", dir_l[72]);
        // printf("4 = %d\n", dir_l[73]);
        // printf("5 = %d\n", dir_l[74]);
        // printf("ly = %d\n", left_points_turnDown[1]);
        // printf("1 = %d\n", dir_l[69]);
        // printf("2 = %d\n", dir_r[0]);
        // printf("2.5 = %d\n", dir_r[1]);
        // printf("3 = %d\n", dir_r[2]);
        // printf("4 = %d\n", dir_r[3]);
        // printf("5 = %d\n", dir_r[4]);
    //    printf("ly = %d\n", (int)left_points_turnUp[1]);        
        
        // clock_t clock_end = clock();
        // printf("timesue: %ld us\n", clock_end - clock_start);
        // printf("lx = %d\n", left_points_turnDown[0]);
        // printf("ly = %d\n", left_points_turnDown[1]);
        // printf("lsum = %d\n",data_statics_l);
            // left_points_turnDown[0] = points_l[i][0];
            //     left_points_turnDown[1] = points_l[i][1];