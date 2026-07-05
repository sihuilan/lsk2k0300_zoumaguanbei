#include "zf_common_headfile.h"

int16 diff = 0;
float offset, last_offset, Expect_v, Expect_w;
float OutCurvature = 0.0;
float ExpectCurvature;
int16 turn_PWM=0 , turn_speed = 0;
float direP1 = 0.0, direD1 = 0.0; //转向PD
int16 choice = 0;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      转向控制
//-------------------------------------------------------------------------------------------------------------------
static void imageDirectionControl(void)
{
    // AI识别
    if(RoadFlag.AI)
   {
    AIControl();
    return;
   } 
    // //丢线右转
    else if(RoadFlag.blind == 1)
    {
        CarInfo.dirOffset = -OFFSET_MAX;
        // Right = COL - 1;        //归满
        // LastRight = COL - 1;    //归满
        // Left = OFFSET_MAX;
        // LastLeft = OFFSET_MAX;
        // CarInfo.dirOffset =(L_boundary[10] + R_boundary[10] - MIDDLE)*0.5+ (L_boundary[18] + R_boundary[18] - MIDDLE)*0.5;
        // if(CarInfo.dirOffset < 60)     CarInfo.dirOffset = 60;
    }
    //丢线左转
    else if(RoadFlag.blind == 2)
    {
        // CarInfo.dirOffset =(L_boundary[10] + R_boundary[10] - MIDDLE)*0.5+ (L_boundary[18] + R_boundary[18] - MIDDLE)*0.5;
        // if(CarInfo.dirOffset < 60)     CarInfo.dirOffset = 60; 
        CarInfo.dirOffset = OFFSET_MAX;
        // Left = 0;               //归零
        // LastLeft = 0;           //归零
        // Right = COL - OFFSET_MAX;
        // LastRight = COL - OFFSET_MAX;
    }
    //十字控制
    else if(RoadFlag.cross)
        CrossControl();
        // cross_fill(Bmp,L_boundary, R_boundary, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);

    // //坡道控制
    // else if(RoadFlag.ramp)
    //     RampControl();

    //圆环控制
    else if(RoadFlag.circle)
        Circle_Control();

    // //斑马线控制
    // else if(RoadFlag.zebra)
    //     ZebraControl();
  
    // //避障控制
    // else if(RoadFlag.obstacle)
    //     Obstacle_Control();
    else 
    {
        //正常拐弯
        if(LongLeft> DataInfo.forwardLook ) //右拐
        {
            // Left =((COL - 1 - L_boundary[DataInfo.forwardLook])*0.7)+((COL - 1 - L_boundary[DataInfo.forwardLook-4])*0.3);
            // LastLeft = ((COL - 1 - L_boundary[DataInfo.forwardLook])*0.7)+((COL - 1 - L_boundary[DataInfo.forwardLook-4])*0.3);

            Left =(COL - 1 - L_boundary[DataInfo.forwardLook - 1]);
            LastLeft = (COL - 1 - L_boundary[DataInfo.forwardLook - 1]);
            //            BuzzerMs(200);
        }
        if(LongRight > DataInfo.forwardLook)    //左拐
        {
            // Right = ((COL - 1 - R_boundary[DataInfo.forwardLook])*0.7) + ((COL - 1 - R_boundary[DataInfo.forwardLook-4])*0.3);
            // LastRight = ((COL - 1 - R_boundary[DataInfo.forwardLook])*0.7) + ((COL - 1 - R_boundary[DataInfo.forwardLook-4])*0.3);

            Right = (COL - 1 - R_boundary[DataInfo.forwardLook - 1]);
            LastRight = (COL - 1 - R_boundary[DataInfo.forwardLook - 1]);
            //            BuzzerMs(200);
        }
        if( (LongLeft < DataInfo.forwardLook && LongRight < DataInfo.forwardLook)  && LongLeft <= LongRight )
        {
            CarInfo.dirOffset =(L_boundary[LongRight - 2] + R_boundary[LongRight - 2] - MIDDLE);    
        }
        else if( (LongLeft < DataInfo.forwardLook && LongRight < DataInfo.forwardLook)  && LongLeft > LongRight )
        {
            CarInfo.dirOffset =(L_boundary[LongLeft - 2] + R_boundary[LongLeft - 2] - MIDDLE);    
        }
        else if( (LongLeft < DataInfo.forwardLook && LongRight > DataInfo.forwardLook))
        {
            CarInfo.dirOffset =(L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook -6] - MIDDLE)*0.4 + (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.6;    
        }      
        else if( (LongLeft > DataInfo.forwardLook && LongRight < DataInfo.forwardLook))
        {
            CarInfo.dirOffset =(L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook -6] - MIDDLE)*0.4 + (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.6;    
        }       
        else if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
        {
        CarInfo.dirOffset =((L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.7 + (L_boundary[DataInfo.forwardLook-6] + R_boundary[DataInfo.forwardLook-6] - MIDDLE)*0.3);           
        }
        //某一边界小于前瞻行，需要对该边界进行勘误
        else
        {
            if(LongLeft > LongRight)
            {
                if(fabs(Left - Right) < 45)
                    Right = border_max;
            }
            else if(LongRight > LongLeft)
            {
                if(fabs(Left - Right) < 45)
                    Left = border_min;
            }
            CarInfo.dirOffset = MIDDLE - Left - Right;
        }
    }
}//            CarInfo.dirOffset = L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE;
//-------------------------------------------------------------------------------------------------------------------
//  @brief      转向PID控制
//-------------------------------------------------------------------------------------------------------------------
void imageDirectionPID(void)
{
    imageDirectionControl();

    //一级
    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);  //摄像头偏差限幅  
    diff = CarInfo.dirOffset - CarInfo.lastDirOffset;
    CarInfo.lastDirOffset =  CarInfo.dirOffset;

    diff = ValueLimitfloat(diff, 30, -30);              //偏差控制的PD中的D

    if(RoadFlag.circle && (Circle.CircleSize==3 || Circle.CircleSize==4) )
        CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR80_EP;
//    else if(RoadFlag.circle && Circle.CircleSize==4)
//        CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR100_EP;
    else
        {CarInfo.expectCurvature = (float)CarInfo.dirOffset * DataInfo.image_dirEP1;}  //P v

    if(RoadFlag.circle && Circle.CircleSize==3)
        CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
    else if(RoadFlag.circle && Circle.CircleSize==4)
        CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
    else
        {CarInfo.expectCurvature -= (float)diff * DataInfo.image_dirED1;}// DataInfo.image_dirED
        
        CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 75, -75);//100    
    //二级
   // Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 /ONE_METER;    //单个编码器要除2  *100(即扩大一百倍) 50=100/2   转化成m/s：0.005/ONE_METER
    Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 / 2 /ONE_METER;
    Expect_w = Expect_v * (float)(- CarInfo.expectCurvature / 1700.0);
    //车轴长——两编码器间的距离

    Expect_w = Expect_w * (180 / 3.1415926);
   
    offset = Expect_w + CarInfo.YawRate;
    if(abs(CarInfo.dirOffset)>=25 && abs(CarInfo.dirOffset)< 45  && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)      //小弯中弯 
        turn_speed = offset * DataInfo.image_bigP + (offset - last_offset)*DataInfo.image_bigD;
    else if(abs(CarInfo.dirOffset) >= 45 && abs(CarInfo.dirOffset) <= 90 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker )     //大弯道  && abs(CarInfo.dirOffset) <=80
        turn_speed = offset * DataInfo.image_superP + (offset - last_offset) * DataInfo.image_superD;
        // turn_speed = offset * DataInfo.image_bigP + (offset - last_offset)*DataInfo.image_bigD;
    else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)      //丢线控制 
        turn_speed = offset * DataInfo.image_superP + (offset - last_offset) * DataInfo.image_superD;
    else if(Circle.CircleSize==1 && RoadFlag.circle != 0 && RoadFlag.circle%2==0)   //R60右环
        turn_speed = offset * DataInfo.image_superP + (offset - last_offset) * DataInfo.image_superD;
    else if(Circle.CircleSize==2 && RoadFlag.circle%2==1)                           //R60左环
        turn_speed = offset * DataInfo.image_superP + (offset - last_offset) * DataInfo.image_superD;
    else                                    //直线
        turn_speed = offset * DataInfo.image_dirTP2 + (offset - last_offset) * DataInfo.image_dirTD2;

    // last_offset=Expect_w;       
    last_offset = offset;
    turn_speed = ValueLimitint(turn_speed, 880, -880);//780


}


