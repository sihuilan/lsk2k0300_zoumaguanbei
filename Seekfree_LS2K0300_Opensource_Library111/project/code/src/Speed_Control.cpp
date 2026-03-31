#include "zf_common_headfile.h"


#define ENCODER_1           "/dev/zf_encoder_1"
#define ENCODER_2           "/dev/zf_encoder_2"

float Speed_L,Speed_R;
int16 SL_output = 0, SR_output = 0;
int16 L_offset = 0, R_offset = 0, IL_offset = 0, IR_offset = 0;
int16 L_last_offset = 0, R_last_offset = 0;

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

    dis = ((float)(CarInfo.L_encoder + CarInfo.R_encoder)) / ONE_METER;

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
    //电磁
//    if(ADDir.Ramp)
//        ADDir.Rampdis += dis;
//    else
//        ADDir.Rampdis = 0;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      速度控制函数
//-------------------------------------------------------------------------------------------------------------------
void Speed_Control(void)
{
    float speed = 0, setspeed = 30;

    if(DataInfo.MODE == 0)  //摄像头模式
    {
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
        //出库速度
        else if(Go_Flag)
        {
            speed = DataInfo.goSpeed;
        }
        //出库后弯道
        else if(gotime != 0 && RoadFlag.Curve)
        {
            speed = DataInfo.goSpeed + 70;//140 60+60
        }
        //坡道速度
        else if(RoadFlag.ramp) // && RoadFlag.ramp <= 1.6 (DataInfo.door == 0 && ramptime != 0) ||
        {
            speed = DataInfo.rampSpeed;//110
        }
        //坡道后速度
        else if(ramptime==1)// || intermtime != 0
        {
            speed = DataInfo.rampSpeed + 20;
        }
        //R50速度
        else if(RoadFlag.circle && Circle.CircleSize==1)
        {
            speed = (uint8)Circle.Speed_R50;
        }
        //R60速度
        else if(RoadFlag.circle && Circle.CircleSize==2)
        {
            speed = (uint8)Circle.Speed_R60;
        }
        //R80 R100速度
        else if(RoadFlag.circle && (Circle.CircleSize==3 || Circle.CircleSize==4))
        {
            speed = (uint8)Circle.Speed_R80;
        }
        //避障速度
        else if(RoadFlag.obstacle)
        {
            speed = DataInfo.obstacleSpeed;
        }
        else if(RoadFlag.intermDis >= 5.0)
            speed = DataInfo.rampSpeed + 45;
        //入库速度
        else if(ZebraTime == 2 && RoadFlag.zebra != 3)//ZebraTime == 2 &&
        {
            speed = DataInfo.zebraSpeed;
        }
        //停车
        else if(RoadFlag.zebra == 3)
            speed = 0;

        /*else
        {
            if(dl1a_distance_mm <= 1000)    speed = DataInfo.obstacleSpeed;
            else
                speed = DataInfo.image_traditionSpeed;
        }
        */

//        if((CarInfo.L_encoder <= 0 || CarInfo.R_encoder <= 0) && CarInfo.distance > 1.0 && RoadFlag.zebra != 3)
//        {
//            stoptime++;
//            if(stoptime > 10)
//                RoadFlag.protect = 1;
//        }
//        else
//        {
//            stoptime = 0;
//            setspeed = speed;
//        }
    }

    if(DataInfo.MODE == 1)  //电磁模式
    {
//        if(ADInfo.straight)
//            speed = DataInfo.ad_straightSpeed;
//        if(ADCircle.flag)
//        speed = ADCircle.Speed60;

//        if(ADInfo.distance >= 20)
//            RoadFlag.protect = 1;
//        else if(ADInfo.ADC[0]<5 && ADInfo.ADC[1]<5 && ADInfo.ADC[2]<5 && DataInfo.MODE == 1)
//            RoadFlag.protect = 1;
//        else
//            speed = ADDir.ad_traditionSpeed;
    }

    //setspeed = speed;//speed

    Speed_L = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 1m/s，放大一百倍
    L_offset = setspeed - Speed_L;
    IL_offset += S_IL * L_offset;        //对每次的误差值乘以一个系数，进行积分
    IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);
    SL_output = (int16)(S_PL * L_offset + IL_offset);   //PI控制
    L_last_offset = L_offset;

    Speed_R = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;
    R_offset = setspeed - Speed_R;
    IR_offset += S_IR * R_offset;
    IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);
    SR_output = (int16)(S_PR * R_offset + IR_offset);
    R_last_offset = L_offset;

    //输出限幅
    SL_output = ValueLimitint(SL_output, MOTOR_MAX, MOTOR_MIN);
    SR_output = ValueLimitint(SR_output, MOTOR_MAX, MOTOR_MIN);

//    //左
//    L_offset = setspeed - CarInfo.L_encoder;
//    IL_offset += S_I * L_offset;
//    IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);
//
//    SL_output = (int16)(S_P * L_offset + IL_offset);
//    L_last_offset = L_offset;
//
//    //右
//    R_offset = setspeed - CarInfo.R_encoder;
//    IR_offset += S_I * R_offset;
//    IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);
//
//    SR_output = (int16)(S_P * R_offset + IR_offset);
//    R_last_offset = R_offset;

}
