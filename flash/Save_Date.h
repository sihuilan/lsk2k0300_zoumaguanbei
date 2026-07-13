#ifndef CODE_INC_SAVE_DATE_H_
#define CODE_INC_SAVE_DATE_H_

// 存储数据用的扇区
#define FLASH_SECTION_INDEX    (0)

//最大页数
#define     Max_Sector          8

//定义数据在Linux系统中储存的文件位置
#define FILENAME_UINT_8         "parameter_uint8.txt"                   
#define FILENAME_INT8           "parameter_int8.txt"                    
#define FILENAME_UINT16         "parameter_uint16.txt"                  
#define FILENAME_INT16          "parameter_int16.txt"                   
#define FILENAME_UINT32         "parameter_uint32.txt"                 
#define FILENAME_INT32          "parameter_int32.txt"                   
#define FILENAME_FLOAT          "parameter_float.txt"                   

//页号（按照变量类型定义页号）
#define     Sector_uint8        0
#define     Sector_int8         1
#define     Sector_uint16       2
#define     Sector_int16        3
#define     Sector_uint32       4
#define     Sector_int32        5
#define     Sector_float        6

typedef struct
{
    //基础赛道的转向PD
    float image_dirTLP;    
    float image_dirTRP;

/***************************************位置环的转向PD*******************************/
    //角速度环PD
    float image_dirEP;
    float image_dirED;

    //直道中的转向PD
    float image_Str_P;
    float image_Str_D;

    //中等弯道的转向PD
    float image_bigP;
    float image_bigD;
        
    //大弯道的转向PD
    float image_superP;
    float image_superD;

    //环岛里面的转向PD
    float Ring_CurP;
    float Ring_CurD;

    //识别绕行时的转向PD
    float AI_CurP;
    float AI_CurD;
/**************************************************************************************/
    float crossline;        //十字补线参数
    float rampline;         //坡道长度

    float rampP;            //坡道转向P
    float rampSpeed;        //坡道速度

    float obstacleP;        //避障转向P
    float obstacleSpeed;    //避障速度
    float obstacleline;      //避障定距
    float obsdis1L;
    float obsdis2L;
    float obsdis3L;
    float obsdis1R;
    float obsdis2R;
    float obsdis3R;

    float traP;//差速Pfl
    float CurP;
    float CirPR50;
    float CirPR60;

    float CurveP;

    //摄像头速度
    float image_straightSpeed;     //直道速度
    float image_curSpeed;          //弯道速度
    float image_BigCurSpeed;
    float image_traditionSpeed;    //普通速度
    float Ring_speed;


    uint8 goSpeed;                 //出库速度
    float zebraSpeed;
    float zebraDis;
    float zebragoL;
    float zebragoR;

    uint8 image_curSpeed1;
    uint8 Circle_Distance;

    float Ramp_Top_whiterow;

    //第二页
    uint8 MODE;             //模式:    0：摄像头     1：电磁
    uint8 forwardLook;      //前瞻行
    uint8 door;             //出库方向  0:左       1:右
    uint8 OBST;             //避障方向  1:左       2:右
    uint8 GOtime;           //圈数         1:一圈   2:两圈

    float Help_Flag;

    float diff_crossP;
    float crossDis;
 
    float coerce_rampDis; 
    float smallcur_P;
    float brushless_P;

    float Obstacle_line;

    float fan_speed;        //风扇速度        
    float cross_speed;      //十字速度
    float strfan_speed;     //直线风扇速度

}DataInfomationType;


typedef struct
{
        char Circle_size1;
        char Circle_size2;
        char Circle_size3;
        char Circle_size4;
        char CircleSize;
        //R50参数
        float Circle_R50In;
        float Circle_R50Out;
        //R60
        float Circle_R60In;
        float Circle_R60Out;
        //R80
        float Circle_R80In;
        float Circle_R80Out;
        //R100
        float Circle_R100In;
        float Circle_R100Out;

        //R60和R80PD，R60和R50PD可通用， R80和R100PD可通用
        float CircleR50_LP;
        float CircleR50_LD;
        float CircleR50_RP;
        float CircleR50_RD;

        float CircleR60_LP;
        float CircleR60_RP;
        float CircleR60_D;

        float CircleR80_EP;
        float CircleR80_ED;
        float CircleR80_P;
        float CircleR80_D;

        float CircleR100_P;
        //辅助定距
        float CircleR50_Dis;
        float CircleR60_Dis;
        float CircleR80_Dis;
        float CircleR100_Dis;

        //圆环速度
        float Speed_R50;
        float Speed_R60;
        float Speed_R80;
/**********************R50圆环的定距参数*******************************/
        float CircleR50_L_3To5_Dis;
        float CircleR50_L_5To7_Dis;
        float CircleR50_L_13_Dis;

        float CircleR50_R_4To6_Dis;
        float CircleR50_R_6To8_Dis;
        float CircleR50_R_14_Dis;

        float CircleR50_5To7_Ang;
        float CircleR50_7To9_Ang;
        float CircleR50_9To13_Ang;
/**********************R60圆环的定距参数*******************************/
        float CircleR60_L_3To5_Dis;
        float CircleR60_L_5To7_Dis;
        float CircleR60_L_13_Dis;

        float CircleR60_R_4To6_Dis;
        float CircleR60_R_6To8_Dis;
        float CircleR60_R_14_Dis;

        float CircleR60_5To7_Ang;
        float CircleR60_7To9_Ang;
        float CircleR60_9To13_Ang;
/***********************圆环转向角度******************************/


}ImageCircleType;


typedef struct
{
        float distance;           //平均行驶距离

        float Pitch;            //俯仰角
        float PitchRate;        //俯仰角速度
        float Yaw;              //偏航角
        float YawRate;          //偏航角速度
        float Last_YawRate;
        float expectCurvature;  //期望曲率
float Voltage;          //电池电压

        int16 L_encoder;         //左轮编码器读数
        int16 R_encoder;         //右轮编码器读数

        float speed_L;           //左轮速度
        float speed_R;           //右轮速度

        int dirOffset;        //摄像头转向偏差
        int16 lastDirOffset;    //摄像头历史转向偏差

        int16 turn_PWM;
        float Last_Yaw;

        float Last_Pitch;

        float   last_Lk;
        float   last_Rk;

        float S_LP;             //速度环左轮pid
        float S_LI;
        float S_LD; 

        float S_RP;             //速度环右轮pid
        float S_RI;
        float S_RD;

}CarInfomationType;

#pragma once

extern CarInfomationType CarInfo;
extern ImageCircleType Circle;
extern DataInfomationType DataInfo;


void FlashData_Init(void);
void Save_All_Data(void);
void Read_All_Data(void);

#endif