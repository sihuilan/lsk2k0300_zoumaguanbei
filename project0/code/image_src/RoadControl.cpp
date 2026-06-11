#include "zf_common_headfile.h"

std::atomic<bool> Go_Flag(true);
//uint8 StopCar = FALSE;
uint8 gotime;

// void AIControl(void)
// {   //成功最多次
//     if(RoadFlag.AI == 0) return; 
//     // 左避障
//     if(RoadFlag.AI == 1)
//     {
//         CarInfo.dirOffset = 56; 
//         BuzzerMs(50);
//         if(RoadFlag.AIDis >= 0.28)//0.3
//         {
//          RoadFlag.AI = 3; 
//         }
//     }
//     // 第二阶段：右转回正
//     else if(RoadFlag.AI == 3 )
//     {
//         CarInfo.dirOffset = -63; 
//         RoadFlag.AI = 5;
//     }
//     else if(RoadFlag.AI == 5 && RoadFlag.AIDis >= 0.63)//0.64
//     {
//         if(Left == 0 && Right == 179)
//         {
//             CarInfo.dirOffset = 30;
//             if(RoadFlag.AIDis >= 0.73)
//             {
//               RoadFlag.AI = 0; // 清零，恢复正常巡线
//               RoadFlag.AIDis = 0;
//             }
//         }
//         else
//         {
//          RoadFlag.AI = 0; // 清零，恢复正常巡线
//          RoadFlag.AIDis = 0;
//         }
//     }
// }
void AIControl(void)
{
    if(RoadFlag.AI == 0) return;
    // 阶段1：左转 (正 dirOffset)
    if(RoadFlag.AI == 2)
    {
        CarInfo.dirOffset = 56;                // 正 = 左转（与早期成功一致）
        printf("S2 dir=56 dis=%.3f\r\n", RoadFlag.AIDis);
        if(RoadFlag.AIDis >= 0.35f)
        {
            RoadFlag.AI = 3;
        }
    }
    // 阶段2：直行
    else if(RoadFlag.AI == 3)
    {
        CarInfo.dirOffset = 0;
        printf("S3 dir=0 dis=%.3f\r\n", RoadFlag.AIDis);
        if(RoadFlag.AIDis >= 0.45f)
        {
            RoadFlag.AI = 4;
        }
    }
    // 阶段3：右转回正 (负 dirOffset)
    else if(RoadFlag.AI == 4)
    {
        CarInfo.dirOffset = -63;               // 负 = 右转
        printf("S4 dir=-63 dis=%.3f\r\n", RoadFlag.AIDis);
        if(RoadFlag.AIDis >= 0.70f)
        {
            RoadFlag.AI = 0;
            CarInfo.dirOffset = 0;
            RoadFlag.AIDis = 0;
            BuzzerMs(10);
        }
    }
}
// void AIControl(void)
// {
//     if(RoadFlag.AI == 0) return;

//     // 阶段1：左转至 30° (AI == 2)
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = 55;                
//         printf("S2 angle=%.1f dir=55 dis=%.3f\r\n", RoadFlag.AIAngle, RoadFlag.AIDis);

//         if(RoadFlag.AIAngle >= 28.0f || RoadFlag.AIDis > 0.35f)
//         {
//             RoadFlag.AI = 4;                 
//             printf("---- S2 -> S4 (angle=%.1f) ----\r\n", RoadFlag.AIAngle);
//         }
//     }
//     // 阶段2：右转回正，目标角度 0° (AI == 4)
//     else if(RoadFlag.AI == 4)
//     {
//         CarInfo.dirOffset = -55;             
//         printf("S4 angle=%.1f dir=-55 dis=%.3f\r\n", RoadFlag.AIAngle, RoadFlag.AIDis);

//         if((RoadFlag.AIAngle <= 3.0f && RoadFlag.AIAngle >= -3.0f) || RoadFlag.AIDis > 0.70f)
//         {
//             RoadFlag.AI = 0;
//             CarInfo.dirOffset = 0;
//             RoadFlag.AIDis = 0;
//             BuzzerMs(10);
//             printf("---- End (angle=%.1f) ----\r\n", RoadFlag.AIAngle);
//         }
//     }
// }
// void AIControl(void)
// {
//     if(RoadFlag.AI == 0) return;

//     // 阶段1：左转至 
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = 55;                // 左转
//         printf("S2 angle=%.1f dir=50 dis=%.3f\r\n",  RoadFlag.AIAngle, RoadFlag.AIDis);

//         if( RoadFlag.AIAngle >= 28.0f || RoadFlag.AIDis > 0.50f)
//         {
//             RoadFlag.AI = 3;
//             printf("---- S2 -> S3 (angle=%.1f) ----\r\n", RoadFlag.AIAngle);
//         }
//     }
//     // 阶段2：极短直行 (AI == 3)
//     else if(RoadFlag.AI == 3)
//     {
//         CarInfo.dirOffset = 0;
//         printf("S3 angle=%.1f dir=0 dis=%.3f\r\n", RoadFlag.AIAngle, RoadFlag.AIDis);

//         if(RoadFlag.AIDis >= 0.45f)            // 总距 0.45m（直行仅0.15m左右）
//         {
//             RoadFlag.AI = 4;
//             printf("---- S3 -> S4 ----\r\n");
//         }
//     }
//     // 阶段3：右转固定距离回正 (AI == 4)
//     else if(RoadFlag.AI == 4)
//     {
//         CarInfo.dirOffset = -55;
//         printf("S4 angle=%.1f dir=-50 dis=%.3f\r\n",  RoadFlag.AIAngle, RoadFlag.AIDis);

//         if(RoadFlag.AIDis >= 0.65f)            // 总距 0.65m（右转 0.20m）
//         {
//             RoadFlag.AI = 0;
//             CarInfo.dirOffset = 0;
//             RoadFlag.AIDis = 0;
//             BuzzerMs(10);
//             printf("---- End (angle=%.1f) ----\r\n", RoadFlag.AIAngle);
//         }
//     }
// }
//     //右避障
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = -56;
//         Left = 90; 
//         Right = COL - 1; 
       
//         if(RoadFlag.AIDis >= 0.28)//0.3
//         {
//             RoadFlag.AI = 4; 
//         }
//     }
    
//     // 第二阶段：左转回正
//     else if(RoadFlag.AI == 4 )
//     {
//             CarInfo.dirOffset = 63; 
//             Right = COL - 90; 
//             Left = 0;
//             RoadFlag.AI = 6;
//     }
//     else  if(RoadFlag.AI == 6 && RoadFlag.AIDis >= 0.63)//0.64
//     {
//         RoadFlag.AI = 0; // 清零，恢复正常巡线
//         RoadFlag.AIDis = 0;
//     }
// }
// //2能用
// void AIControl(void)
// {
//     if(RoadFlag.AI == 0) return; 
//     // 第一阶段：左转避让
//     if(RoadFlag.AI == 2)
//     {
//         Left = COL - 1- LeftBlack[DataInfo.forwardLook];
//         CarInfo.dirOffset = (90 - Left + 20) ; 
//         CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, 60, -60);
//         printf("绕行开始: Left=%d, Right=%d, dirOffset=%d\n", Left, Right, CarInfo.dirOffset);
//         BuzzerMs(20);
//         if(RoadFlag.AIDis >= 0.5)
//         {
//             RoadFlag.AI = 4; 
//         }
//     }
//     // // 第二阶段：右转回正
//     else if(RoadFlag.AI == 4 )
//     {
//             CarInfo.dirOffset = -35;    
//             BuzzerMs(10);
//            if(RoadFlag.AIDis >= 0.7)
//             {
//                 RoadFlag.AI = 6;
//             }
//     }
//     else  if(RoadFlag.AI == 6 )
//     {
//         BuzzerMs(20);
//         RoadFlag.AI = 0; 
//         RoadFlag.AIDis = 0;
//         printf("绕行结束: Left=%d, Right=%d, dirOffset=%d\n", Left, Right, CarInfo.dirOffset);
        
//     }
// // }

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      AI绕行控制
// void AIControl(void)
// {
//     // 左避障
//     if(RoadFlag.AI == 1)
//     {
//        CarInfo.dirOffset = 60;
//         BuzzerMs(200); 
//         // 跑够 0.25 米，说明车身已经开始探出，切入阶段 3
//         if(RoadFlag.AIDis > 0.35) 
//         {
//             RoadFlag.AI = 3;
//         }
//     }
//     //保持打角
//     else if(RoadFlag.AI == 3)
//     {
//        CarInfo.dirOffset = 60; 
        
//         if(fabs(RoadFlag.AIAngle) > 20 && RoadFlag.AIDis >= 0.4) 
//         {
//             RoadFlag.AI = 7; // 角度够了，切入阶段 7 开始猛烈右转回摆
//         }
//     }
//     // 持续右转，直到车子往前跑够 0.4 米
//     else if(RoadFlag.AI == 7)
//     {
//        CarInfo.dirOffset = -50; 
        
//         if(RoadFlag.AIDis >= 0.6) 
//         {
//             RoadFlag.AI = 9; 
//         }
//     }
//     //平缓右转
//     else if(RoadFlag.AI == 9)
//     {
   
//         CarInfo.dirOffset = -20; 
//         if(RoadFlag.AIAngle < -12 && RoadFlag.AIDis >= 0.75)
//         {
//             // 1. 左右都不丢边（完美回归）
//             if(Left != 0 && Right != 179 && LongLeft > DataInfo.forwardLook) 
//             {
//                 CarInfo.dirOffset = 35; 
//                 RoadFlag.AI = 0;       
//             }
//             // 2. 左右都丢边
//             else if(Left == 0 && Right == 179)
//             {
//                 CarInfo.dirOffset = 45;
//                 RoadFlag.AI = 0;
//             }
//             // 3. 左边丢边，右边不丢边
//             else if(Left == 0 && Right != 179)
//             {
//                 CarInfo.dirOffset = 43;
//                 RoadFlag.AI = 0;
//                 BuzzerMs(200); 
//             }
//         }
//     }
// }

//-------------------------------------------------------------------------------------------------------------------
//  @brief      斑马线控制
//-------------------------------------------------------------------------------------------------------------------
void ZebraControl(void)
{
    if(ZebraTime == 1 && DataInfo.GOtime == 1)    ZebraTime = 2;

    if(ZebraTime == 1 && DataInfo.GOtime == 2)
    {
        if(DataInfo.door == 0)
        CarInfo.dirOffset = (int16)((Left_Normal - Right) * 0.2); //1.0
        else if(DataInfo.door == 1)
        CarInfo.dirOffset = (int16)((Left - Right_Normal) * 0.2);//0.5
    }

    if(ZebraTime == 2)   //ZebraTime == 2 RoadFlag.zebra
    {
        if(DataInfo.door == 0)
        {
            if(RoadFlag.zebra == 1)
            {
                CarInfo.dirOffset = (int16)((Left_Normal - Right) * 2.5);//(int16)((Left_Normal - Right) * 2.5)
            }
            else if(RoadFlag.zebra == 2)
            {
                CarInfo.dirOffset = 90;
            }
            else if(RoadFlag.zebra == 3)
            {
                CarInfo.dirOffset = 0;
            }
        }
        else if(DataInfo.door == 1)
        {
            if(RoadFlag.zebra == 1)
           {
//                CarInfo.dirOffset = (int16)((LeftBlack[DataInfo.forwardLook] - Left_Normal) * 2.0);
               CarInfo.dirOffset = (int16)((Left - Right_Normal) * 2.5);
           }
           else if(RoadFlag.zebra == 2)
           {
             CarInfo.dirOffset = -90;
           }
           else if(RoadFlag.zebra == 3)
           {
               CarInfo.dirOffset = 0;
           }
        }
    }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      十字控制
//-------------------------------------------------------------------------------------------------------------------
void CrossControl(void)
{
    uint8 i = 0;
    uint8 summation = 0;    //十字后白点列数

    if(CarInfo.dirOffset < 0)
    {
          for(i = 30; i < COL; i++)
          {
              if(WhiteCol[i] >= DataInfo.crossline && WhiteCol[i + 1] > DataInfo.crossline)
              {
                  summation += i;
                  break;
              }
          }

          for(; i < COL - 1; i++)
          {
              if(WhiteCol[i] <= DataInfo.crossline && WhiteCol[i + 1] < DataInfo.crossline)
              {
                  summation += i;
                  break;
              }
          }
    }
    else
    {
          for(i = COL-30; i > 0; i--)
          {
              if(WhiteCol[i - 1] >= DataInfo.crossline && WhiteCol[i - 2] > DataInfo.crossline)
              {
                  summation += i - 1;
                  break;
              }
          }

          for(; i > 1; i--)
          {
              if(WhiteCol[i - 1] <= DataInfo.crossline && WhiteCol[i - 2] < DataInfo.crossline)
              {
                  summation += i - 1;
                  break;
              }
          }
    }

    CarInfo.dirOffset = (int)((summation - MIDDLE) * 0.4);//0.8

//    if(crosstime != 0 && Left == 0 && Right != 119)          CarInfo.dirOffset = 65;
//     else if(crosstime != 0 && Left != 0 && Right == 119)     CarInfo.dirOffset = -65;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      坡道控制
//-------------------------------------------------------------------------------------------------------------------
void RampControl(void)
{
    uint8 i = 0;
    int IT_offset = 0;

    for (i = 0;i < 4;i++)//4 12
    {
        IT_offset +=  (LeftBlack[i] + RightBlack[i] - MIDDLE);
    }

//    CarInfo.dirOffset = 0;
//    CarInfo.dirOffset = (IT_offset >> 3);

    CarInfo.dirOffset = (IT_offset >> 2);
    if(fabs(CarInfo.dirOffset) >= 13)
        CarInfo.dirOffset = 0;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      避障控制
//-------------------------------------------------------------------------------------------------------------------
void Obstacle_Control(void)
{
    //左避障
    if(RoadFlag.obstacle == 1)
    {
      //  CarInfo.dirOffset = 90;         //打角出去
      RoadFlag.obstacleAngle=28;
        if(RoadFlag.obstDis > 0.40)//0.4 0.5 0.4 45
        {
            RoadFlag.obstacle = 3;
        }
    }
    else if(RoadFlag.obstacle == 3 && fabs(RoadFlag.obstacleAngle)>30)
    {
        CarInfo.dirOffset = 0;          //打完角后走小段距离
        if(RoadFlag.obstDis > 0.45)//0.6
            RoadFlag.obstacle = 5;
    }
    else if(RoadFlag.obstacle == 5)
    {
        CarInfo.dirOffset = -50;        //打回一个角度角
        RoadFlag.obstacle = 7;
    }
    else if(RoadFlag.obstacle == 7 && RoadFlag.obstDis > DataInfo.obsdis2L)//0.8 0.9 0.7 0.6     写入flash
    {
        CarInfo.dirOffset = -20;        //打角回赛道 45 35
        RoadFlag.obstacle = 9;
    }
    else if(RoadFlag.obstacle == 9 && RoadFlag.obstacleAngle < -10)
    {
        //左右都不丢边
        if(Left != 0 && Right != 119 && LongLeft >DataInfo.forwardLook && RoadFlag.obstDis >= DataInfo.obsdis3L) //0.5 1.0 写入flash
        {
            CarInfo.dirOffset = 40;//-30  MIDDLE - Left - Right; 25
            RoadFlag.obstacle = 0;
//            BuzzerMs(200);
        }
        //左右都丢边
        else if(Left==0 && Right==119 && RoadFlag.obstDis >= DataInfo.obsdis3L)//1.1
        {
            CarInfo.dirOffset = 45;//15 30
            RoadFlag.obstacle = 0;
//            BuzzerMs(200);
        }
        //左边丢边右边不丢边
        else if(Left == 0 && Right != 119 && RoadFlag.obstDis >= DataInfo.obsdis3L)//1.1 1.2
        {
            CarInfo.dirOffset = 35;//-30 20 15
            RoadFlag.obstacle = 0;
            BuzzerMs(200);
        }
    }
    //右避障
    if(RoadFlag.obstacle == 2)
    {
        CarInfo.dirOffset = -90;         //打角出去
        if(RoadFlag.obstDis > 0.4)
        {
            RoadFlag.obstacle = 4;
        }
    }
    else if(RoadFlag.obstacle == 4 && fabs(RoadFlag.obstacleAngle)>30) //30 40
    {
        CarInfo.dirOffset = 0;          //打完角后走小段距离
        if(RoadFlag.obstDis > 0.50)//0.6
            RoadFlag.obstacle = 6;
    }
    else if(RoadFlag.obstacle == 6)
    {
        CarInfo.dirOffset = 55;        //打回一个角度角 直行 55 55 50
        RoadFlag.obstacle = 8;
    }
    else if(RoadFlag.obstacle == 8 && RoadFlag.obstDis >= DataInfo.obsdis2R)//0.8  0.95 0.7
    {
        CarInfo.dirOffset = 20;        //打角回赛道  45 50 25
        RoadFlag.obstacle = 10;
        BuzzerMs(200);
    }
    else if(RoadFlag.obstacle == 10)
    {
        //左右都不丢边
        if(Left != 0 && Right != 119 && LongRight >DataInfo.forwardLook && RoadFlag.obstDis >= DataInfo.obsdis3R) //0.5  1.1 1.25 1.1
        {
            CarInfo.dirOffset = -35;          //MIDDLE - Left - Right; 30
            RoadFlag.obstacle = 0;
//            BuzzerMs(200);
        }
        //左右都丢边
        else if(Left==0 && Right==119 && RoadFlag.obstDis >= DataInfo.obsdis3R)   //1.25
        {
            CarInfo.dirOffset = -45;  //30
            RoadFlag.obstacle = 0;
            BuzzerMs(200);
        }
        //右边丢边左边不丢边
        else if(Left != 0 && Right == 119 && RoadFlag.obstDis >= DataInfo.obsdis3R)  //1.25
        {
            CarInfo.dirOffset = -35;  //30
            RoadFlag.obstacle = 0;
//            BuzzerMs(200);
        }
    }
}
