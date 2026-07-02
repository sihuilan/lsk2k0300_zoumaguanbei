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



float Speed_PID_LP[2] = {CarInfo.S_LP,CarInfo.S_LP_Cur};
float Speed_PID_LI[2] = {CarInfo.S_LI,CarInfo.S_LI_Cur};
float Speed_PID_RP[2] = {CarInfo.S_RP,CarInfo.S_RP_Cur};
float Speed_PID_RI[2] = {CarInfo.S_RI,CarInfo.S_RI_Cur};
uint8 PID_chose = 0;

uint8 stoptime;

void PID_Init(void) {
        Speed_PID_LP[0] = CarInfo.S_LP;
        Speed_PID_LP[1] = CarInfo.S_LP_Cur;
        Speed_PID_LI[0] = CarInfo.S_LI;
        Speed_PID_LI[1] = CarInfo.S_LI_Cur;
        Speed_PID_RP[0] = CarInfo.S_RP;
        Speed_PID_RP[1] = CarInfo.S_RP_Cur;
        Speed_PID_RI[0] = CarInfo.S_RI;
        Speed_PID_RI[1] = CarInfo.S_RI_Cur;
        

    }
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
            PID_chose = 0;
        }
        //识别绕行速度
        if(RoadFlag.AI)
        {
            speed = DataInfo.image_curSpeed-30;
            PID_chose = 0;
        }
        //弯道速度
        else if(abs(CarInfo.dirOffset)>=25 &&  abs(CarInfo.dirOffset)<= 90  && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker) //小弯中弯
        {
            speed = DataInfo.image_curSpeed;
            PID_chose = 0;
        }
        // else if(abs(CarInfo.dirOffset) > 80 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker )     //大弯道 &&  abs(CarInfo.dirOffset)<= 80 
        // {
        //     speed = DataInfo.image_curSpeed - 20 ;
        //     PID_chose = 0;            
        // }
        // else if(LongLeft > LongRight && LongLeft <= 50 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)
        // {
        //     speed = DataInfo.image_curSpeed;
        //     PID_chose = 0;
        // }
        // else if(LongRight > LongLeft && LongRight <= 50 && LongRight >= MIN_LINE && LongLeft <= MAX_LINE) 
        // {
        //     speed = DataInfo.image_curSpeed;
        //     PID_chose = 0; 
        // }
        else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)
        {
            speed = DataInfo.image_curSpeed - 30;
            PID_chose = 0;
        }
        //R60速度
        else if(RoadFlag.circle && Circle.CircleSize==2)
        {
            speed = DataInfo.image_curSpeed;
            PID_chose = 0;
        }
        else
        {
          speed = DataInfo.image_traditionSpeed;
          PID_chose = 0;
        }
    setspeed = speed;//speed
    L_setspeed = setspeed + turn_speed/2;//speed
    R_setspeed = setspeed - turn_speed/2;
    // L_setspeed = DataInfo.image_traditionSpeed;//speed
    // R_setspeed = DataInfo.image_traditionSpeed;
    L_setspeed = ValueLimitint(L_setspeed, 700, 0);
    R_setspeed = ValueLimitint(R_setspeed, 700, 0);
/******************************速度环的左轮控制*************************************/
/*********************************************************************************/
    speedL = CarInfo.L_encoder * 100 / 0.005 / ONE_METER;   // 1m/s，放大一百倍
    Speed_L =0.85 * speedL + 0.15* Speed_L_last;
    Speed_L_last = Speed_L;

    L_offset = L_setspeed - Speed_L;   
    IL_offset += CarInfo.S_LI * L_offset;        //对每次的误差值乘以一个系数，进行积分
    IL_offset = ValueLimitint(IL_offset, IMAX, IMIN);

    DL_offset = L_offset - L_last_offset;
    L_last_offset = L_offset;
    SL_output = (int16)(CarInfo.S_LP * L_offset + IL_offset + DL_offset * CarInfo.S_LD);   //PID控制
/******************************速度环的右轮控制*************************************/
/**********************************************************************************/
    speedR = CarInfo.R_encoder * 100 / 0.005 / ONE_METER;
    Speed_R =  0.85 * speedR + 0.15 * Speed_R_last; 
    Speed_R_last = Speed_R;

    R_offset = R_setspeed - Speed_R;
    IR_offset += CarInfo.S_RI * R_offset;
    IR_offset = ValueLimitint(IR_offset, IMAX, IMIN);

    DR_offset = R_offset-R_last_offset;
    R_last_offset =R_offset;  
    SR_output = (int16)(CarInfo.S_RP * R_offset + IR_offset + DR_offset * CarInfo.S_RD);

    //输出限幅
    SL_output = ValueLimitint(SL_output, MOTOR_MAX, MOTOR_MIN);
    SR_output = ValueLimitint(SR_output, MOTOR_MAX, MOTOR_MIN);

}
