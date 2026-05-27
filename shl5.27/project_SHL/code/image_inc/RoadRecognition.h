#ifndef CODE_IMAGE_INC_ROADRECOGNITION_H_
#define CODE_IMAGE_INC_ROADRECOGNITION_H_


#include "Image.h"

//丢线参数
#define     MIN_LINE        2       //丢线情况下边界数最小行数
#define     MAX_LINE        12      //丢线情况下边界数最大行数

//长直道参数
#define     TOPLINE         36//32


//斑马线参数
#define     ZEBRA_DIS       0.9    //无视斑马线行走距离

//十字参数
#define     CROSS_LEFT      30//47              //十字左值范围
#define     CROSS_RIGHT     (139-CROSS_LEFT)    //十字右值范围
#define     WHITE_MAX       139                 //前瞻行最多白点数
#define     WHITE_MIN       110//100             //前瞻行最少白点数

#define     FORK_LEFT       30                  //三岔检测左列
#define     FORK_RIGHT      90                  //三岔检测右列

//坡道参数
#define     RAMP_WIDTH      40                  //坡道宽度

//圆环参数
#define     CIRCLE_COL      16                  //圆环正常赛道列数
#define     CIRCLE_LINE     12                  //圆环最少行数
#define     CIRCLE_WHITE    80                  //圆环中弯道白点数

typedef struct
{
    float zebradis;     //斑马线定距
    float zebraAngle;   //入库打角
    float obstacleAngle;//避障打角
    uint8 zebraLine;
    float AIAngle;

    float crossDis;     //十字定距
    float rampDis;      //坡道定距
    float outrampdis;   //坡道定距
    float obstDis;      //避障定距
    float circleDis;    //圆环定距
    float circleAngle;  //圆环打角
    float breakDis;     //横断定距
    float intermDis;    //辅助定距
    float AIpassDis;
    float AIDis;

    uint8 protect;      //保护标志位
    uint8 blind;        //丢线标志位
    uint8 turn;
    uint8 straight;
    uint8 curve;
    uint8 zebra;        //斑马线标志位
    uint8 cross;        //十字标志位
    uint8 ramp;         //坡道标志位
    uint8 circle;       //圆环标志位
    uint8 obstacle;     //避障标志位
    uint8 breaker;       //断路标志位
    uint8 Curve;
    uint8 AI;
    uint8 AIpass;

    float Obs_N_O_cha;
    uint8 lcur;
    float coerce_rampDis;
    uint8 Last_cross;

}RoadFlagType;


extern RoadFlagType RoadFlag;
extern uint8 ZebraTime;
extern uint8 crosstime;
extern uint8 ramptime;
extern uint8 curvetime;
extern uint8 LongStraightTime;
extern uint8 breaktime;
extern uint8 intermtime;
//extern float outramp;

void Protect(uint8 *p, uint8 line);  //*** 
void LoseLineProtect(void);         //*** 
void CheckZebra(void);
void Cross_check(void);             //*** 
void Ramp_check(void);
void Obstacle_check(void);
void Breaker_check(void);
void Curve_check(void);

#endif
