#include "zf_common_headfile.h"

#include <math.h>
uint8 CircleTime;
uint8 Circle_Width = 0;
uint8 Cir_Angular = 0;      //折角

void ImageCircle_init(void)
{

    Circle.Circle_size1 = 1;//1
    Circle.Circle_size2 = 2;//2
    Circle.Circle_size3 = 1;//3
    Circle.Circle_size4 = 2;//4
    //圆环 R50(1) R60(2) R80(3) R100(4)
    Circle.CircleSize=0;

    //R50
    Circle.Circle_R50In =2.00; //2.3 2.1 3.0 3.3 5.5
    Circle.Circle_R50Out=0.5;//1.6 1.0 1.2 1.0 0.8

    //R60
    Circle.Circle_R60In=2.70;//2.8 4.8 1.9 2.3 2.1
    Circle.Circle_R60Out=0.10;//3.2 3.7 1.5 1.7 0.7

    //R80
    Circle.Circle_R80In=1.80;
    Circle.Circle_R80Out=0.40;

    //R100
    Circle.Circle_R100In=1.00;
    Circle.Circle_R100Out=0.20;

    Circle.Speed_R50=150.0;//160 165
    Circle.Speed_R60=175.0;//105 70 85 120 185
    Circle.Speed_R80=190.0;//100



    //R50与R60用同一套转向PD
    Circle.CircleR50_LP = 4.80;//2.0 2.8 3.05 4.6
    Circle.CircleR50_LD = 0.00;//
    Circle.CircleR50_RP = 3.5;//2.0 2.8 1.8 1.45
    Circle.CircleR50_RD = 0.00;//

    Circle.CircleR60_LP = 5.0;//2.0 1.6 3
    Circle.CircleR60_RP = 4.0;
    Circle.CircleR60_D = 0.00;//0 0.4

    //R80与R100用同一套转向PD
    Circle.CircleR80_EP=0.65;
    Circle.CircleR80_ED=0.00;
    Circle.CircleR80_P=3.00;
    Circle.CircleR80_D=0.00;

//    Circle.CircleR100_EP=0.6;
//    Circle.CircleR100_ED=0.00;
    Circle.CircleR100_P=2.00;
//    Circle.CircleR100_D=0.00;


    Circle.CircleR60_Dis=3.5;
    Circle.CircleR80_Dis=5.0;
}

void Check_Circle(void)
{
    uint8 j=0, i=0, z=0;
    uint8 White_COL[15] = {0};
    //White_col[COL] = {0};
    uint8 circle_vertical=0 ;
    //WhiteLine=0;    //大于设定值的行数
    uint8 intermittent_flag=0;  //过渡元素
    uint8 first_width=0;

    switch(CircleTime)
    {
        case 0: Circle.CircleSize = Circle.Circle_size1; break;
        case 1: Circle.CircleSize = Circle.Circle_size2; break;
        case 2: Circle.CircleSize = Circle.Circle_size3; break;
        case 3: Circle.CircleSize = Circle.Circle_size4; break;
    }

    if(!RoadFlag.circle && !RoadFlag.cross && !RoadFlag.ramp && !RoadFlag.zebra &&!RoadFlag.obstacle && !RoadFlag.blind)
    {
        Circle_Width = LeftBlack[43] - RightBlack[43]; //判断宽度15——50  16   //原本是26 26可能已经到了圆环的中间的黑色圈圈处 这个要看摄像头调成什么样
        if(Circle_Width==COL-1)                                              //摄像头放得比较低，前瞻近，最远看得远的话 就得看什么时候识别到圆环 识别到圆环的时候下尖角和圆环中间                                                       
        {                                                                   //的黑色圆之间那一堆在多少行 且有多少行
            return;
        }
        // printf("Circle_Width = %d\n",Circle_Width);
        //判断有没有大于设定值的行数
        for(i = 0; i < ROW-1; i++)
        {
            if(WhiteRow[i] >= 75)//78  这个看识别到圆环的时候下尖角和圆环中间的黑色圆之间那一堆有大概多少行，多了少了都会影响识别，注意左右环都要适合
                circle_vertical ++;
        }
        // printf("circle_vertical = %d\n",circle_vertical);
        //右环判断
        //左边界接近切割行,右边界小于某个值并且大于一个值，长直道的白点数大于普通直道的白点数，宽度大于某个值
        if(LongLeft >= CIR_STRAIGHT && LongRight <= 40 && LongRight >= 5 && circle_vertical >= 15 && Circle_Width >= 70 ) //&& std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30
        {
            //如果左边界的坐标一行一行的增大,可能是其他元素,把它滤掉
            for(i=0,j=0;i<LongLeft-1;i++)
            {
                if(LeftBlack[i] < LeftBlack[i + 1])     j++;
                if(j >= 3)
                {
                    intermittent_flag = 1;
                    break;
                }
            }


            //以右边界的最高行作为判断宽度的依据,可以看出有没有宽度突变的情况,以及计算出赛道宽度
            first_width = LeftBlack[LongRight - 1] - RightBlack[LongRight - 1];
            // printf("intermittent_flag= %d\n",intermittent_flag);
            // printf("first_width = %d\n",first_width);
            //宽度大于普通赛道的宽度,不是过渡元素
            if(intermittent_flag == 0 && first_width >= 40)
            {
                //在右边界之上五行,看边界是否在扩大 如果有出现白点,则加
                for(j=0;j<5;j++)
                {
                    z = LongRight + j;
                    for(i = LeftBlack[z];i>0;i--)
                    {
                        if(Bmp[z][i-1] == 255)    White_COL[j]++;//这部分暂时没用到
                        else                    break;
                    }
                }

              //统计白点数组是否比原宽度大5,若有,则+1
              for(i = 0, j = 0; i < 5; i ++)
              {
                   if(WhiteRow[LongRight + i] > first_width +5 )//(LeftBlack[LongRight + i] - LeftBlack[LongRight -1+i])*2
                       j++; //宽度一直比原来的大
              }
             //若有三行,则为右环
              if(j >= 3){
                  RoadFlag.circle = 2;
                //   printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
                  RoadFlag.cross=0;
                 //BuzzerMs(100);
              }
           }
        }
        //左环判断
         //右边界靠近切割行,左边界小于设定值
         else if(LongRight >= CIR_STRAIGHT && LongLeft <= 40 && LongLeft >= 5 && circle_vertical >= 15 && Circle_Width >= 70 )   //&& std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30    //LongLeft <= 20 circle_vertical >=12  && Circle_Width >= 84
         {
             //判断有没有右边界越来越小的情况,可能是坡道,作为过渡元素
             for(i = 0, j = 0; i < LongRight - 1; i++)
             {
               if(RightBlack[i] > RightBlack[i + 1])     j++;

               if(j>=2)
               {
                 intermittent_flag = 1;
                 break;
                }
             }

             //以左边界为准,判断宽度变化
             first_width = LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1];
            // printf("first_width = %d\n",first_width);
            //排除过渡元素,宽度大于设定宽度
             if(intermittent_flag == 0 && first_width >= 40)
             {
//                  //在右边界之上五行,看边界是否在扩大 如果有出现白点,则加
//                  for(j=0;j<5;j++)
//                  {
//                      z = LongLeft + j;
//                      for(i = RightBlack[z] + 1; i < COL - 1; i++)
//                      {
//                          if(Bmp[z][i] == 255)    White_COL[j]++;
//                          else                    break;
//                      }
//                  }
//                  for(c = 50; c > 45; c--)//行
//                  {
//                      for(z = (COL / 2) - 5; z < (COL / 2) + 5; z++)
//                      {
//                          if(Bmp[c][z] == 255)
//                          {
//                              White_col[c]++;             //储存行白点数
//                              if(White_col[c] >= 6)      WhiteLine++;    //行黑点数大于32的行数
//                          }
//                      }
//                  }

//                  for(i = 0, j = 0; i < 5; i++)
//                  {
//                      if(WhiteRow[i] > first_width + 5)
//                          j++;
//                  }

//                   if(j >= 3 && WhiteLine >= 4)
//                   {
//                       RoadFlag.circle = 1;
//                       printf("RoadFlag.circle=%d\n",RoadFlag.circle);
//                       RoadFlag.cross=0;
// //                      BuzzerMs(100);
//                   }
              //统计白点数组是否比原宽度大5,若有,则+1
              for(i = 0, j = 0; i < 5; i ++)
              {
                   if(WhiteRow[LongLeft + i]  >= first_width  + 5 )//(RightBlack[LongLeft + i] - RightBlack[LongLeft -1+i])*2
                       j++; //宽度一直比原来的大
              }
             //若有三行,则为右环
              if(j >= 3){
                  RoadFlag.circle = 1;
                  printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
                  RoadFlag.cross=0;
                 //BuzzerMs(100);
              }
              }
         }

// //下面两个补丁
//         if(LongLeft==0 && LongRight!=0 && !RoadFlag.circle && Search_Stop_Line >= 45 && LeftBlack1[Search_Stop_Line-1]!=COL-1)
//         {
//         j=0;
//         for(i=0;i<ROW;i++)
//         {
//             if(LeftBlack1[i] !=COL-1)
//             {
//                 j=i;
//                 break;
//             }
//         }
//         // printf("L_j=%d\n",j);
//         if(j>=20 && j<=30 && std::fabs(CarInfo.dirOffset) <= 15)
//         {
//             for(i=40;i<ROW;i++)
//             {
//                 if((LeftBlack1[i]-LeftBlack1[i-1]) >= 10)
//                 {
//                     RoadFlag.circle=3;
//                     L_Bent_connection_ring=1;
//                     printf("L_Bent_connection_ring=%d\n",L_Bent_connection_ring);
//                     break;
//                 }
//             }
//         }

//      }
//      else if(LongRight==0 && LongLeft!=0 && !RoadFlag.circle && Search_Stop_Line >= 45 && RightBlack1[Search_Stop_Line-1]!=0)
//      {
//         j=0;
//         for(i=0;i<ROW;i++)
//         {
//             if(RightBlack1[i] != 0)
//             {
//                 j=i;
//                 break;
//             }
//         }
//         // printf("R_j=%d\n",j);
//         if(j>=20 && j<=30 && std::fabs(CarInfo.dirOffset) <= 15)
//         {
//             for(i=40;i<ROW;i++)
//             {
//                 if((RightBlack1[i]-RightBlack1[i-1]) <= -10)
//                 {
//                     RoadFlag.circle=4;
//                     R_Bent_connection_ring=1;
//                     printf("R_Bent_connection_ring=%d\n",R_Bent_connection_ring);
//                     break;
//                 }
//             }
//         }
//      }
//       
}
    //圆环
   else if(RoadFlag.circle)
    {
//        if(DataInfo.door == 0)   CircleTime = 0;         //左出
//        else if(DataInfo.door == 1)  CircleTime = 2;     //右出

        if(RoadFlag.circle == 1)
        {
//            for(i = DataInfo.forwardLook+8; i >= DataInfo.forwardLook+4; i--)
//            {
//                for(j = RightBlack[i]+3; j > RightBlack[i]+23; j--)
//                {
//                    if(Bmp[i][j] == 0) blackcount++;
//                }
//            }
//            if(blackcount >= 4)
//            {
//                RoadFlag.zebra = 1;
//                RoadFlag.circle = 0;
//            }
//            else
                for(i = 0; i < 15; i ++)
                    if(LeftBlack[i] == COL - 1)  j++;//左边界丢边
                if(j > 10)
                {
                    RoadFlag.circle = 3;
    //                BuzzerMs(100);
                }
        }
        else if(RoadFlag.circle == 3)
        {
            if(Circle.CircleSize == 1 && RoadFlag.circleDis >= 0.80)   //R50 0.67
                RoadFlag.circle=5;

            else if(Circle.CircleSize == 2 && RoadFlag.circleDis >= 0.80)   //R60  //0.6  1.02 0.75 0.8 0.85
            {
                RoadFlag.circle = 5;
                //BuzzerMs(100);
            }
            else if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 1.05)   //R80
                RoadFlag.circle = 5;

            else if(Circle.CircleSize == 4 && RoadFlag.circleDis >= 1.2)   //R100
                RoadFlag.circle = 5;
        }
//        else if(RoadFlag.circle==3)
//        {
//            for(i=8;i<25;i++)
//            {
//              if(LeftBlack[i] > 1 && LeftBlack[i] < COL-15)    j++;
//            }
//            //突然出现左边界
//            if(j>15)
//            {
//                    RoadFlag.circle=5;
//            }
//        }
        else if(RoadFlag.circle==5)
        {
            if(fabs(RoadFlag.circleAngle) > 38.0)//38    // &&Right < Release_Right && RightBlack[0] == 0 && RightBlack[1] == 0 && RightBlack[2] == 0
            {
                RoadFlag.circle = 7;
                BuzzerMs(100);
            }
        }
        else if(RoadFlag.circle==7)
        {
            if(LongRight > 30)//15
            {
                for(i = 15; i < LongRight + 6; i++) //10  +8
                {
                    //从右往左，中线位置在变大，说明前面有折角，找出有折角的那一行
                    if(MiddleLine[i] > MiddleLine[i + 1] && MiddleLine[i + 1] > MiddleLine[i + 2])
                    {
                        Cir_Angular = i;
                        break;
                    }
                }
            }
            if(fabs(RoadFlag.circleAngle) > 260.0) // ||Cir_Angular > 20)  //10 //260 270
            {    RoadFlag.circle = 9;
                BuzzerMs(100);
            }
        }
        else if(RoadFlag.circle == 9 && fabs(RoadFlag.circleAngle) > 330.0)
        {
            RoadFlag.circle = 13;
            Left = 0;
            LastLeft = 0;
            BuzzerMs(100);
        }
        else if(RoadFlag.circle==13)
        {
            if(LongLeft >= 15 && LongRight >= DataInfo.forwardLook && Bmp[3][0] == 0 && Bmp[3][119] == 0 && Bmp[4][0] == 0 && Bmp[4][119] == 0)
            {
                if(Circle.CircleSize == 1 && RoadFlag.circleDis >= 4.45) //R50
                {
                    RoadFlag.circle=0;
                    CircleTime ++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 2 && RoadFlag.circleDis >= 5.0) //R50
                {
                    RoadFlag.circle=0;
                    CircleTime ++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 5.6 )
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 4 && RoadFlag.circleDis >= 6.2)
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    intermtime ++;
                    BuzzerMs(200);
                }
//                else if(RoadFlag.circleDis >= 4.8 )
//                {
//                    RoadFlag.circle=0;
//                    CircleTime++;
//                    BuzzerMs(200);
//                }

            }
        }

        //右环
        if(RoadFlag.circle==2)
        {
            //
//            for(i=DataInfo.forwardLook+1; i<=DataInfo.forwardLook+8; i++)
//            {
//                 for(c=LeftBlack[i];c<RightBlack[i];c++)
//                {
//                    if(Bmp[j][i] == 0)
//                    {
//                        blackcount++;
//                    }
//                }
//            }
//            if(blackcount>=10)
//            {
////                intermittent_flag = 1;
//                RoadFlag.circle = 0;
//                RoadFlag.zebra=1;
//            }
            for(i = 0; i < 15; i ++)//15
            {
                 if(RightBlack[i] == 0)     ++ j;
            }
            if(j > 10)//10
            {
                RoadFlag.circle = 4;
//                BuzzerMs(100);
            }
        }
        else if(RoadFlag.circle==4)
        {
            if(Circle.CircleSize == 1 && RoadFlag.circleDis >= 0.80)   //R50 0.65 0.8
                RoadFlag.circle = 6;

            if(Circle.CircleSize == 2 && RoadFlag.circleDis > 0.95)   //R60  1.02 0.75 0.9
                RoadFlag.circle = 6;
//                    BuzzerMs(100);

            if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 1.05) //R80
                RoadFlag.circle = 6;

            if(Circle.CircleSize == 4 && RoadFlag.circleDis > 1.20)   //R100
                RoadFlag.circle = 6;
        }
        else if(RoadFlag.circle==6)
        {
            if(fabs(RoadFlag.circleAngle) > 38.0)// &&(Left> Release_Left && LeftBlack[0] == COL-1 && LeftBlack[1]== COL-1 && LeftBlack[2] == COL-1))
//            if(Left>Release_Left && LeftBlack[0]==COL-1 && LeftBlack[1]==COL- 1 &&LeftBlack[2]==COL-1)
            {
                RoadFlag.circle = 8;
                    BuzzerMs(100);
            }
        }
//没识别到标志位10 只靠标志位8的控制也能很好的跑完圆环出去直道。。。？
        else if(RoadFlag.circle == 8)
        {
            if(LongLeft > 30)
            {
                for(i = 15; i < LongLeft + 6;i++)
                {
                    //从右往左，中线位置在变大，说明前面有折角，找出有折角的那一行
                    if(MiddleLine[i] < MiddleLine[i + 1] && MiddleLine[i + 1] < MiddleLine[i + 2])
                    {
                        Cir_Angular = i;
                        break;
                    }
                }
//                if (WhiteSum >= 70 && LongLeft <= 40 && WhiteCol[65] <= 25 && RoadFlag.circleDis > 3.3)  //&& White_COL[ForwardLook - 5] >= 119 && White_COL[ForwardLook - 6] >= 119
//                {
//                    RoadFlag.circle = 10;
//                    BuzzerMs(100);
//                }
            }
            if(fabs(RoadFlag.circleAngle) > 260.0)//Cir_Angular > 19 ||   10  260
            {
                RoadFlag.circle = 10;
//                BuzzerMs(100);
            }
        }

        else if(RoadFlag.circle==10 && fabs(RoadFlag.circleAngle) > 330.0)//330.0 335
//        else if(LongLeft >= 35 && WhiteSum >= 60 && RoadFlag.circleDis >= 4.65)
        {
            RoadFlag.circle=14;
            Right = COL - 1;
            LastRight = COL -1;
            BuzzerMs(100);
        }
        else if(RoadFlag.circle==14)
        {
            if(LongRight >= 15 && LongLeft >= DataInfo.forwardLook && Bmp[3][0] == 0 && Bmp[3][119] == 0 && Bmp[4][0] == 0 && Bmp[4][119] == 0)
            {
                if(Circle.CircleSize == 1 && RoadFlag.circleDis >= 4.45) //R50
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 2 && RoadFlag.circleDis >= 5.0) //R50
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 5.6)
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    BuzzerMs(200);
                }
                else if(Circle.CircleSize == 4 && RoadFlag.circleDis >= 6.2)
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    intermtime++;
                    BuzzerMs(200);
                }
//                else if(RoadFlag.circleDis >= 4.8 )
//                {
//                    RoadFlag.circle=0;
//                    CircleTime++;
//                    BuzzerMs(200);
//                }

            }
        }
        if(CircleTime >= 2) CircleTime = 0;
//        if(DataInfo.door == 0 && CircleTime ==2)            CircleTime = 0;     //左出
//        else if(DataInfo.door == 1 && CircleTime ==4)       CircleTime = 2;     //右出
    }

}

void Circle_Control(void)
{
    uint8 i=0;
    float Slope=0;
    uint8 COL_site = 0;
    uint8 Angular = 0;
    //uint8 estimate = 0;
    //uint8 *p = 0;

    if(RoadFlag.circle == 2 || RoadFlag.circle == 4)
        CarInfo.dirOffset = (int16)((LeftBlack[DataInfo.forwardLook] - Left_Normal) * 2.0);
//    {
//        if(LongLeft >= DataInfo.forwardLook)
//            CarInfo.dirOffset = (int16)((LeftBlack[DataInfo.forwardLook] - Left_Normal) * 2.0);
//        else
//            CarInfo.dirOffset = (int16)((MIDDLE - Left - Left_Normal) * 2.0);
//    }
    else if(RoadFlag.circle == 6)
    {
        switch(Circle.CircleSize)
        {
            case 0:   RoadFlag.circle=0; break;
            case 1:   CarInfo.dirOffset = (int)(- (35 - LongRight) * Circle.Circle_R50In); break;
            case 2:   CarInfo.dirOffset = (int)(- (38 - LongRight) * Circle.Circle_R60In); break;
            case 3:   CarInfo.dirOffset = (int)(- (38 - LongRight) * Circle.Circle_R80In); break;
            case 4:   CarInfo.dirOffset = (int)(- (40 - LongRight) * Circle.Circle_R100In); break;
            default:break;
        }

//        if(LongRight > 35)
//        {
//            CarInfo.dirOffset = -Left;
//            if(CarInfo.dirOffset>-15)
//            {
//                CarInfo.dirOffset = -45;
//            }
//        }
//        else
//        {
//            CarInfo.dirOffset = (int)(- (38 - LongRight) * Circle.Circle_R60In);
//        }
        if(CarInfo.dirOffset > 0)            CarInfo.dirOffset = -CarInfo.dirOffset;
    }
    else if(RoadFlag.circle == 8)
    {
        if(LongLeft <= 20)    RoadFlag.blind=1;
        if(RoadFlag.blind==1)
        {
            CarInfo.dirOffset = -70;
        }
//        else
//        {
        else
            CarInfo.dirOffset = (MIDDLE - Left - Right) * 1.0;//0.9

//            if(CarInfo.dirOffset > -20)    CarInfo.dirOffset = -50;//-60  52  60
//
//              RoadFlag.blind=0;
//        }
    }
//    {
//         if(fabs(Left-Right)<32)         Right = COL-1;
//
//         CarInfo.dirOffset = MIDDLE - Left - Right;
//
//         if(LongRight <= DataInfo.forwardLook && LongLeft <= DataInfo.forwardLook &&LongLeft >=3 )
//         {
//             if(LongLeft>26)   estimate = LongLeft - 6;
//             else              estimate = LongLeft - 2;
//
//             for(j=3;j<estimate;j++)
//             {
//                 p=&Bmp[j][0];
//
//                 for(i=LeftBlack[j] - 1; i > RightBlack[j] + 1; i--)
//                 {
//                     if(*(p+i)==0 &&*(p+i-1)==0)  break;
//                 }
//                 if(i != RightBlack[j] + 1)      break;
//             }
//
//             if(j == estimate && LeftBlack[LongLeft - 1] < 60)        RoadFlag.blind=1;
//             if(RoadFlag.blind==1)
//             {
//                 CarInfo.dirOffset = -OFFSET_MAX;
//                 Right = COL-1;
//                 LastRight = COL-1;
//                 Left = 84;
//                 LastLeft = 84;
//             }
//         }
//
//     }
    else if(RoadFlag.circle == 10)
    {
        for(i = 0; i < COL; i ++)
        {
            if(WhiteCol[i] > 25)//25
            {
                COL_site = i;
                break;
            }
        }
        for(i = 5; i < LongLeft - 1; i ++)
        {
            if(MiddleLine[i] < MiddleLine[i + 1])
            {
                Angular = i;
                break;
            }
        }
        if(Angular >= DataInfo.forwardLook)
        {
            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;//0.8
                if( CarInfo.dirOffset < -85)        CarInfo.dirOffset = -48;//75
                else if(CarInfo.dirOffset > -20)    CarInfo.dirOffset = -40;//50 45
            }
            else if(Circle.CircleSize==1)
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.83;

        }
        else if(Angular > 0)
        {
            Slope = COL_site - LeftBlack[Angular]; //两点间距离作差

            switch(Circle.CircleSize)
            {
                case 1: Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R50Out;   break;//算出斜率
                case 2: Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R60Out;   break;
                case 3: Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R80Out;   break;
                case 4: Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R100Out;  break;
                default:break;
            }

            Slope = (Slope * (DataInfo.forwardLook - Angular)); //算出变化数

            if(fabs(Slope) > LeftBlack[Angular])    LeftBlack[DataInfo.forwardLook] = 0;
            else    LeftBlack[DataInfo.forwardLook] = LeftBlack[Angular] + (int)Slope;

            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;//0.8
                if( CarInfo.dirOffset < -85)        CarInfo.dirOffset = -48;//75 60
                else if(CarInfo.dirOffset > -20)    CarInfo.dirOffset = -40;//50 45
            }
            else if(Circle.CircleSize==1)
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.83;
        }
        else
        {
            Slope = COL_site - LeftBlack[LongLeft - 1]; //两点间距离作差(纵坐标之差)

            switch(Circle.CircleSize)
            {
                case 1:Slope = (Slope / (WhiteCol[COL_site] - LongLeft + 1)) *Circle.Circle_R50Out; break; //算出斜率
                case 2:Slope = (Slope / (WhiteCol[COL_site] - LongLeft + 1)) *Circle.Circle_R60Out; break;
                case 3:Slope = (Slope / (WhiteCol[COL_site] - LongLeft + 1)) *Circle.Circle_R80Out; break;
                case 4:Slope = (Slope / (WhiteCol[COL_site] - LongLeft + 1)) *Circle.Circle_R100Out; break;
            }
            Slope = (Slope * (DataInfo.forwardLook - LongLeft + 1)); //算出变化数

            if(fabs(Slope) > LeftBlack[Angular])    LeftBlack[DataInfo.forwardLook] = 0;
            else    LeftBlack[DataInfo.forwardLook] = LeftBlack[LongLeft - 1] + (int)Slope;

            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;//0.8
                if( CarInfo.dirOffset < -85)        CarInfo.dirOffset = -48;//75 60
                else if(CarInfo.dirOffset > -20)    CarInfo.dirOffset = -40;//50 45
            }
            else if(Circle.CircleSize==1)
                CarInfo.dirOffset = (LeftBlack[DataInfo.forwardLook] - MIDDLE) * 0.83;//0.85
        }
    }

    else if(RoadFlag.circle==14)
    {
        if(LongLeft >= DataInfo.forwardLook)
            CarInfo.dirOffset = (int16)((LeftBlack[DataInfo.forwardLook] - Left_Normal) * 2.0);//2.0
        else
            CarInfo.dirOffset = (int16)((MIDDLE - Left - Left_Normal) * 2.0);
    }

    else if(RoadFlag.circle==1||RoadFlag.circle==3)
        CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) * 1.0);//2.0
//    {
//        if(LongRight >= DataInfo.forwardLook)
//            CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) * 1.0);//2.0
//        else
//            CarInfo.dirOffset = (int16)((MIDDLE - Right - Right_Normal) * 2.0);
//    }
    else if(RoadFlag.circle==5)
    {

        switch(Circle.CircleSize)
        {
             case 0:   RoadFlag.circle=0; break;
             case 1:   CarInfo.dirOffset = (int)((35 - LongLeft) * Circle.Circle_R50In); break;
             case 2:   CarInfo.dirOffset = (int)((38 - LongLeft) * Circle.Circle_R60In); break;
             case 3:   CarInfo.dirOffset = (int)((38 - LongLeft) * Circle.Circle_R80In); break;
             case 4:   CarInfo.dirOffset = (int)((40 - LongLeft) * Circle.Circle_R100In); break;
             default:break;
        }
//        if(LongLeft > 35)//35
//        {
//            CarInfo.dirOffset  = MIDDLE- Right;
//            if(CarInfo.dirOffset < 15)
//            {
//                CarInfo.dirOffset = 45;//45
//            }
//        }
//        else if(LongLeft <= 35)
//        {
//            CarInfo.dirOffset = (int)((38 - LongLeft) * Circle.Circle_R60In);//Circle.Circle_R50InL
//        }

        if(CarInfo.dirOffset < 0)    CarInfo.dirOffset = -CarInfo.dirOffset;
    }
    else if(RoadFlag.circle==7)
    {
        if(LongRight <= 23)    //DataInfo.forwardLook 24
                RoadFlag.blind = 2;
        if(RoadFlag.blind == 2)
        {
            CarInfo.dirOffset = 80;
        }
//        else
//        {
//            RoadFlag.blind=0;
        else if(Circle.CircleSize >= 2)
        {
            CarInfo.dirOffset = (MIDDLE - Left - Right) * 2.0;
            if(CarInfo.dirOffset < 30)   //30  20
                CarInfo.dirOffset = 60;  //65  60 62
        }
        else if(Circle.CircleSize == 1)
            CarInfo.dirOffset = (MIDDLE - Left - Right) * 1.0;
        }
//    }
//    {
//            if(fabs(Left-Right)<32)     Left=0;
//
//            CarInfo.dirOffset = MIDDLE - Left - Right;
//
//            if(LongLeft<=DataInfo.forwardLook && LongRight<=DataInfo.forwardLook && LongRight>=3)
//            {
//                if(LongRight>26)     estimate = LongRight - 6;
//                else                 estimate = LongRight - 2;
//
//                for(j=3;j<estimate;j++)
//                {
//                    p=&Bmp[j][0];
//
//                    for(i = RightBlack[j] + 1; i < LeftBlack[j] - 1; i++)
//                    {
//                        if(*(p + i) == 0 && *(p + i + 1) == 0) break;
//                    }
//
//                    if(i != LeftBlack[j] - 1) break;
//                }
//
//                if(j == estimate && RightBlack[LongRight - 1] > 60) RoadFlag.blind=2;//60
//                if(RoadFlag.blind==2)
//                {
//                    CarInfo.dirOffset = OFFSET_MAX;
//                    Left=0;
//                    LastLeft =0;
//                    Right = 36;
//                    LastRight = 36;
//                }
//            }
//    }
    else if(RoadFlag.circle == 9)
    {
        for(i = COL-1; i > 0; i --)
        {
            if(WhiteCol[i] > 22)
            {
                COL_site = i;
                break;
            }
        }
        for(i = 10; i < LongRight - 1; i++)
        {
            if(MiddleLine[i] > MiddleLine[i+1]){
                Angular = i;
                break;
            }
        }
        if(Angular >= DataInfo.forwardLook)
        {
            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.85;
                if( CarInfo.dirOffset >85)           CarInfo.dirOffset=48;
                else if(CarInfo.dirOffset<20)    CarInfo.dirOffset = 40;//45
            }
            else if(Circle.CircleSize==1)
                CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;

        }
        else if(Angular > 0)
        {
           Slope = COL_site - RightBlack[Angular]; //两点间距离作差 y-y0

           switch(Circle.CircleSize)
           {
               case 1:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R50Out; break;//算出斜率 k=(y-y0)/(x-x0)
               case 2:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R60Out; break;
               case 3:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R80Out; break;
               case 4:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R100Out; break;
           }
           Slope = Slope * (DataInfo.forwardLook - Angular); //算出变化数

           if((RightBlack[Angular] + (int)Slope) >= COL - 1)
               RightBlack[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
           else
               RightBlack[DataInfo.forwardLook] = RightBlack[Angular] + (int)Slope;
           if(Circle.CircleSize >= 2)
           {
               CarInfo.dirOffset = (99 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.85;
               if( CarInfo.dirOffset >85)       CarInfo.dirOffset=48;//70
               else if(CarInfo.dirOffset<20)    CarInfo.dirOffset = 40;//50
           }
           else if(Circle.CircleSize == 1)
               CarInfo.dirOffset = (99 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;

       }
        else{
            Slope = COL_site - RightBlack[LongRight - 1]; //两点间距离作差 y-y0

            switch(Circle.CircleSize)
            {
                case 1:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R50Out; break;//算出斜率 k=(y-y0)/(x-x0)
                case 2:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R60Out; break;
                case 3:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R80Out; break;
                case 4:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R100Out; break;
            }
            Slope = (Slope * (DataInfo.forwardLook - LongRight + 1)); //算出变化数

            if((RightBlack[Angular] + (int)Slope) >= COL - 1)
                RightBlack[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
            else
                RightBlack[DataInfo.forwardLook] = RightBlack[LongRight - 1] + (int)Slope;

            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.85;
                if( CarInfo.dirOffset > 85)            CarInfo.dirOffset = 48;
                else if(CarInfo.dirOffset < 20)        CarInfo.dirOffset = 40;
            }
            else if(Circle.CircleSize == 1)
                CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.80;
        }
    }
    else if(RoadFlag.circle==13)
    {
        CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) * 2.5);
    }
}
