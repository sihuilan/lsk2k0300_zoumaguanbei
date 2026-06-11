#include "zf_common_headfile.h"

int16 diff = 0;
float offset, last_offset, Expect_v, Expect_w;
float OutCurvature = 0.0;
float ExpectCurvature;
int16 turn_PWM;
float direP1 = 0.0, direD1 = 0.0; //转向PD

//-------------------------------------------------------------------------------------------------------------------
//  @brief      转向控制
//-------------------------------------------------------------------------------------------------------------------
static void imageDirectionControl(void)
{
    uint8 i;
    uint8 lineSum = 0;
    uint8 summation = 0;

    //丢线右转
    if(RoadFlag.blind == 1)
    {
        CarInfo.dirOffset = -OFFSET_MAX;
        Right = COL - 1;        //归满
        LastRight = COL - 1;    //归满
        Left = OFFSET_MAX;
        LastLeft = OFFSET_MAX;
    }
    //丢线左转
    else if(RoadFlag.blind == 2)
    {
        CarInfo.dirOffset = OFFSET_MAX;
        Left = 0;               //归零
        LastLeft = 0;           //归零
        Right = COL - OFFSET_MAX;
        LastRight = COL - OFFSET_MAX;
    }

    //其他右转
    else if(RoadFlag.turn == 1)
    {
        CarInfo.dirOffset = -65;
        Right = COL - 1;        //归满
        LastRight = COL - 1;    //归满
        Left = OFFSET_MAX;
        LastLeft = OFFSET_MAX;
    }
    //其他左转
    else if(RoadFlag.turn == 2)
    {
        CarInfo.dirOffset = 65;
        Left = 0;               //归零
        LastLeft = 0;           //归零
        Right = COL - OFFSET_MAX;
        LastRight = COL - OFFSET_MAX;
    }

    //十字控制
    else if(RoadFlag.cross)
        CrossControl();

    //坡道控制
    else if(RoadFlag.ramp)
        RampControl();

    //圆环控制
    else if(RoadFlag.circle)
        Circle_Control();

    //斑马线控制
    else if(RoadFlag.zebra)
        ZebraControl();

    
    //避障控制
    else if(RoadFlag.obstacle)
        Obstacle_Control();

    else
    {
        //正常拐弯
        if(LongLeft > DataInfo.forwardLook) //右拐
        {
            Left = COL - 1 - LeftBlack[DataInfo.forwardLook];
            LastLeft = COL -1 - LeftBlack[DataInfo.forwardLook];
        }
        if(LongRight > DataInfo.forwardLook)    //左拐
        {
            Right = COL - 1 - RightBlack[DataInfo.forwardLook];
            LastRight = COL - 1 - RightBlack[DataInfo.forwardLook];
        }

        //十字附近识别
        if(LongLeft > 8 && LongLeft < 23 && LongRight > 8 && LongRight < 23)//22
        {
            for(i = 0; i < COL; i++)
            {
                if(WhiteCol[i] >= 25)//23
                    lineSum ++;
            }
        }

        //左右边界长度均大于前瞻行，无需做任何特殊处理
        if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
            CarInfo.dirOffset = LeftBlack[DataInfo.forwardLook] + RightBlack[DataInfo.forwardLook] - MIDDLE;
#if 1
        //十字防抖
        else if(lineSum > 16)
        {
            for(i = 0; i < COL; i ++)
            {
                if(WhiteCol[i] >= 23)
                {
                    summation += i;
                    break;
                }
            }

            for(i = COL; i > 0; i --)
            {
                if(WhiteCol[i - 1] >= 23)
                {
                    summation += i - 1;
                    break;
                }
            }

            CarInfo.dirOffset = (int16)((summation - COL - 1) * 0.70);//*0.75
         }

#endif
        //某一边界小于前瞻行，需要对该边界进行勘误
        else
        {
            if(LongLeft > LongRight)
            {
                if(fabs(Left - Right) < 45)
                    Right = COL - 1;
            }
            else if(LongRight > LongLeft)
            {
                if(fabs(Left - Right) < 45)
                    Left = 0;
            }
            CarInfo.dirOffset = MIDDLE - Left - Right;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      期望曲率
//-------------------------------------------------------------------------------------------------------------------
static float FitCurvature(float dirOffset)
{
  float ExpectCurvature[8] = {2.76, 5.7, 8.34, 12.12,14.2,16.8,19.2,22.22}; //期望曲率
  float Err[8] =             {5,    10,    15,    21,  27,  33,  39,   45}; //偏差

  //相邻两个期望曲率和相邻两个偏差之间对应的值就是通过线性插值求得
  //线性插值  它是根据一维数据序列中需要插值的点的左右临近两个数据来进行数值估计
  //则这里可以理解为 期望曲率 = 偏差*动态P + 偏差的微分*D

  uint8 i;
  float Prol;
  float OutCurvature = 0;
  float *p;
  float ABSOffset = fabs(dirOffset);

  p = ExpectCurvature;

  //找出偏差所在的区域
  for(i=0;i<8;i++)
  {
      if(ABSOffset<=Err[i])     break;
  }
  //根据找到的区间算出曲率
  //在第0区间，P是固定的
  if(i==0)
  {
      Prol = p[0]/Err[0];
      OutCurvature = dirOffset * Prol;
  }
  //在第8区间，当偏差>47后都是曲率的最大值
  else if(i==8)
  {
      if(dirOffset > 0)
          OutCurvature  = p[7];
      else if(dirOffset < 0)
          OutCurvature = -p[7];
  }
  //其他区间需要通过线性插值来计算出期望曲率
  else
  {
      //求出斜率K
    Prol = (ABSOffset - Err[i-1])/(Err[i]-Err[i-1]);
    if(dirOffset>0)
      OutCurvature = (p[i-1] + (p[i]-p[i-1])*Prol);
    else if(dirOffset<0)
      OutCurvature = -(p[i-1] + (p[i]-p[i-1])*Prol);
    else
      OutCurvature = 0;
  }

  return OutCurvature;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      转向PID控制
//-------------------------------------------------------------------------------------------------------------------
void imageDirectionPID(void)
{
    imageDirectionControl();
    //一级
    diff = CarInfo.dirOffset - CarInfo.lastDirOffset;
    CarInfo.lastDirOffset =  CarInfo.dirOffset;
    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);

    if(RoadFlag.circle && (Circle.CircleSize==3 || Circle.CircleSize==4) )
        CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR80_EP;
//    else if(RoadFlag.circle && Circle.CircleSize==4)
//        CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR100_EP;
    else
        CarInfo.expectCurvature = (float)CarInfo.dirOffset * DataInfo.image_dirEP;;  //P v

        diff = ValueLimitfloat(diff, 30, -30);

    if(RoadFlag.circle && Circle.CircleSize==3)
        CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
    else if(RoadFlag.circle && Circle.CircleSize==4)
        CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
    else
    {
        CarInfo.expectCurvature -= (float)diff * 0;// DataInfo.image_dirED

        CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 45, -45);//100


    }
    
    //二级
    Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 / ONE_METER;    //单个编码器要除2  *100(即扩大一百倍) 50=100/2   转化成m/s：0.005/ONE_METER
    //Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / ONE_METER;    //单个编码器要除2  *100(即扩大一百倍) 50=100/2   转化成m/s：0.005/ONE_METER
    Expect_w = Expect_v * (float)(- CarInfo.expectCurvature / 800.0);
    //车轴长——两编码器间的距离
    Expect_w = Expect_w * (180 / 3.1415926);
    offset = Expect_w + CarInfo.YawRate;


    if(RoadFlag.Curve)                 //出库后转弯
        turn_PWM = offset * DataInfo.CurveP + (Expect_w-last_offset) * 0;
    else if(ramptime==1)            //坡道后
        turn_PWM = offset * 0.6;
    else if(RoadFlag.zebra == 2)    //入库
        turn_PWM = offset * 7.0 + (Expect_w-last_offset) * 0;

    else if(Circle.CircleSize==1 && RoadFlag.circle%2==1)                           //R50左环
        turn_PWM = offset * Circle.CircleR50_LP + (Expect_w-last_offset)*Circle.CircleR50_LD;
    else if(Circle.CircleSize==1 && RoadFlag.circle != 0 && RoadFlag.circle%2==0)   //R50右环
        turn_PWM = offset * Circle.CircleR50_RP + (Expect_w-last_offset)*Circle.CircleR50_RD;

    else if(Circle.CircleSize==2 && RoadFlag.circle%2==1)                           //R60左环
        turn_PWM = offset * Circle.CircleR60_LP + (Expect_w-last_offset)*Circle.CircleR60_D;
    else if(Circle.CircleSize==1 && RoadFlag.circle != 0 && RoadFlag.circle%2==0)   //R60右环
        turn_PWM = offset * Circle.CircleR60_RP + (Expect_w-last_offset)*Circle.CircleR60_D;

    else if(RoadFlag.circle&& (Circle.CircleSize==3 || Circle.CircleSize==4))      //R80 R100
        turn_PWM = offset * Circle.CircleR80_P + (Expect_w-last_offset)*Circle.CircleR80_D;

//    else if(RoadFlag.circle&&Circle.CircleSize==4)      //R100
//        turn_PWM = offset * Circle.CircleR100_P + (Expect_w-last_offset)*Circle.CircleR100_D;

    else if(RoadFlag.obstacle)       //避障
        turn_PWM = offset * DataInfo.obstacleP + (Expect_w-last_offset) * 0;

    else if(CarInfo.dirOffset >= 0 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker)     //普通左转弯
        turn_PWM = offset * DataInfo.image_dirTLP + (Expect_w-last_offset) * DataInfo.image_dirTD;     //1 0
    else                                                                                               //普通右转弯
        turn_PWM = offset * DataInfo.image_dirTRP + (Expect_w-last_offset) * DataInfo.image_dirTD;     //1 0

    //K_d \times (offset - last\_offset\_real)

    last_offset=Expect_w;

    // turn_PWM = ValueLimitint(turn_PWM, 800, -800);//780
    turn_PWM = ValueLimitint(turn_PWM, 400, -400);//780

}

#if 0
void imageDirectionPID(void)
{
    float diff = 0.0, P = 0.0, D = 0.0;
    float Expect_v = 0.0, Expect_w = 0.0;
    float offset = 0.0, last_offset = 0.0;
    imageDirectionControl();

    if(Go_Flag || RoadFlag.zebra)
    {
        P = 12;
        D = 0;
    }
    //圆环
    if(RoadFlag.circle)
    {
        P = Circle.CircleR60_P;
        D = Circle.CircleR60_D;
    }
    //坡道
    else if(RoadFlag.ramp)
    {
        P = DataInfo.rampP;
        D = 0;
    }
    else
    {
        P = DataInfo.image_dirEP;
        D = DataInfo.image_dirED;
    }

    //增幅限幅，防止PD变化过大引起抖动
    direP1 = ChangeLimit(P, direP1, 0.1, -0.1);
    direD1 = ChangeLimit(D, direD1, 0.1, -0.1);

    diff = CarInfo.dirOffset - CarInfo.lastDirOffset;

    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
    CarInfo.expectCurvature = (float)direP1 * CarInfo.dirOffset;

    diff = ValueLimitfloat(diff, 30, -30);

    CarInfo.expectCurvature -= (float)diff * direD1;

//    CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 60, -60);
    CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 650, -650);
    CarInfo.lastDirOffset = CarInfo.dirOffset;

}
#endif
