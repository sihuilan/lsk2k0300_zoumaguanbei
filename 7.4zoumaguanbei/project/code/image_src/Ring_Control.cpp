// #include "zf_common_headfile.h"


// #include <math.h>
// uint8 CircleTime;
// uint8 Circle_Width = 0;
// uint8 Cir_Angular = 0;      //折角

// uint8 cir_l_up[3]={0};               
// uint8 cir_r_up[3]={0};  
// uint8 cir_flag = 0;
// uint16 v_point_l_index = 0; 
// uint16 v_point_r_index = 0;
// int l_ring_Buxian[ROW][2] = {0};
// // uint8 buxian_flag = 0;


// uint8 point_check(void)
// {
//     if(left_points_turnUp[2] && left_up_mutation[2] 
//     &&  abs(left_points_turnUp[0] - left_up_mutation[0]) < 10
//     &&  abs(left_points_turnUp[1] - left_up_mutation[1]) < 10
//     )
//     {
//         if(data_statics_l - left_points_turnUp[3] > 50) {
//             return 0;
//         }

//         // int tmpCount = 0;
//         // int j = 10;
//         // for(int i = left_up_mutation[1]; i > 0 && j > 0; i--, j--)
//         // {
//         //     if(points_l[i][1] < 15)
//         //     {
//         //         tmpCount++;
//         //     }
//         // }

//         // if(tmpCount < 1)
//         // {
//         //       printf("b");
//         //     return 0;
//         // }

//         return 1;
//     }

//     return 0;

// }

// void buxian(void)
// {
//     uint8 buxian_flag = 0;
//     buxian_flag = point_check();

//     //右固定拉线
//         int16 x_base = R_boundary[0];
//         int16 y_base = points_r[0][1];
//         int16 x_target   = left_points_turnUp[0];
//         int16 y_target   = left_points_turnUp[1];

//         if (buxian_flag == 1)
//         {
           
//             float k_r = (float)(x_target - x_base) / (float)(y_target - y_base);
            
//             // 顺着 Y 轴，对右边撕裂处进行强行填补
//             for (uint8 i = y_base; i <= y_target; i++)
//             {
//                 if (i >= 0 && i < CROP_H) // 边界安全保护
//                 {
//                     // 直线方程：X = k * (Y - Y_down) + X_down
//                     int32 fill_x = (int32)(k_r * (i - y_base) + x_base);
//                     R_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
//                 }
//             }
//             CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
//         }
    
// }
// void ImageCircle_init(void)
// {

//     Circle.Circle_size1 = 1;//1
//     Circle.Circle_size2 = 2;//2
//     Circle.Circle_size3 = 1;//3
//     Circle.Circle_size4 = 2;//4
//     //圆环 R50(1) R60(2) R80(3) R100(4)
//     Circle.CircleSize=0;

//     //R50
//     Circle.Circle_R50In =2.00; //2.3 2.1 3.0 3.3 5.5
//     Circle.Circle_R50Out=0.5;//1.6 1.0 1.2 1.0 0.8
 
//     //R60
//     Circle.Circle_R60In=2.70;//2.8 4.8 1.9 2.3 2.1
//     Circle.Circle_R60Out=0.10;//3.2 3.7 1.5 1.7 0.7

//     //R80
//     Circle.Circle_R80In=1.80;
//     Circle.Circle_R80Out=0.40;

//     //R100
//     Circle.Circle_R100In=1.00;
//     Circle.Circle_R100Out=0.20;

//     Circle.Speed_R50=150.0;//160 165
//     Circle.Speed_R60=175.0;//105 70 85 120 185
//     Circle.Speed_R80=190.0;//100



//     //R50与R60用同一套转向PD
//     Circle.CircleR50_LP = 4.80;//2.0 2.8 3.05 4.6
//     Circle.CircleR50_LD = 0.00;//
//     Circle.CircleR50_RP = 3.5;//2.0 2.8 1.8 1.45
//     Circle.CircleR50_RD = 0.00;//

//     Circle.CircleR60_LP = 5.0;//2.0 1.6 3
//     Circle.CircleR60_RP = 4.0;
//     Circle.CircleR60_D = 0.00;//0 0.4

//     //R80与R100用同一套转向PD
//     Circle.CircleR80_EP=0.65;
//     Circle.CircleR80_ED=0.00;
//     Circle.CircleR80_P=3.00;
//     Circle.CircleR80_D=0.00;

// //    Circle.CircleR100_EP=0.6;
// //    Circle.CircleR100_ED=0.00;
//     Circle.CircleR100_P=2.00;
// //    Circle.CircleR100_D=0.00;


//     Circle.CircleR60_Dis=3.5;
//     Circle.CircleR80_Dis=5.0;
// }

// void Check_Circle(void)
// {
//     uint8 j=0, i=0, z=0;
//     uint8 White_COL[15] = {0};
//     //White_col[COL] = {0};
//     uint8 circle_vertical=0 ;
//     //WhiteLine=0;    //大于设定值的行数
//     uint8 intermittent_flag=0;  //过渡元素
//     uint8 first_width=0;

//     if(!RoadFlag.circle && !RoadFlag.cross && !RoadFlag.ramp && !RoadFlag.zebra &&!RoadFlag.obstacle && !RoadFlag.blind)
//     {
//         Circle_Width = L_boundary[38] - R_boundary[38]; //判断宽度15——50  16   //原本是26 26可能已经到了圆环的中间的黑色圈圈处 这个要看摄像头调成什么样
//         if(Circle_Width==COL-1)                                              //摄像头放得比较低，前瞻近，最远看得远的话 就得看什么时候识别到圆环 识别到圆环的时候下尖角和圆环中间                                                       
//         {                                                                   //的黑色圆之间那一堆在多少行 且有多少行
//             return;
//         }
//         // printf("Circle_Width = %d\n",Circle_Width);
//         //判断有没有大于设定值的行数
//         for(i = 0; i < ROW-1; i++)
//         {
//             if(WhiteRow[i] >= 120)//78  这个看识别到圆环的时候下尖角和圆环中间的黑色圆之间那一堆有大概多少行，多了少了都会影响识别，注意左右环都要适合
//                 circle_vertical ++;
//         }
//         // printf("circle_vertical = %d\n",circle_vertical);
//         //右环判断
//         //左边界接近切割行,右边界小于某个值并且大于一个值，长直道的白点数大于普通直道的白点数，宽度大于某个值
//         if(LongLeft >= CIR_STRAIGHT && LongRight <= 40 && LongRight >= 5 && circle_vertical >= 15 && Circle_Width >= 90 ) //&& std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30
//         {
//             //如果左边界的坐标一行一行的增大,可能是其他元素,把它滤掉
//             for(i=0,j=0;i<LongLeft-1;i++)
//             {
//                 if(L_boundary[i] < L_boundary[i + 1])     j++;
//                 if(j >= 3)
//                 {
//                     intermittent_flag = 1;
//                     break;
//                 }
//             }

//             //以右边界的最高行作为判断宽度的依据,可以看出有没有宽度突变的情况,以及计算出赛道宽度
//             first_width = L_boundary[LongRight - 1] - R_boundary[LongRight - 1];
//             // printf("intermittent_flag= %d\n",intermittent_flag);
//             // printf("first_width = %d\n",first_width);
//             //宽度大于普通赛道的宽度,不是过渡元素
//             if(intermittent_flag == 0 && first_width >= 40)
//             {
//                 //在右边界之上五行,看边界是否在扩大 如果有出现白点,则加
//                 for(j=0;j<5;j++)
//                 {
//                     z = LongRight + j;
//                     for(i = L_boundary[z];i>0;i--)
//                     {
//                         if(Bmp[z][i-1] == 255)    White_COL[j]++;//这部分暂时没用到
//                         else                    break;
//                     }
//                 }

//               //统计白点数组是否比原宽度大5,若有,则+1
//               for(i = 0, j = 0; i < 5; i ++)
//               {
//                    if(WhiteRow[LongRight + i] > first_width +5 )//(L_boundary[LongRight + i] - L_boundary[LongRight -1+i])*2
//                        j++; //宽度一直比原来的大
//               }
//              //若有三行,则为右环
//               if(j >= 3){
//                   RoadFlag.circle = 2;
//                 //   printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
//                   RoadFlag.cross=0;
//                  BuzzerMs(100);
//               }
//            }
//         }
//         //左环判断
//          //右边界靠近切割行,左边界小于设定值
//          else if(LongRight >= CIR_STRAIGHT && LongLeft <= 40 && LongLeft >= 5 && circle_vertical >= 15 && Circle_Width >= 90 )   //&& std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30    //LongLeft <= 20 circle_vertical >=12  && Circle_Width >= 84
//          {
//              //判断有没有右边界越来越小的情况,可能是坡道,作为过渡元素
//              for(i = 0, j = 0; i < LongRight - 1; i++)
//              {
//                if(R_boundary[i] > R_boundary[i + 1])     j++;

//                if(j>=2)
//                {
//                  intermittent_flag = 1;
//                  break;
//                 }
//              }

//              //以左边界为准,判断宽度变化
//              first_width = L_boundary[LongLeft - 5] - R_boundary[LongLeft - 5];
//             // printf("first_width = %d\n",first_width);
//             //排除过渡元素,宽度大于设定宽度
//              if(intermittent_flag == 0 && first_width >= 40)
//              {
//               //统计白点数组是否比原宽度大5,若有,则+1
//               for(i = 0, j = 0; i < 5; i ++)
//               {
//                 // printf("a");
//                    if(WhiteRow[LongLeft + i]  >= first_width  + 5 )//(R_boundary[LongLeft + i] - R_boundary[LongLeft -1+i])*2
//                        j++; //宽度一直比原来的大
//               }
//              //若有三行,则为右环
//               if(j >= 3){
//                   RoadFlag.circle = 1;
//                 //   printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
//                   RoadFlag.cross=0;
//                  BuzzerMs(100);
//               }
//               }
//          }
//     }
//     //圆环
//    else if(RoadFlag.circle)
//     {
//         if(RoadFlag.circle == 1)
//         {
//                 for(i = 0; i < 15; i ++)
//                     if(L_boundary[i] == border_max)  j++;//左边界丢边
//                 if(j > 10)
//                 {
//                     RoadFlag.circle = 3;
//                    BuzzerMs(100);
//                 }             
//         }
//         else if(RoadFlag.circle == 3)
//         {
//             if(RoadFlag.circleDis >= 0.95)   //R50 0.67
//                 RoadFlag.circle=5;

//         } 
//         else if(RoadFlag.circle==5)
//         { 
//             BuzzerMs(100);
//             if(fabs(RoadFlag.circleAngle) > 50.0 && RoadFlag.circleDis >= 1.35 )//38    // &&Right < Release_Right && R_boundary[0] == 0 && R_boundary[1] == 0 && R_boundary[2] == 0
//             {
//                 RoadFlag.circle = 7;
               
//             }
//             printf("RoadFlag.circle=%d\n",RoadFlag.circle);  
//         }
//         else if(RoadFlag.circle==7)
//         {
//             if(LongRight > 30)//15
//             {
//                 for(i = 15; i < LongRight + 6; i++) //10  +8
//                 {
//                     //从右往左，中线位置在变大，说明前面有折角，找出有折角的那一行
//                     if(MiddleLine[i] > MiddleLine[i + 1] && MiddleLine[i + 1] > MiddleLine[i + 2])
//                     {
//                         Cir_Angular = i;
//                         break;
//                     }
//                 }
//             }
//             if(fabs(RoadFlag.circleAngle) > 260.0) // ||Cir_Angular > 20)  //10 //260 270
//             {    RoadFlag.circle = 9;
//                 BuzzerMs(100);
//             }
//         }
//         else if(RoadFlag.circle == 9 && fabs(RoadFlag.circleAngle) > 330.0)
//         {
//             RoadFlag.circle = 13;
//             Left = 0;
//             LastLeft = 0;
//             BuzzerMs(100);
//         }
//         else if(RoadFlag.circle==13)
//         {
//             if(LongLeft >= 15 && LongRight >= DataInfo.forwardLook && Bmp[3][0] == 0 && Bmp[3][119] == 0 && Bmp[4][0] == 0 && Bmp[4][119] == 0)
//             {
//                 if(RoadFlag.circleDis >= 4.45) //R50
//                 {
//                     RoadFlag.circle=0;
//                     CircleTime ++;
//                     BuzzerMs(200);
//                 }
//             }
//         }
//     }
// }

// void Circle_Control(void)
// {
//     uint8 i=0;
//     float Slope=0;
//     uint8 COL_site = 0;
//     uint8 Angular = 0;

//     if(RoadFlag.circle==1||RoadFlag.circle==3)
//     CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) * 2.0);//2.0

//     else if(RoadFlag.circle==5)
//     {

//         if(CarInfo.dirOffset < 0)    CarInfo.dirOffset = -CarInfo.dirOffset;
//     }
//     else if(RoadFlag.circle==7)
//     {
//         if(LongRight <= 23)    //DataInfo.forwardLook 24
//                 RoadFlag.blind = 2;
//         if(RoadFlag.blind == 2)
//         {
//             CarInfo.dirOffset = 80;
//         }
//         else if(Circle.CircleSize >= 2)
//         {
//             CarInfo.dirOffset = (MIDDLE - Left - Right) * 1.7;
//             if(CarInfo.dirOffset < 30)   //30  20
//                 CarInfo.dirOffset = 65;  //65  60 62
//             // printf("1=%d\n",CarInfo.dirOffset);
//         }
//     } 
//      else if(RoadFlag.circle == 9)
//     {
//         uint8 line , ShowSlope;
//             for(i = COL-1; i > 0; i --)
//             {
//                 if(WhiteCol[i] > 32)//22
//                 {
//                     COL_site = i;
//                     break;
//                 }
//             }

//             for(i = 10; i < LongRight - 1; i++){
//                 if(MiddleLine[i] > MiddleLine[i+1]){
//                     Angular = i;
//                     break;
//                 }
//             }
//             for(r=0;r<=35;r++)
//             {
//                 for(c=70;c<137;c++)
//                 {
//                     if(Bmp[r][c]==255 &&Bmp[r][c+1]==0 && Bmp[r][c+2]==0)
//                     {
//                         break;
//                     }
//                     else
//                     {
//                         Circle_WhiteRow[r]++;
//                     }
//                 }
//                 for(c=70;c>2;c--)
//                 {
//                     if(Bmp[r][c]==255 && Bmp[r][c-1]==0 && Bmp[r][c-2]==0)
//                     {
//                         break;
//                     }
//                     else
//                     {
//                         Circle_WhiteRow[r]++;
//                     }
//                 }
//                 if(Circle_WhiteRow[r]>=115)
//                 {
//                     line++;
//                 }
//             }

//             ShowAngular=Angular;
//                 if(Angular >= DataInfo.forwardLook)
//                 {
//                     if(Circle.CircleSize >= 2)
//                     {
//                         CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 1.1;//0.85
//                         if(CarInfo.dirOffset >85)           CarInfo.dirOffset=48;
//                         else if(CarInfo.dirOffset<20)    CarInfo.dirOffset = 40;//45
//                     }
//                     else if(Circle.CircleSize==1)
//                         CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 1.05;//0.8

//                 }
//                 else if(Angular > 0)
//                 {
//                 Slope = COL_site - RightBlack[Angular]; //两点间距离作差 y-y0
//                 ShowSlope=Slope;

//                     Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R60Out;
//                     Slope = Slope * (DataInfo.forwardLook - Angular); //算出变化数

//                 if(line>=10 && ShowSlope<=80.0)
//                     {
//                         Slope = ((COL-15)/ (19 - Angular))*Circle.Circle_R60Out;
//                         break;
//                     }
//                 }

//            if((RightBlack[Angular] + (int)Slope) >= COL - 1)
//                RightBlack[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
//            else
//            {
//                RightBlack[DataInfo.forwardLook] = RightBlack[Angular] + (int)Slope;
//                if(RightBlack[DataInfo.forwardLook]<110 && line>=10 && ShowSlope<=80.0 )
//                {
//                    RightBlack[DataInfo.forwardLook]=COL-1;
//                }
//            }
//                CarInfo.dirOffset = (109 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.95;
//                if(CarInfo.dirOffset<20)    CarInfo.dirOffset = 50;
       
//         else{
//             Slope = COL_site - RightBlack[LongRight - 1]; //两点间距离作差 y-y0

//             Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R60Out; 
//             Slope = (Slope * (DataInfo.forwardLook - LongRight + 1)); //算出变化数

//             if((RightBlack[Angular] + (int)Slope) >= COL - 1)
//                 RightBlack[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
//             else
//                 RightBlack[DataInfo.forwardLook] = RightBlack[LongRight - 1] + (int)Slope;
         
//             CarInfo.dirOffset = (COL-1 + RightBlack[DataInfo.forwardLook] - MIDDLE) * 0.85;//0.85
//             if( CarInfo.dirOffset > 85)            CarInfo.dirOffset = 48;
//             else if(CarInfo.dirOffset < 20)        CarInfo.dirOffset = 40;          
//             }
//     }
    
    
//     else if(RoadFlag.circle==13)
//     {
//         // CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) * 2.5);

//         if(LongRight >= DataInfo.forwardLook)
//         {
//             CarInfo.dirOffset = (int16)((RightBlack[DataInfo.forwardLook] - Right_Normal) );//2.0   
//             // printf("1\n");         
//         }

//         else
//         {
//             CarInfo.dirOffset = (int16)((MIDDLE - Right - Right_Normal) );      
//             // printf("2\n");
//         }
//     }
// }

