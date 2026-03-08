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
    //摄像头一级PD
    float image_dirEP;
    float image_dirED;

    //摄像头二级PD
    float image_dirTLP;
    float image_dirTRP;
    float image_dirTD;

    //电磁一级PD
    float ad_dirEP;
    float ad_dirED;

    //电磁二级PD
    float ad_dirTP;
    float ad_dirTD;

    float crossline;        //十字补线参数
    float rampline;         //坡道长度

    float rampP;            //坡道转向P
    float rampSpeed;        //坡道速度

/*    //弯道PD
    float BigCurP;
    float SmallCurP;
    float BigCurD;
    float SmallCurD;
*/

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
    float image_traditionSpeed;    //普通速度
    uint8 goSpeed;                 //出库速度
    float zebraSpeed;
    float zebraDis;
    float zebragoL;
    float zebragoR;

    uint8 image_curSpeed1;
    uint8 Circle_Distance;

    float Ramp_Top_whiterow;


    //电磁速度
    uint8 ad_straightSpeed;     //直道速度
    uint8 ad_curSpeed;          //弯道速度
    uint8 ad_traditionSpeed;    //普通速度

    //第二页
    uint8 MODE;             //模式:    0：摄像头     1：电磁
    uint8 forwardLook;      //前瞻行
    uint8 door;             //出库方向  0:左       1:右
    uint8 OBST;             //避障方向  1:左       2:右
    uint8 GOtime;           //圈数         1:一圈   2:两圈

    float Help_Flag;

    float image_dirEP2;
    float BigCurP2;
    float SmallCurP2;

    float image_dirEI;
    float BigCurI;
    float SmallCurI;

    float image_dirED2;
    float BigCurD2;
    float SmallCuD2;

    float L_crossline;        //舵机左转时十字参数
    float R_crossline;        //舵机右转时十字参数

    float L_obstacleline;        //左避障定距
    float R_obstacleline;        //右避障定距

    float crossP;
    float crossD;

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


#pragma once

extern DataInfomationType DataInfo;


void FlashData_Init(void);
void Save_All_Data(void);
void Read_All_Data(void);

#endif