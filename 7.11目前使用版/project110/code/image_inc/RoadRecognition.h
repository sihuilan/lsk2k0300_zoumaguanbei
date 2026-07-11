#ifndef CODE_IMAGE_INC_ROADRECOGNITION_H_
#define CODE_IMAGE_INC_ROADRECOGNITION_H_


#include "Image.h"

//丢线参数
#define     MIN_LINE        6       //丢线情况下边界数最小行数
#define     MAX_LINE        5      //丢线情况下边界数最大行数

//长直道参数
#define     TOPLINE         100//32


//斑马线参数
#define     ZEBRA_DIS       0.9    //无视斑马线行走距离

//十字参数
#define     CROSS_LEFT      39//47              //十字左值范围
#define     CROSS_RIGHT     (179-CROSS_LEFT)    //十字右值范围
#define     WHITE_MAX       179                 //前瞻行最多白点数
#define     WHITE_MIN       135//100             //前瞻行最少白点数

#define     FORK_LEFT       30                  //三岔检测左列
#define     FORK_RIGHT      90                  //三岔检测右列

//坡道参数
#define     RAMP_WIDTH      40                  //坡道宽度

//圆环参数
#define     CIRCLE_COL      16                  //圆环正常赛道列数
#define     CIRCLE_LINE     12                  //圆环最少行数
#define     CIRCLE_WHITE    80                  //圆环中弯道白点数

#define K_MUTATION_LIMIT  0.3f
typedef struct
{
    float zebradis;     //斑马线定距
    float zebraAngle;   //入库打角
    float obstacleAngle;//避障打角
    float AIAngle;      //识别避障时打角
    uint8 zebraLine;

    float crossDis;     //十字定距
    float rampDis;      //坡道定距
    float outrampdis;   //坡道定距
    float obstDis;      //避障定距
    float circleDis;    //圆环定距
    float circleAngle;  //圆环打角
    float breakDis;     //横断定距
    float intermDis;    //辅助定距
    float AIDis;        //识别避障时定距

    uint8 protect;      //保护标志位
    uint8 blind;        //丢线标志位
    uint8 turn;
    uint8 straight;
    uint8 zebra;        //斑马线标志位
    uint8 cross;        //十字标志位
    uint8 ramp;         //坡道标志位
    uint8 circle;       //圆环标志位
    uint8 obstacle;     //避障标志位
    uint8 breaker;       //断路标志位
    uint8 Curve;
    uint8 curve;
    uint8 AI;

    float Obs_N_O_cha;
    uint8 lcur;
    float coerce_rampDis;
    uint8 Last_cross;

    int16 L_left_touch;
    int16 L_right_touch;
    int16 L_top_touch;
    int16 R_left_touch;
    int16 R_right_touch;
    int16 R_top_touch;
}RoadFlagType;

extern RoadFlagType RoadFlag;
extern uint8 ZebraTime;
extern uint8 crosstime;
extern uint8 crossnum;
extern uint8 ramptime;
extern uint8 curvetime;
extern uint8 LongStraightTime;
extern uint8 breaktime;
extern uint8 intermtime;
extern uint8 SpaceDisNum;
extern uint8 zebraNum;
extern uint8 count3;
// extern uint8 cross_flag = 0;

extern uint8 l_up[2];                       
extern uint8 r_up[2];                        
extern uint8 l_down[2];                       
extern uint8 r_down[2];  
extern uint8 l_up_1[2];    
extern uint8 r_up_1[2];
extern uint8 l_down_1[2];        
extern uint8 r_down_1[2];
extern uint8 lesser_Left[2];
extern uint8 lesser_Right[2];



void AI_check(void);
void Protect(uint8 *p, uint8 line);  //*** 
void LoseLineProtect(void);         //*** 
void CheckStraight(void);
void CheckZebra(void);
void Cross_check(void);             //*** 
void Ramp_check(void);
void Obstacle_check(void);
void Breaker_check(void);
void Curve_check(void);

// int16 limit_a_b(int16 x, int a, int b);
// float Slope_Calculate(uint8 begin, uint8 end, uint16 *border);
// void calculate_s_i(uint8 start, uint8 end, uint16 *border, float *slope_rate, float *intercept);



#endif
