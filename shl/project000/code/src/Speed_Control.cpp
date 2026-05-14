#include "zf_common_headfile.h"


#define ENCODER_1           "/dev/zf_encoder_1"
#define ENCODER_2           "/dev/zf_encoder_2"

float Speed_L,Speed_R;
float Speed_L_last,Speed_R_last;
float speedL,speedR;
float SL_output = 0, SR_output = 0;
float L_offset = 0, R_offset = 0, IL_offset = 0, IR_offset = 0;
float DL_offset = 0,DR_offset = 0;
float L_last_offset = 0, R_last_offset = 0;
float speed = 0, L_setspeed = 0 , R_setspeed = 0;
uint8 stoptime;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器读取函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Get(void)
{
    float dis = 0.0;

    CarInfo.R_encoder =  encoder_get_count(ENCODER_2);      //读取左轮编码器数值
    CarInfo.L_encoder =  -encoder_get_count(ENCODER_1);      //读取右轮编码器数值

//    encoder_clear_count(ENCODER_1);                      //清除左轮编码器数值 (三轮库函数里的函数)
//    encoder_clear_count(ENCODER_2);                      //清除右轮编码器数值

    dis = ((float)(CarInfo.L_encoder + CarInfo.R_encoder)) / (2*ONE_METER);

    CarInfo.distance += dis;                   //平均距离累加

   //斑马线行驶距离
   if(RoadFlag.zebra)       RoadFlag.zebradis += dis;
   else                     RoadFlag.zebradis = 0;

    //十字行走距离
   if(RoadFlag.cross)       RoadFlag.crossDis += dis;
   else                     RoadFlag.crossDis = 0;

    //坡道行走距离
    if(RoadFlag.ramp)       RoadFlag.rampDis += dis;
    else                    RoadFlag.rampDis = 0;

    //圆环行走距离
    if(RoadFlag.circle)     RoadFlag.circleDis += dis;
    else                    RoadFlag.circleDis = 0;

    //避障行走距离
    if(RoadFlag.obstacle)     RoadFlag.obstDis += dis;
    else                      RoadFlag.obstDis = 0;

    //断路行走距离
    if(RoadFlag.breaker)      RoadFlag.breakDis += dis;
    else                      RoadFlag.breakDis = 0;
    //坡道后行走距离
    if(ramptime==1)           RoadFlag.outrampdis += dis;
    else                      RoadFlag.outrampdis = 0;
    
    if(intermtime)            RoadFlag.intermDis += dis;
    else                      RoadFlag.intermDis = 0;
    
    if(RoadFlag.AI)           RoadFlag.AIDis += dis;
    else                      RoadFlag.AIDis = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
    uint16 setspeed;
        //直道高速
        if(RoadFlag.straight)
        {
            speed = DataInfo.image_straightSpeed;
        }
        //急弯速度
        else if(abs(CarInfo.dirOffset)>40 && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)
        {
            speed = DataInfo.image_curSpeed;
        }
        // //出库速度
        // else if(Go_Flag)       
        // {
        //     speed = DataInfo.goSpeed;
        // }
        // //出库后弯道
        // else if(gotime != 0 && RoadFlag.Curve)
        // {
        //     speed = DataInfo.goSpeed + 70;//140 60+60
        // }
        // //坡道速度
        // else if(RoadFlag.ramp) // && RoadFlag.ramp <= 1.6 (DataInfo.door == 0 && ramptime != 0) ||
        // {
        //     speed = DataInfo.rampSpeed;//110
        // }
        // //坡道后速度
        // else if(ramptime==1)// || intermtime != 0
        // {
        //     speed = DataInfo.rampSpeed + 20;
        // }
        //R50速度
        // else if(RoadFlag.circle && Circle.CircleSize==1)
        // {
        //     speed = (uint8)Circle.Speed_R50;
        // }
        //R60速度
        else if(RoadFlag.circle && Circle.CircleSize==2)
        {
            speed = (uint8)Circle.Speed_R60;
        }
        // //R80 R100速度
        // else if(RoadFlag.circle && (Circle.CircleSize==3 || Circle.CircleSize==4))
        // {
        //     speed = (uint8)Circle.Speed_R80;
        // }
        // //避障速度
        // else if(RoadFlag.obstacle)
        // {
        //     speed = DataInfo.obstacleSpeed;
        // }
        // else if(RoadFlag.intermDis >= 5.0)
        //     speed = DataInfo.rampSpeed + 45;
        //停车
        // else if(RoadFlag.zebra == 3)
        //     speed = 0;

        else
        {
          speed = DataInfo.image_traditionSpeed;
        }
    setspeed = speed;//speed
    L_setspeed = setspeed + turn_speed;//speed
    R_setspeed = setspeed - turn_speed;
    L_setspeed = ValueLimitint(L_setspeed, 800, 0);
    R_setspeed = ValueLimitint(R_setspeed, 800, 0);
/******************************速度环的左轮控制*************************************/
/*********************************************************************************/
    // speedL = CarInfo.L_encoder * 100 / 0.005 / L_METER;   // 1m/s，放大一百倍
    speedL = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 1m/s，放大一百倍
    Speed_L =0.4*speedL + 0.6* Speed_L_last;
    Speed_L_last = Speed_L;

    L_offset = L_setspeed - Speed_L;   
    IL_offset += CarInfo.S_LI * L_offset;        //对每次的误差值乘以一个系数，进行积分
    IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);

    DL_offset = L_offset-L_last_offset;
    SL_output = (int16)(CarInfo.S_LP * L_offset + IL_offset + CarInfo.S_LD * DL_offset);   //PID控制
    L_last_offset = L_offset;
    // Speed_L = CarInfo.L_encoder * 100 / 0.005 / L_METER;   // 1m/s，放大一百倍
    // L_offset = L_setspeed - Speed_L;
    // IL_offset += CarInfo.S_LI * L_offset;        //对每次的误差值乘以一个系数，进行积分
    // IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);
    // SL_output = (int16)(CarInfo.S_LP * L_offset + IL_offset);   //PI控制
    // L_last_offset = L_offset;
/******************************速度环的右轮控制*************************************/
/**********************************************************************************/
    // speedR = CarInfo.R_encoder * 100 / 0.005 / R_METER;   // 1m/s，放大一百倍
    speedR = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;
    Speed_R =  0.4 * speedR + 0.6* Speed_R_last; 
    Speed_R_last = Speed_R;

    R_offset = R_setspeed - Speed_R;
    IR_offset += CarInfo.S_RI * R_offset;
    IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);

    DR_offset = R_offset-R_last_offset;
    SR_output = (int16)(CarInfo.S_RP * R_offset + IR_offset + CarInfo.S_RD * DR_offset);
    R_last_offset =R_offset;

    // Speed_R = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;
    // R_offset = R_setspeed - Speed_R;
    // IR_offset += CarInfo.S_RI * R_offset;
    // IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);
    // SR_output = (int16)(CarInfo.S_RP * R_offset + IR_offset);
    // R_last_offset = L_offset;
    //输出限幅
    SL_output = ValueLimitint(SL_output, MOTOR_MAX, MOTOR_MIN);
    SR_output = ValueLimitint(SR_output, MOTOR_MAX, MOTOR_MIN);

}
