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

        // //十字附近识别
        // if(LongLeft > 8 && LongLeft < 23 && LongRight > 8 && LongRight < 23)//22
        // {
        //     for(i = 0; i < COL; i++)
        //     {
        //         if(WhiteCol[i] >= 25)//23
        //             lineSum ++;
        //     }
        // }

        //左右边界长度均大于前瞻行，无需做任何特殊处理
        if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
            CarInfo.dirOffset = L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE;
        else if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //右边丢线向右转
        {
            CarInfo.dirOffset =(L_boundary[25] + R_boundary[25] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4 ;
        }
        else if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //左边丢线拉近向左转
        {
            CarInfo.dirOffset =(L_boundary[25] + R_boundary[25] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4;
        }
#if 1                           //**********考虑一下下面这个地方对十字处理时的影响
        //十字防抖
        // else if(lineSum > 16)
        // {
        //     for(i = 0; i < COL; i ++)
        //     {
        //         if(WhiteCol[i] >= 23)
        //         {
        //             summation += i;
        //             break;
        //         }
        //     }

        //     for(i = COL; i > 0; i --)
        //     {
        //         if(WhiteCol[i - 1] >= 23)
        //         {
        //             summation += i - 1;
        //             break;
        //         }
        //     }

        //     CarInfo.dirOffset = (int16)((summation - COL - 1) * 0.70);//*0.75
        //  }

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
void imageDirectionPID(void)
{
    float diff = 0.0;
    float DirP = 0;//400/90
    float DirD = 0.0;
    imageDirectionControl();

    if(RoadFlag.circle)
    {
        DirP = DataInfo.image_bigP; 
        DirD = DataInfo.image_bigD;
    }
    else if(!RoadFlag.circle&&fabs(CarInfo.dirOffset)<=45&&fabs(CarInfo.dirOffset)>=10)
    {
       DirP =DataInfo.image_bigP;
       DirD =DataInfo.image_bigD;
    }    
    else if(!RoadFlag.circle&&fabs(CarInfo.dirOffset)>45)
    {
        DirP = DataInfo.image_superP;
        DirD = DataInfo.image_superD;
    }
    else
    {
        DirP = DataInfo.image_dirTP2; // 直道
        DirD = DataInfo.image_dirTD2;   
    }

    if(std::fabs(CarInfo.dirOffset)<=45)//    else && std::fabs(CarInfo.dirOffset) > 20 !RoadFlag.circle&&
    {    
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 20, -20);//0,1.6,0.1,-0.1 1.5 2
        direD1 = ChangeLimit(DirD, direD1, 40, -40);// 5
    }
    else if(std::fabs(CarInfo.dirOffset)>45)//!RoadFlag.circle&&
    {
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 40, -40);//0,1.6,0.1,-0.1 1.5 2
        direD1 = ChangeLimit(DirD, direD1, 70, -70);// 5        
    } 

    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);  
    diff = CarInfo.dirOffset - CarInfo.lastDirOffset;
    CarInfo.lastDirOffset = CarInfo.dirOffset;
    diff = ValueLimitfloat(diff, 40, -40);

    CarInfo.expectCurvature = (float)CarInfo.dirOffset * direP1;
    CarInfo.expectCurvature -= (float)diff * direD1;
        
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

    temp_turn_speed = offset * DataInfo.image_dirEP1 - diff_yaw * DataInfo.image_dirED1;
    
    temp_turn_speed = ValueLimitint(temp_turn_speed, 880, -880);
    atomic_turn_speed.store(temp_turn_speed, std::memory_order_relaxed);
}
    // else 
    // {
    //     //正常拐弯
    //     if(LongLeft> DataInfo.forwardLook ) //右拐
    //     {
    //         // Left =((COL - 1 - L_boundary[DataInfo.forwardLook])*0.7)+((COL - 1 - L_boundary[DataInfo.forwardLook-4])*0.3);
    //         // LastLeft = ((COL - 1 - L_boundary[DataInfo.forwardLook])*0.7)+((COL - 1 - L_boundary[DataInfo.forwardLook-4])*0.3);

    //         Left =(COL - 1 - L_boundary[DataInfo.forwardLook - 1]);
    //         LastLeft = (COL - 1 - L_boundary[DataInfo.forwardLook - 1]);
    //         //            BuzzerMs(200);
    //     }
    //     if(LongRight > DataInfo.forwardLook)    //左拐
    //     {
    //         // Right = ((COL - 1 - R_boundary[DataInfo.forwardLook])*0.7) + ((COL - 1 - R_boundary[DataInfo.forwardLook-4])*0.3);
    //         // LastRight = ((COL - 1 - R_boundary[DataInfo.forwardLook])*0.7) + ((COL - 1 - R_boundary[DataInfo.forwardLook-4])*0.3);

    //         Right = (COL - 1 - R_boundary[DataInfo.forwardLook - 1]);
    //         LastRight = (COL - 1 - R_boundary[DataInfo.forwardLook - 1]);
    //         //            BuzzerMs(200);
    //     }
    //     // if( (LongLeft > 35 && LongRight > 35 ) && (LongLeft < DataInfo.forwardLook && LongRight < DataInfo.forwardLook)  && LongLeft <= LongRight )
    //     // {
    //     //     CarInfo.dirOffset =(L_boundary[LongRight - 2] + R_boundary[LongRight - 2] - MIDDLE);    
    //     // }
    //     // else if( (LongLeft > 35 && LongRight > 35 ) && (LongLeft < DataInfo.forwardLook && LongRight < DataInfo.forwardLook)  && LongLeft > LongRight )     
    //     // {
    //     //     CarInfo.dirOffset =(L_boundary[LongLeft - 2] + R_boundary[LongLeft - 2] - MIDDLE);    
    //     // }
    //     // else 
    //     if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //右边丢线向右转
    //     {
    //         CarInfo.dirOffset =(L_boundary[30] + R_boundary[30] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4 ;
    //     }
    //     else if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //左边丢线拉近向左转
    //     {
    //         CarInfo.dirOffset =(L_boundary[30] + R_boundary[30] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4;
    //     }
    //     else if( (LongLeft < DataInfo.forwardLook && LongRight > DataInfo.forwardLook))
    //     {
    //         CarInfo.dirOffset =(L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook -6] - MIDDLE)*0.4 + (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.6;    
    //     }      
    //     else if( (LongLeft > DataInfo.forwardLook && LongRight < DataInfo.forwardLook))
    //     {
    //         CarInfo.dirOffset =(L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook -6] - MIDDLE)*0.4 + (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.6;    
    //     }              
    //     else if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
    //     {
    //         CarInfo.dirOffset =((L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.6 + (L_boundary[DataInfo.forwardLook-6] + R_boundary[DataInfo.forwardLook-6] - MIDDLE)*0.4);           
    //     }
    //     //某一边界小于前瞻行，需要对该边界进行勘误
    //     else
    //     {
    //         if(LongLeft > LongRight)
    //         {
    //             if(fabs(Left - Right) < 45)
    //                 Right = border_max;
    //         }
    //         else if(LongRight > LongLeft)
    //         {
    //             if(fabs(Left - Right) < 45)
    //                 Left = border_min;
    //         }
    //         CarInfo.dirOffset = MIDDLE - Left - Right;
    //     }
    // }
// }
// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      转向PID控制
// //-------------------------------------------------------------------------------------------------------------------
// void imageDirectionPID(void)
// {
//     imageDirectionControl();

//     //一级
//     CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);  //摄像头偏差限幅  
//     diff = CarInfo.dirOffset - CarInfo.lastDirOffset;
//     CarInfo.lastDirOffset =  CarInfo.dirOffset;

//     diff = ValueLimitfloat(diff, 30, -30);              //偏差控制的PD中的D

//     if(RoadFlag.circle && (Circle.CircleSize==3 || Circle.CircleSize==4) )
//         CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR80_EP;
// //    else if(RoadFlag.circle && Circle.CircleSize==4)
// //        CarInfo.expectCurvature = (float)CarInfo.dirOffset*Circle.CircleR100_EP;
//     else
//         {CarInfo.expectCurvature = (float)CarInfo.dirOffset * DataInfo.image_dirEP1;}  //P v

//     if(RoadFlag.circle && Circle.CircleSize==3)
//         CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
//     else if(RoadFlag.circle && Circle.CircleSize==4)
//         CarInfo.expectCurvature -= (float)diff*Circle.CircleR80_ED;
//     else
//         {CarInfo.expectCurvature -= (float)diff * DataInfo.image_dirED1;}// DataInfo.image_dirED
        
//         CarInfo.expectCurvature = ValueLimitfloat(CarInfo.expectCurvature, 100, -100);//100    
//     //二级
//    // Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 /ONE_METER;    //单个编码器要除2  *100(即扩大一百倍) 50=100/2   转化成m/s：0.005/ONE_METER
//     Expect_v = (CarInfo.L_encoder + CarInfo.R_encoder) * 100 / 0.005 / 2 /ONE_METER;
//     Expect_w = Expect_v * (float)(- CarInfo.expectCurvature / 1600.0);
//     //车轴长——两编码器间的距离
//     float current_yaw_rate = atomic_yaw_rate.load(std::memory_order_relaxed);

//     Expect_w = Expect_w * (180 / 3.1415926);
//     offset = Expect_w + current_yaw_rate;

//     float diff_yaw = current_yaw_rate - last_yaw_rate;
//     int16 temp_turn_speed = 0;
//     // if(abs(CarInfo.dirOffset)>=25 && abs(CarInfo.dirOffset)< 45  && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)      //小弯中弯 
//     //     turn_speed = offset * DataInfo.image_bigP + (offset - Expect_w)*DataInfo.image_bigD;
//     // else if(abs(CarInfo.dirOffset) >= 45 && abs(CarInfo.dirOffset) <= 100 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker )     //大弯道  && abs(CarInfo.dirOffset) <=80
//     //     turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     //     // turn_speed = offset * DataInfo.image_bigP + (offset - last_offset)*DataInfo.image_bigD;
//     // else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)      //丢线控制 
//     //     turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else if(Circle.CircleSize==1 && RoadFlag.circle != 0 && RoadFlag.circle%2==0)   //R60右环
//     //     turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else if(Circle.CircleSize==2 && RoadFlag.circle%2==1)                           //R60左环
//     //     turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else                                    //直线
//     //     turn_speed = offset * DataInfo.image_dirTP2 + (offset - Expect_w) * DataInfo.image_dirTD2;

//     // if(abs( CarInfo.dirOffset)< 45  && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)      //小弯中弯 
//     //     temp_turn_speed = offset * DataInfo.image_bigP + (offset - Expect_w)*DataInfo.image_bigD;
//     // else if(abs(CarInfo.dirOffset) >= 60 && abs(CarInfo.dirOffset) <= 90 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker )     //大弯道  && abs(CarInfo.dirOffset) <=80
//     //     temp_turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     //     // turn_speed = offset * DataInfo.image_bigP + (offset - last_offset)*DataInfo.image_bigD;
//     // else if(RoadFlag.blind == 1 || RoadFlag.blind == 2)      //丢线控制 
//     //     temp_turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else if(Circle.CircleSize==1 && RoadFlag.circle != 0 && RoadFlag.circle%2==0)   //R60右环
//     //     temp_turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else if(Circle.CircleSize==2 && RoadFlag.circle%2==1)                           //R60左环
//     //     temp_turn_speed = offset * DataInfo.image_superP + (offset - Expect_w) * DataInfo.image_superD;
//     // else                                    //直线
//         // temp_turn_speed = offset * DataInfo.image_dirTP2 + (offset - Expect_w) * DataInfo.image_dirTD2;
//     if(RoadFlag.straight == 1)  temp_turn_speed = offset * DataInfo.image_dirTP2 - diff_yaw * DataInfo.image_dirTD2;

//     else if(abs(CarInfo.dirOffset)>=30 && abs(CarInfo.dirOffset)< 60  && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)      //小弯中弯 
//         temp_turn_speed = offset * DataInfo.image_bigP + diff_yaw*DataInfo.image_bigD;
//     else if(abs(CarInfo.dirOffset) >= 60 && abs(CarInfo.dirOffset) <= 90 && !RoadFlag.circle && !RoadFlag.obstacle && !RoadFlag.breaker )     //大弯道  && abs(CarInfo.dirOffset) <=80
//         temp_turn_speed = offset * DataInfo.image_superP + diff_yaw * DataInfo.image_superD;
//     else
//         temp_turn_speed = offset * DataInfo.image_bigP - diff_yaw*DataInfo.image_bigD;
    
//     // last_offset=Expect_w;       
//     // last_offset = offset;
//     last_yaw_rate = current_yaw_rate;
//     temp_turn_speed = ValueLimitint(temp_turn_speed, 880, -880);//780
//     atomic_turn_speed.store(temp_turn_speed, std::memory_order_relaxed);
// }



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
