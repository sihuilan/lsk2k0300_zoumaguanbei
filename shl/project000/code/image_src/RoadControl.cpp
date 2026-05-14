#include "zf_common_headfile.h"

std::atomic<bool> Go_Flag(true);
//uint8 StopCar = FALSE;
uint8 gotime;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      AI绕行控制
//-------------------------------------------------------------------------------------------------------------------
void AIControl(void)
{
  if(RoadFlag.AI)
  {
   if(RoadFlag.AI=1)
 {   // 第一阶段：
    if(RoadFlag.AIDis <0.4)
    {
        CarInfo.dirOffset = 65; 
        Left = 0; 
        Right = COL - OFFSET_MAX;
    }
    // 第二阶段：右转回正（约 20 个周期）
    else if(RoadFlag.AIDis>=0.4)
    {
        // 强制右转，寻找赛道右边界
        CarInfo.dirOffset = -65; 
        Right = COL - 1; 
        Left = OFFSET_MAX;
        
    }
    else if(RoadFlag.AIDis<0.7)
    {
        RoadFlag.AI = 0;
    }
}
//      if(RoadFlag.AI=2)
//   {

//     // 第一阶段：
//     if(RoadFlag.AIDis <0.4)
//     {
//        CarInfo.dirOffset = -65; 
//         Right = COL - 1; 
//         Left = OFFSET_MAX;
//     }
//     // 第二阶段：右转回正（约 20 个周期）
//     else if(RoadFlag.AIDis>=0.4)
//     {
//         // 强制左转，寻找赛道右边界
//         CarInfo.dirOffset = 65; 
//         Left = 0; 
//         Right = COL - OFFSET_MAX;
        
//     }
//     else if(RoadFlag.AIDis<0.7)
//     {
//         RoadFlag.AI = 0;
//     }
// }
}

 
}







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
        CarInfo.dirOffset = 90;         //打角出去
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
