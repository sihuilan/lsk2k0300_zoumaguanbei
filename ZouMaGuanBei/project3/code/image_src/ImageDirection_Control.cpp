#include "zf_common_headfile.h"
int16 diff = 0;
float offset, last_offset, Expect_v, Expect_w, last_yaw_rate;
float OutCurvature = 0.0;
float ExpectCurvature;
int16 turn_PWM=0 , turn_speed = 0;
float direP1 = 0.0, direD1 = 0.0, direP2 = 0.0, direD2=0.0,direI1=0.0; //转向PD
int16 choice = 0;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      转向控制
//-------------------------------------------------------------------------------------------------------------------
static void imageDirectionControl(void)
{
    // AI识别
    if(RoadFlag.AI)
    AIControl();
    // //丢线右转
    else if(RoadFlag.blind == 1)
    {
        CarInfo.dirOffset = -100;
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
        CarInfo.dirOffset = 100;
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
        if(LongLeft > DataInfo.forwardLook) //右拐
        {
            Left = COL - 1 - L_boundary[DataInfo.forwardLook];
            LastLeft = COL -1 - L_boundary[DataInfo.forwardLook];
        }
        if(LongRight > DataInfo.forwardLook)    //左拐
        {
            Right = COL - 1 - R_boundary[DataInfo.forwardLook];
            LastRight = COL - 1 - R_boundary[DataInfo.forwardLook];
        }
        if(LongLeft > 85 && LongRight > 85)
        {
            CarInfo.dirOffset =(L_boundary[70] + R_boundary[70] - MIDDLE)*0.3+ (L_boundary[60] + R_boundary[60] - MIDDLE)*0.7 + (L_boundary[24] + R_boundary[24] - MIDDLE)*0;  //现在使用的两行前瞻为42和36
        }

        //左右边界长度均大于前瞻行，无需做任何特殊处理
        else if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
            CarInfo.dirOffset = (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.7 + (L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook - 6] - MIDDLE)*0.3;
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
}
void imageDirectionPID(void)
{
    float diff = 0.0;
    float DirP = 0;//400/90
    float DirD = 0.0;
    imageDirectionControl();

    if(RoadFlag.inside_CrossRound == 1)
    {
        // if(fabs(CarInfo.dirOffset)>50)
        // {
            DirP = DataInfo.Ring_CurP; 
            DirD = DataInfo.Ring_CurD;
        // }
        // else
        // {
        //     DirP = DataInfo.image_bigP;
        //     DirD = DataInfo.image_bigD;            
        // }
    }
    else if(RoadFlag.circle)
    {
        // if(fabs(CarInfo.dirOffset)>50)
        // {
            DirP = DataInfo.AI_CurP_L; 
            DirD = DataInfo.AI_CurP_R;
        // }
        // else
        // {
        //     DirP = DataInfo.image_bigP;
        //     DirD = DataInfo.image_bigD;            
        // }
    }
    // else 
    else if(!RoadFlag.circle && fabs(CarInfo.dirOffset)<=75 && fabs(CarInfo.dirOffset)>=20)
    {
       DirP = DataInfo.image_bigP;
       DirD = DataInfo.image_bigD;
    }    
    else if(!RoadFlag.circle && fabs(CarInfo.dirOffset)>70)
    {
        DirP = DataInfo.image_superP;
        DirD = DataInfo.image_superD;
    }
    else
    {
        DirP = DataInfo.image_Str_P; // 直道
        DirD = DataInfo.image_Str_D;   
    }

    if(std::fabs(CarInfo.dirOffset)<=70 && !RoadFlag.AI)
    {    
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 10, -10);//0,1.6,0.1,-0.1 1.5 2&& RoadFlag.inside_CrossRound == 0
        direD1 = ChangeLimit(DirD, direD1, 45, -45);// 5
    }
    else if(std::fabs(CarInfo.dirOffset)>70 && !RoadFlag.AI )
    {
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 45, -45);//0,1.6,0.1,-0.1 1.5 2&& RoadFlag.inside_CrossRound == 0
        direD1 = ChangeLimit(DirD, direD1, 80, -80);// 5        
    }
      else if(RoadFlag.AI)
   {
    if (RoadFlag.AI == 1 || RoadFlag.AI == 3) {
        // 左转绕行：用更大的 P
        direP1 = DataInfo.AI_CurP_L;//2.2f;      
        direD1 = DataInfo.AI_CurP_L;//0.0f;
    } else {
        // 右转绕行：用原来的参数
        direP1 = 1.76;   // ← 1.7
        direD1 = 0;
    }
  } 
    // else
    // {
        // direP1 = DirP;
        // direD1 = DirD;
    // } 

    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);  
    diff = CarInfo.dirOffset - CarInfo.lastDirOffset;
    CarInfo.lastDirOffset = CarInfo.dirOffset;
    diff = ValueLimitfloat(diff, 40, -40);

    CarInfo.expectCurvature = (float)CarInfo.dirOffset * direP1;
    CarInfo.expectCurvature += (float)diff * direD1;
        
    // CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 100, -100);    

    // ==========================================
    // 运动学转换（中间桥梁）
    // ==========================================
    Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 / 2 / ONE_METER;
    Expect_w = Expect_v * (float)(- CarInfo.expectCurvature / 1600.0);
    
    float current_yaw_rate = atomic_yaw_rate.load(std::memory_order_relaxed);
    Expect_w = Expect_w * (180 / 3.1415926);
    offset = Expect_w + current_yaw_rate; 

    float diff_yaw = current_yaw_rate - last_yaw_rate;
    last_yaw_rate = current_yaw_rate;

    // ==========================================
    // 二级：角速度环（内环）
    // ==========================================
    int16 temp_turn_speed = 0;

    temp_turn_speed = offset * DataInfo.image_dirEP - diff_yaw * DataInfo.image_dirED;
    
    temp_turn_speed = ValueLimitint(temp_turn_speed, 880, -880);
    atomic_turn_speed.store(temp_turn_speed, std::memory_order_relaxed);
}
 
//-------------------------------------------------------------------------------------------------------------------
//  @brief      期望曲率
// //-------------------------------------------------------------------------------------------------------------------
// static float FitCurvature(float dirOffset)
// {
//   float ExpectCurvature[8] = {2.76, 5.7, 8.34, 12.12,14.2,16.8,19.2,22.22}; //期望曲率
//   float Err[8] =             {5,    10,    15,    21,  27,  33,  39,   45}; //偏差

//   //相邻两个期望曲率和相邻两个偏差之间对应的值就是通过线性插值求得
//   //线性插值  它是根据一维数据序列中需要插值的点的左右临近两个数据来进行数值估计
//   //则这里可以理解为 期望曲率 = 偏差*动态P + 偏差的微分*D

//   uint8 i;
//   float Prol;
//   float OutCurvature = 0;
//   float *p;
//   float ABSOffset = fabs(dirOffset);

//   p = ExpectCurvature;

//   //找出偏差所在的区域
//   for(i=0;i<8;i++)
//   {
//       if(ABSOffset<=Err[i])     break;
//   }
//   //根据找到的区间算出曲率
//   //在第0区间，P是固定的
//   if(i==0)
//   {
//       Prol = p[0]/Err[0];
//       OutCurvature = dirOffset * Prol;
//   }
//   //在第8区间，当偏差>47后都是曲率的最大值
//   else if(i==8)
//   {
//       if(dirOffset > 0)
//           OutCurvature  = p[7];
//       else if(dirOffset < 0)
//           OutCurvature = -p[7];
//   }
//   //其他区间需要通过线性插值来计算出期望曲率
//   else
//   {
//       //求出斜率K
//     Prol = (ABSOffset - Err[i-1])/(Err[i]-Err[i-1]);
//     if(dirOffset>0)
//       OutCurvature = (p[i-1] + (p[i]-p[i-1])*Prol);
//     else if(dirOffset<0)
//       OutCurvature = -(p[i-1] + (p[i]-p[i-1])*Prol);
//     else
//       OutCurvature = 0;
//   }

//   return OutCurvature;
// }

// #if 0
// void imageDirectionPID(void)
// {
//     float diff = 0.0, P = 0.0, D = 0.0;
//     float Expect_v = 0.0, Expect_w = 0.0;
//     float offset = 0.0, last_offset = 0.0;
//     imageDirectionControl();

//     if(Go_Flag || RoadFlag.zebra)
//     {
//         P = 12;
//         D = 0;
//     }
//     //圆环
//     if(RoadFlag.circle)
//     {
//         P = Circle.CircleR60_P;
//         D = Circle.CircleR60_D;
//     }
//     //坡道
//     else if(RoadFlag.ramp)
//     {
//         P = DataInfo.rampP;
//         D = 0;
//     }
//     else
//     {
//         P = DataInfo.image_dirEP;
//         D = DataInfo.image_dirED;
//     }

//     //增幅限幅，防止PD变化过大引起抖动
//     direP1 = ChangeLimit(P, direP1, 0.1, -0.1);
//     direD1 = ChangeLimit(D, direD1, 0.1, -0.1);

//     diff = CarInfo.dirOffset - CarInfo.lastDirOffset;

//     CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
//     CarInfo.expectCurvature = (float)direP1 * CarInfo.dirOffset;

//     diff = ValueLimitfloat(diff, 30, -30);

//     CarInfo.expectCurvature -= (float)diff * direD1;

// //    CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 60, -60);
//     CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 650, -650);
//     CarInfo.lastDirOffset = CarInfo.dirOffset;

// }
// #endif
