#include "zf_common_headfile.h"

// 只有X边界
uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];

// 只有Y边界
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];

// X Y边界都是单独指定的
uint8 x1_boundary[CROP_H], x2_boundary[CROP_H], x3_boundary[CROP_H];
uint8 y1_boundary[CROP_W], y2_boundary[CROP_W], y3_boundary[CROP_W];


uint8 image_copy[CROP_H][CROP_W];

void wave_oscilloscope(void) 
{

    // if(tcp_client_init(SERVER_IP, PORT) == 0)
    // {
    //     printf("tcp_client ok\r\n");
    // }
    // else
    // {
    //     printf("tcp_client error\r\n");
    // }

    
    // 逐飞助手初始化 设置回调函数
    seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);



  // 写入需要发送的数据，有几个通道就写多少个数据
        // 这里仅写入4个通道数据
        //速度环PID

    // seekfree_assistant_oscilloscope_data.data[0] = turn_speed;//CarInfo.L_encoder
    seekfree_assistant_oscilloscope_data.data[0] = Speed_L;//CarInfo.R_encoder
    seekfree_assistant_oscilloscope_data.data[1] = Speed_R;//CarInfo.R_encoder
    // // seekfree_assistant_oscilloscope_data.data[2] = DataInfo.image_traditionSpeed;//CarInfo.L_encoder
    //    seekfree_assistant_oscilloscope_data.data[0] = CarInfo.speed_L;//CarInfo.L_encoder
    //    seekfree_assistant_oscilloscope_data.data[1] = CarInfo.speed_R;//CarInfo.R_encoder
    // seekfree_assistant_oscilloscope_data.data[1] = turn_speed;
    seekfree_assistant_oscilloscope_data.data[2] = L_setspeed;
    seekfree_assistant_oscilloscope_data.data[3] = R_setspeed;
    // seekfree_assistant_oscilloscope_data.data[4] = offset;
    
        // seekfree_assistant_oscilloscope_data.data[3] = L_PWM;
        // seekfree_assistant_oscilloscope_data.data[4] = R_PWM;
        //舵机PID
        // seekfree_assistant_oscilloscope_data.data[0] = CarInfo.dirOffset;//CarInfo.L_encoder
        // seekfree_assistant_oscilloscope_data.data[1] = CarInfo.YawRate;//CarInfo.R_encoder
        // seekfree_assistant_oscilloscope_data.data[2] = Expect_w;
        // seekfree_assistant_oscilloscope_data.data[3] = CarInfo.expectCurvature;
        // seekfree_assistant_oscilloscope_data.data[4] = offset;
        // seekfree_assistant_oscilloscope_data.data[4] = LeftBlack[DataInfo.forwardLook] + RightBlack[DataInfo.forwardLook];
        // seekfree_assistant_oscilloscope_data.data[0] = RoadFlag.blind;
        // seekfree_assistant_oscilloscope_data.data[1] = RoadFlag.cross;
//        detector_oscilloscope_data.data[4] = 10;
//        detector_oscilloscope_data.data[5] = 100;
//        detector_oscilloscope_data.data[6] = 1000;
//        detector_oscilloscope_data.data[7] = 10000;
        
        // 设置本次需要发送几个通道的数据
        seekfree_assistant_oscilloscope_data.channel_num = 5;
        // seekfree_assistant_oscilloscope_data.channel_num = 7;
        
        // 这里进发送了4个通道的数据，最大支持8通道
        seekfree_assistant_oscilloscope_send(&seekfree_assistant_oscilloscope_data);
        
        // system_delay_ms(20);
        // 有可能会在逐飞助手软件上看到波形更新不够连续，这是因为使用WIFI有不确定的延迟导致的
        
        // 解析上位机发送过来的参数，解析后数据会存放在seekfree_assistant_oscilloscope_data数组中，可以通过在线调试的方式查看数据
        // 例程为了方便因此写在了主循环，实际使用中推荐放到周期中断等位置，需要确保函数能够及时的被调用，调用周期不超过20ms
        seekfree_assistant_data_analysis();   
}
void transfer_image(void)
{

#if(1 == INCLUDE_BOUNDARY_TYPE || 2 == INCLUDE_BOUNDARY_TYPE || 4 == INCLUDE_BOUNDARY_TYPE)
    int32 i = 0;
#elif(3 == INCLUDE_BOUNDARY_TYPE)
int32 i = 0;
    int32 j = 0;
#endif

    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);

#if(0 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(仅包含原始图像信息)
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W , CROP_H);


#elif(1 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < CROP_H; i++)
    {
        // // x1_boundary[i] = 50 - (50 - 20) * i / CROP_H;
        // // x3_boundary[i] = 70 + (148 - 70) * i / CROP_H;


        // x2_boundary[i] = COL-1-MiddleLine[49-i];
        // x1_boundary[i] = COL-1-LeftBlack[49-i];
        // x3_boundary[i] = COL-1-RightBlack[49-i];

                
        // x2_boundary[i] = MiddleLine[i];
        // x1_boundary[i] = LeftBlack[i];
        // x3_boundary[i] = RightBlack[i];
    }
    
    //     for(i = 0; i < CROP_W; i++)
    // {

    //     y1_boundary[i] = ROW-1-(DataInfo.forwardLook-2);
    //     y2_boundary[i] = ROW-1-(DataInfo.forwardLook+4);
    //     y3_boundary[i] = ROW-1-(DataInfo.forwardLook+10);
        
    // }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    //seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, Bmp[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, CROP_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);
    // seekfree_assistant_camera_boundary_config(Y_BOUNDARY, CROP_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);



#elif(2 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点)
    // 通常很少有这样的使用需求
    // 对边界数组写入数据
    for(i = 0; i < CROP_W; i++)
    {
        // y1_boundary[i] = 50 - (50 - 20) * i / CROP_H;
        // y2_boundary[i] = CROP_W / 2;
        // y3_boundary[i] = 78 + (78 - 58) * i / CROP_H;
        
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, CROP_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);


#elif(3 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息含有横纵轴坐标)
    // 这样的方式可以实现对于有回弯的边界显示
    j = 0;
    for(i = CROP_H - 1; i >= CROP_H / 2; i--)
    {
        // 直线部分
        xy_x1_boundary[j] = 34;
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = 47;
        xy_y2_boundary[j] = i;
        
        xy_x3_boundary[j] = 60;
        xy_y3_boundary[j] = i;
        j++;
    }

    for(i = CROP_H / 2 - 1; i >= 0; i--)
    {
        // 直线连接弯道部分
        xy_x1_boundary[j] = 34 + (CROP_H / 2 - i) * (CROP_W / 2 - 34) / (CROP_H / 2);
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = 47 + (CROP_H / 2 - i) * (CROP_W / 2 - 47) / (CROP_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;
        
        xy_x3_boundary[j] = 60 + (CROP_H / 2 - i) * (CROP_W / 2 - 60) / (CROP_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }

    for(i = 0; i < CROP_H / 2; i++)
    {
        // 回弯部分
        xy_x1_boundary[j] = CROP_W / 2 + i * (138 - CROP_W / 2) / (CROP_H / 2);
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = CROP_W / 2 + i * (133 - CROP_W / 2) / (CROP_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;
        
        xy_x3_boundary[j] = CROP_W / 2 + i * (128 - CROP_W / 2) / (CROP_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);


#elif(4 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < CROP_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / CROP_H;
        x2_boundary[i] = CROP_W / 2;
        x3_boundary[i] = 80 + (159 - 80) * i / CROP_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, CROP_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#endif


        // // 阻塞式等待，图像刷新
        // if(wait_image_refresh() < 0)
        // {
        //     // 摄像头未采集到图像，这里需要关闭电机，关闭电调等。
        //     exit(0);
        // }


        seekfree_assistant_camera_send();



}