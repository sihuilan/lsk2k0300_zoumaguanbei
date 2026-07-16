#include "zf_common_headfile.h"

#include <math.h>
uint8 CircleTime;
uint8 Circle_Width = 0;
uint8 Cir_Angular = 0;      //折角

uint16 Angular;
float ShowSlope;
uint16 ShowAngular;
//左角点
int left_points_Out[4];

void Ring_Out(void)
{
    static int last_valid_x = 0;   
    static int last_valid_y = 0;   
    static int has_history = 0;    
    static int lose_frame_cnt = 0; 

    int current_frame_found = 0;   
    left_points_Out[2] = 0; // 默认清空

    if(data_statics_l > 9) 
    {
        for(int i = 4; i < data_statics_l - 4; i++)
        {
            if(dir_l[i] == 5 || dir_l[i] == 4) // 容错：允许边缘交界点稍微偏向4
            {
                int dir0conts = 0;
                if (dir_l[i - 1] == 4 || dir_l[i - 1] == 5 || dir_l[i - 1] == 6) dir0conts++;
                if (dir_l[i - 2] == 4 || dir_l[i - 2] == 5 || dir_l[i - 2] == 6) dir0conts++;
                if (dir_l[i - 3] == 4 || dir_l[i - 3] == 5 || dir_l[i - 3] == 6) dir0conts++;
                if (dir_l[i - 4] == 4 || dir_l[i - 4] == 5 || dir_l[i - 4] == 6) dir0conts++;
                
                if(dir0conts < 3) continue; 

                int dir3conts = 0;
                if (dir_l[i + 1] == 2 || dir_l[i + 1] == 3 || dir_l[i + 1] == 1) dir3conts++; // 引入 1 作为斜向容错
                if (dir_l[i + 2] == 2 || dir_l[i + 2] == 3 || dir_l[i + 2] == 1) dir3conts++;
                if (dir_l[i + 3] == 2 || dir_l[i + 3] == 3 || dir_l[i + 3] == 1) dir3conts++;
                if (dir_l[i + 4] == 2 || dir_l[i + 4] == 3 || dir_l[i + 4] == 1) dir3conts++;
                
                if(dir3conts < 3) continue;

                left_points_Out[0] = points_l[i][0];
                left_points_Out[1] = points_l[i][1];
                left_points_Out[2] = 1;
                left_points_Out[3] = i;

                // 刷新记忆
                last_valid_x = points_l[i][0];
                last_valid_y = points_l[i][1];
                has_history = 1;     
                lose_frame_cnt = 0;  
                
                current_frame_found = 1; 
                break; // 极速响应，抓到第一个立刻闪人，不耽误 CPU 时间
            }
        }
    }
    if (current_frame_found == 0) 
    {
        // 既然空间识别变快了，时间丢帧保护可以收紧到 5 帧，防止出环后拖泥带水
        if (has_history == 1 && lose_frame_cnt < 2) 
        {
            left_points_Out[0] = last_valid_x;   
            left_points_Out[1] = last_valid_y;   
            left_points_Out[2] = 1;                
            left_points_Out[3] = -1; // 标记为历史沿用点
            lose_frame_cnt++;                      
        } 
        else 
        {
            left_points_Out[2] = 0;
            has_history = 0;
            lose_frame_cnt = 0;
        }
    }
}
void R_Ring_buxian(void)
{
    Ring_Out();
    if (left_points_Out[2] == 1)
    {
        // 1. 提取出环角点
        int16 x_up_l = left_points_Out[0];
        int16 y_up_l = left_points_Out[1];

        // 2. 确定基准底部点
        int16 x_down_l = L_boundary[0]; // 底部边界点 X 坐标
        int16 y_down_l = 0;             // 底部边界点 Y 坐标

        if (y_up_l != y_down_l)
        {
            // 算出两点间的斜率
            float k_l = (float)(x_up_l - x_down_l) / (float)(y_up_l - y_down_l);
            
            // ?? 修改核心：从底部 0 开始，冲破角点限制，一路延伸到图像最远端 (CROP_H - 1)
            for (int16 i = y_down_l; i < CROP_H; i++)
            {
                // 使用相同的斜率延长整条线
                int32 fill_x = (int32)(k_l * (i - y_down_l) + x_down_l);
                
                L_boundary[i] = limit_a_b(fill_x, border_min, border_max); 
            }
        }
    }
}
// uint8 point_check(void)
// {
//     if(left_points_turnUp[2] && left_up_mutation[2] 
//     &&  abs(left_points_turnUp[0] - left_up_mutation[0]) < 10
//     &&  abs(left_points_turnUp[1] - left_up_mutation[1]) < 10
//     )
//     {
//         if(ROW - left_points_turnUp[1] > 50) {
//             return 0;
//         }
//         return 1;
//     }
//     else if(right_points_turnUp[2] && right_up_mutation[2] 
//     &&  abs(right_points_turnUp[0] - right_up_mutation[0]) < 10
//     &&  abs(right_points_turnUp[1] - right_up_mutation[1]) < 10
//     // &&  right_points_turnUp[0] > size && right_points_turnUp[0] < preprocess.Ring_into_Size_Max
//     )
//     {
//         if( ROW - right_points_turnUp[1] > 50) {
//             // printf("g\n");
//             return 0;
//         }
//         return 2;
//     }

//     return 0;
// }
// void OuttoRing_buxian(void)
// {
//     int16 x_base = 0;
//     int16 y_base = 0;
//     int16 x_target   = 0;
//     int16 y_target   = 0;
//     uint8 i;
//     for(uint8 i = COL-2; i > 0; i --)
//     {
//         if(WhiteCol[i] > 55 && WhiteCol[i] < 65)//25
//         {
//             x_target = i;
//             y_target = WhiteCol[i];            
//             printf("(%d,%d)\n",x_target,y_target);
//             break;
//         }
//     }
//     Ring_Find_Point();
//         x_base = Ring_left_mutation[0];
//         y_base = Ring_left_mutation[1];
//         if(abs(x_base - x_target) <= 3)         x_target -= 20;

//         // printf("(%d,%d)\n",x_base,y_base);
//     //左固定拉线
//     if(Ring_left_mutation[2] == 1)
//     {
//         float k_l = (float)(x_target - x_base) / (float)(y_target - y_base);
//         for (uint8 i = y_base; i <= y_target; i++)
//         {
//             if (i >= 0 && i < CROP_H) // 边界安全保护
//             {
//                 // 直线方程：X = k * (Y - Y_down) + X_down
//                 int32 fill_x = (int32)(k_l * (i - y_base) + x_base);
//                 L_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
//             }
//         }        
//     }
//     else if(Ring_left_mutation[2] == 0)
//     {
//         float k_l = (float)(x_target - L_boundary[0]) / (float)(y_target - 0);
//         for (uint8 i = 0; i <= y_target; i++)
//         {
//             if (i >= 0 && i < CROP_H) // 边界安全保护
//             {
//                 // 直线方程：X = k * (Y - Y_down) + X_down
//                 int32 fill_x = (int32)(k_l * (i - y_base) + x_base);
//                 L_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
//             }
//         }        
//     }
// }
// void IntoRing_buxian(void)
// {
//     uint8 buxian_flag = 0;
//     //右固定拉线
//     int16 x_base = R_boundary[0];
//     int16 y_base = points_r[0][1];
//     int16 x_target   = left_points_turnUp[0];
//     int16 y_target   = left_points_turnUp[1];

//     if (buxian_flag == 1)
//     {
//         x_base = R_boundary[0];
//         y_base = points_r[0][1];
//         x_target   = left_points_turnUp[0];
//         y_target   = left_points_turnUp[1];
//         float k_r = (float)(x_target - x_base) / (float)(y_target - y_base);
        
//         for (uint8 i = y_base; i <= y_target; i++)
//         {
//             if (i >= 0 && i < CROP_H) // 边界安全保护
//             {
//                 // 直线方程：X = k * (Y - Y_down) + X_down
//                 int32 fill_x = (int32)(k_r * (i - y_base) + x_base);
//                 R_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
//             }
//         }
//         // CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
//     }
//     else if (buxian_flag == 2)
//     {
//         x_base = L_boundary[0];
//         y_base = points_l[0][1];
//         x_target   = right_points_turnUp[0];
//         y_target   = right_points_turnUp[1];
//         float k_r = (float)(x_target - x_base) / (float)(y_target - y_base);
        
//         for (uint8 i = y_base; i <= y_target; i++)
//         {
//             if (i >= 0 && i < CROP_H) // 边界安全保护
//             {
//                 // 直线方程：X = k * (Y - Y_down) + X_down
//                 int32 fill_x = (int32)(k_r * (i - y_base) + x_base);
//                 L_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
//             }
//         }
//         // CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
//     }

// }
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
        Circle_Width = L_boundary[68] - R_boundary[68]; //判断宽度15——50  16   //原本是26 26可能已经到了圆环的中间的黑色圈圈处 这个要看摄像头调成什么样
        if(Circle_Width==COL-1)                                              //摄像头放得比较低，前瞻近，最远看得远的话 就得看什么时候识别到圆环 识别到圆环的时候下尖角和圆环中间                                                       
        {                                                                   //的黑色圆之间那一堆在多少行 且有多少行
            return;
        }
        // printf("Circle_Width = %d\n",Circle_Width);
        //判断有没有大于设定值的行数
        for(i = 0; i < ROW-1; i++)
        {
            if(WhiteRow[i] >= 120)//78  这个看识别到圆环的时候下尖角和圆环中间的黑色圆之间那一堆有大概多少行，多了少了都会影响识别，注意左右环都要适合
                circle_vertical ++;
        }
        // printf("circle_vertical = %d\n",circle_vertical);
        //右环判断
        //左边界接近切割行,右边界小于某个值并且大于一个值，长直道的白点数大于普通直道的白点数，宽度大于某个值
        if(LongLeft >= CIR_STRAIGHT && LongRight <= 68 && LongRight >= 5 && circle_vertical >= 50 && Circle_Width >= 130) // && std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30
        {
            //如果左边界的坐标一行一行的增大,可能是其他元素,把它滤掉
            for(i=0,j=0;i<LongLeft-1;i++)
            {
                if(L_boundary[i] < L_boundary[i + 1])     j++;
                if(j >= 3)
                {
                    intermittent_flag = 1;
                    break;
                }
            }
            //以右边界的最高行作为判断宽度的依据,可以看出有没有宽度突变的情况,以及计算出赛道宽度
            first_width = L_boundary[LongRight - 1] - R_boundary[LongRight - 1];
            // printf("intermittent_flag= %d\n",intermittent_flag);
            // printf("first_width = %d\n",first_width);
            //宽度大于普通赛道的宽度,不是过渡元素
            if(intermittent_flag == 0 && first_width >= 40)
            {
                //在右边界之上五行,看边界是否在扩大 如果有出现白点,则加
                for(j=0;j<5;j++)
                {
                    z = LongRight + j;
                    for(i = L_boundary[z];i>0;i--)
                    {
                        if(Bmp[z][i-1] == 255)    White_COL[j]++;//这部分暂时没用到
                        else                    break;
                    }
                }

              //统计白点数组是否比原宽度大5,若有,则+1
              for(i = 0, j = 0; i < 5; i ++)
              {
                   if(WhiteRow[LongRight + i] > first_width +5 )//(L_boundary[LongRight + i] - L_boundary[LongRight -1+i])*2
                       j++; //宽度一直比原来的大
              }
             //若有三行,则为右环
              if(j >= 3){
                  RoadFlag.circle = 2;
                //   printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
                  RoadFlag.cross=0;
                 BuzzerMs(100);
              }
           }
        }
        //左环判断
         //右边界靠近切割行,左边界小于设定值
         else if(LongRight >= CIR_STRAIGHT && LongLeft <= 68 && LongLeft >= 5 && circle_vertical >= 50  && Circle_Width >= 130 )   //    && std::abs(Longest_White_Column_Left[1]- Longest_White_Column_Right[1])>=30    //LongLeft <= 20 circle_vertical >=12  && Circle_Width >= 84
         {
             //判断有没有右边界越来越小的情况,可能是坡道,作为过渡元素
             for(i = 0, j = 0; i < LongRight - 1; i++)
             {
               if(R_boundary[i] > R_boundary[i + 1])     j++;

               if(j>=3)
               {
                 intermittent_flag = 1;
                 break;
                }
             }

             //以左边界为准,判断宽度变化
             first_width = L_boundary[LongLeft - 1] - R_boundary[LongLeft - 1];
            // first_width = L_boundary[DataInfo.forwardLook] - R_boundary[DataInfo.forwardLook];
            // printf("first_width = %d\n",first_width);
            //排除过渡元素,宽度大于设定宽度
             if(intermittent_flag == 0 && first_width >= 50)
             {
              //统计白点数组是否比原宽度大5,若有,则+1
              for(i = 0, j = 0; i < 5; i ++)
              {
                // printf("a");
                   if(WhiteRow[LongLeft + i]  >= first_width  + 5 )//(R_boundary[LongLeft + i] - R_boundary[LongLeft -1+i])*2
                        // printf("1 = %d\n",WhiteRow[LongLeft + i]);
                       j++; //宽度一直比原来的大
                    //    printf("1 = %d\n",j);
              }
             //若有三行,则为右环
              if(j >= 3){
                  RoadFlag.circle = 1;
                  printf("RoadFlag.circle=%d\n",RoadFlag.circle);                  
                  RoadFlag.cross=0;
                 BuzzerMs(100);
              }
              }
         }
    }
    //圆环
   else if(RoadFlag.circle)
    {
/***************************************************左环的标志位***************************************************************************/
/*****************************************************************************************************************************************/
        if(RoadFlag.circle == 1)
        {
            for(i = 0; i < 15; i ++)
            if(L_boundary[i] == border_max)  ++j;//左边界丢边
            if(j > 10)
            {
                RoadFlag.circle = 3;
            printf("%d\n",RoadFlag.circle);
                BuzzerMs(100);
            }
                
        }
        else if(RoadFlag.circle == 3)
        {
            if(Circle.CircleSize == 1 && RoadFlag.circleDis >= Circle.CircleR50_L_3To5_Dis)   //R50 0.67
            {
                RoadFlag.circle=5;
                BuzzerMs(200);
                printf("%d\n",RoadFlag.circle);
            }
            // else if(Circle.CircleSize == 2 && RoadFlag.circleDis >= 0.95)   //R60  //0.6  1.02 0.75 0.8 0.85
            // {
            //     RoadFlag.circle = 5;
            //     // BuzzerMs(100);
            // }
            // else if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 1.05)   //R80
            //     RoadFlag.circle = 5;

            // else if(Circle.CircleSize == 4 && RoadFlag.circleDis >= 1.2)   //R100  
                // printf("RoadFlag.circle=%d\n",RoadFlag.circle);  
        } 
        else if(RoadFlag.circle==5)
        { 
            if(fabs(RoadFlag.circleAngle) > Circle.CircleR50_5To7_Ang )//38 && RoadFlag.circleDis >= Circle.CircleR50_L_5To7_Dis    // &&Right < Release_Right && R_boundary[0] == 0 && R_boundary[1] == 0 && R_boundary[2] == 0
            {
            BuzzerMs(200);
            RoadFlag.circle = 7;
            printf("%d\n",RoadFlag.circle);
               
            }
            // printf("RoadFlag.circle=%d\n",RoadFlag.circle);  
        }
        else if(RoadFlag.circle==7)
        {
            // if(LongRight > 30)//15
            // {
            //     for(i = 15; i < LongRight + 6; i++) //10  +8
            //     {
            //         //从右往左，中线位置在变大，说明前面有折角，找出有折角的那一行
            //         if(MiddleLine[i] > MiddleLine[i + 1] && MiddleLine[i + 1] > MiddleLine[i + 2])
            //         {
            //             Cir_Angular = i;
            //             break;
            //         }
            //     }
            // }
            if(fabs(RoadFlag.circleAngle) > Circle.CircleR50_7To9_Ang) // ||Cir_Angular > 20)  //10 //260 270
            {    
                RoadFlag.circle = 9;
                // RoadFlag.protect = 1;
                printf("%d\n",RoadFlag.circle);
                BuzzerMs(100);
            }
        }
        else if(RoadFlag.circle == 9 && fabs(RoadFlag.circleAngle) > Circle.CircleR50_9To13_Ang)
        {
            RoadFlag.circle = 13;
            printf("%d\n",RoadFlag.circle);
            BuzzerMs(200);
            // BuzzerMs(100);
        }
        else if(RoadFlag.circle==13)
        {
            if(LongRight >= 60 )//&& LongRight >= DataInfo.forwardLook && Bmp[3][0] == 0 && Bmp[3][119] == 0 && Bmp[4][0] == 0 && Bmp[4][119] == 0)
            {
                if(Circle.CircleSize == 1 && RoadFlag.circleDis >= Circle.CircleR50_L_13_Dis) //R50
                {
                    RoadFlag.circle=0;
                    printf("%d\n",RoadFlag.circle);
                    CircleTime ++;
                    // BuzzerMs(200);
                }
                else if(Circle.CircleSize == 2 && RoadFlag.circleDis >= 5.0) //R50
                {
                    RoadFlag.circle=0;
                    CircleTime ++;
                    // BuzzerMs(200);
                }
                else if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 5.6 )
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    // BuzzerMs(200);
                }
                else if(Circle.CircleSize == 4 && RoadFlag.circleDis >= 6.2)
                {
                    RoadFlag.circle=0;
                    CircleTime++;
                    intermtime ++;
                    // BuzzerMs(200);
                }
            }
        }
/***************************************************右环的标志位***************************************************************************/
/*****************************************************************************************************************************************/
        if(RoadFlag.circle==2)
        {
            // printf("%d\n",Circle.CircleSize);
            for(i = 0; i < 15; i ++)//15
            {
                 if(R_boundary[i] == border_min)     ++ j;
            }
            if(j > 10)//10
            {
                RoadFlag.circle = 4;
               BuzzerMs(100);
               printf("%d\n",RoadFlag.circle);
            }
        }
        else if(RoadFlag.circle==4)
        {
            
            if(Circle.CircleSize == 1 && RoadFlag.circleDis >= Circle.CircleR50_R_4To6_Dis)   //R50 0.65 0.8
            {
               RoadFlag.circle = 6;     
               BuzzerMs(200);           
            printf("%d\n",RoadFlag.circle);
            }
            // if(Circle.CircleSize == 2 && RoadFlag.circleDis > 0.95)   //R60  1.02 0.75 0.9
            //     RoadFlag.circle = 6;
                    
            // if(Circle.CircleSize == 3 && RoadFlag.circleDis >= 1.05) //R80
            //     RoadFlag.circle = 6;

            // if(Circle.CircleSize == 4 && RoadFlag.circleDis > 1.20)   //R100
            //     RoadFlag.circle = 6;
        }
        else if(RoadFlag.circle==6)
        {
            if(fabs(RoadFlag.circleAngle) > Circle.CircleR50_5To7_Ang)// &&(Left> Release_Left && L_boundary[0] == COL-1 && L_boundary[1]== COL-1 && L_boundary[2] == COL-1))
            {
                RoadFlag.circle = 8;
                printf("%d\n",RoadFlag.circle);
                BuzzerMs(200);
            }
        }
        else if(RoadFlag.circle == 8)
        {
            if(fabs(RoadFlag.circleAngle) > Circle.CircleR50_7To9_Ang)//Cir_Angular > 19 ||   10  260
            {
                RoadFlag.circle = 10;
                printf("%d\n",RoadFlag.circle);
                BuzzerMs(200);

            }
        }

        else if(RoadFlag.circle==10 && fabs(RoadFlag.circleAngle) > Circle.CircleR50_9To13_Ang  )//330.0 335&& RoadFlag.circleDis >= 4.25
        {
            // int offset0 = 0, k = 0;
            // for(uint i = 10;i <= 55; i++)
            // {offset0 = L_boundary[i+1] - L_boundary[i];
            //     if(offset0 < 0)     ++k;
            // }
            // // printf("%d\n",offset0);
            // if(k>=25 )   // && (Bmp[3][198] == 255 && Bmp[3][197] == 255 && Bmp[4][198] == 255 && Bmp[4][197] == 255)  
            // {
            RoadFlag.circle=14;
            printf("%d\n",RoadFlag.circle);      
            BuzzerMs(200);
        }
        else if(RoadFlag.circle==14)
        {
            if(LongLeft >= 60 )//LongRight >= 15 && LongLeft >= DataInfo.forwardLook && L_boundary1[5]-R_boundary1[5] <= Standard_Road_Wide[5]+4 
            {
                if(Circle.CircleSize == 1 && RoadFlag.circleDis >= Circle.CircleR50_R_14_Dis) //R50
                {
                    RoadFlag.circle=0;
                    printf("%d\n",RoadFlag.circle);
                    // RoadFlag.protect = 1;
                    CircleTime++;
                    // printf("1\n");
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
            }
        }
    }

}

void Circle_Control(void)
{
    uint8 i=0,j=0,r=0,c=0;
    uint8 line=0;
    float Slope=0;
    uint8 COL_site = 0;
    uint8 Angular = 0;
    uint16 Circle_WhiteRow[25];
    //uint8 estimate = 0;
    //uint8 *p = 0;

    if(RoadFlag.circle == 2 || RoadFlag.circle == 4)
        CarInfo.dirOffset = ((L_boundary[30] - Standard_Haf_Road_Wide[30] - MIDDLE/2 ) * 1.0);  
    else if(RoadFlag.circle == 6)
    {
        // dispose_line();
        // jiaodian_check();
        // IntoRing_buxian();
        // CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
        // // if(abs(CarInfo.dirOffset) < 30)     CarInfo.dirOffset = -65;
        // if(CarInfo.dirOffset >= -50)            CarInfo.dirOffset = -50;
        // -CarInfo.dirOffset;
        if(LongRight > 50)
        {
            CarInfo.dirOffset = -Left;
            if(CarInfo.dirOffset>-25)
            {
                CarInfo.dirOffset = -80;
            }
        }
        else{
            switch(static_cast<int>(Circle.CircleSize))
                {
                    case 0:   RoadFlag.circle=0; break;
                    case 1:   CarInfo.dirOffset = (int)(- (90 - LongRight) * Circle.CircleR50_RP); break;
                    //case 2:   CarInfo.dirOffset = (int)(- (48 - LongRight) * Circle.Circle_R60RIn); break;
        //            case 3:   CarInfo.dirOffset = (int)(- (38 - LongRight) * Circle.Circle_R80In); break;
        //            case 4:   CarInfo.dirOffset = (int)(- (40 - LongRight) * Circle.Circle_R100In); break;
                    default:break;
                }
        }
        if(CarInfo.dirOffset > 0)            CarInfo.dirOffset = -CarInfo.dirOffset;
    }
    else if(RoadFlag.circle == 8)
    {
        CarInfo.dirOffset = (L_boundary[DataInfo.forwardLook - 10] + R_boundary[DataInfo.forwardLook - 10] - MIDDLE)*1.2;
        if(CarInfo.dirOffset > -50)
        {
            CarInfo.dirOffset = -70;
        }
        // CarInfo.dirOffset = (MIDDLE - Left - Right)*2.0;
        // if(CarInfo.dirOffset > -70)
        // {
        //     CarInfo.dirOffset = -90;
        // }
    }
    else if(RoadFlag.circle == 10)
    {
        // 1. --- 寻找远端目标点 ---
        for(i = 0; i < COL; i ++)
        {
            if(WhiteCol[i] > 60)//25
            {
                COL_site = i;
                break;
            }
        }
        // 2. --- 寻找近端拐点 ---
        for(i = 5; i < LongLeft - 1; i ++)
        {
            if(MiddleLine[i] < MiddleLine[i + 1])
            {
                Angular = i;
                break;
            }
        }
        // 3. --- 分段补线逻辑 ---
        if(Angular >= DataInfo.forwardLook)
        {
            CarInfo.dirOffset = (int16)((L_boundary[DataInfo.forwardLook] - MIDDLE)*0.8);//0.8
        }
        else if(Angular > 0)
        {
            // ?? 优化 1：计算分母并进行安全保护，防止除以0或极小值导致斜率爆炸
            int16 delta_y = WhiteCol[COL_site] - Angular;
            if(delta_y < 5) delta_y = 5; 

            Slope = COL_site - L_boundary[Angular]; // 两点水平距离作差

            // 使用浮点数暂存真实斜率
            float k = (float)Slope / delta_y; 

            switch(static_cast<int>(Circle.CircleSize))
            {
                case 1: k *= Circle.CircleR100_P;   break;// 乘上环岛专属系数
                // case 2: k *= Circle.Circle_R60ROut;   break;
                // case 3: k *= Circle.Circle_R80Out;    break;
                // case 4: k *= Circle.Circle_R100Out;   break;
                default:break;
            }

            // ?? 优化 2：斜率限幅，防止远端点跳变导致算出来的线“横”在赛道上
            if(k > 2.5f)  k = 2.5f;
            if(k < -2.5f) k = -2.5f;

            Slope = k * (DataInfo.forwardLook - Angular); // 算出变化数

            if(fabs(Slope) > L_boundary[Angular])    L_boundary[DataInfo.forwardLook] = 0;
            else                                    L_boundary[DataInfo.forwardLook] = L_boundary[Angular] + (int)Slope;

            CarInfo.dirOffset = L_boundary[DataInfo.forwardLook] - MIDDLE;

            // ?? 优化 3：软性防呆与双向限幅，替换掉原先暴力的强制取反
            if(CarInfo.dirOffset > 15) 
            {
                CarInfo.dirOffset = 15;  // 【内切救车】：允许轻微往外回正，但限制幅度防止直冲出跑道
            }
            else if(CarInfo.dirOffset < -90) 
            {
                CarInfo.dirOffset = -90; // 【外抛限幅】：限制最大打舵幅度，防止舵机卡死或回直道时“画龙”
            }
        }
        else
        {
            // ?? 优化 1：同理，对无拐点状态的分母进行保护
            int16 delta_y = WhiteCol[COL_site] - LongLeft + 1;
            if(delta_y < 5) delta_y = 5;

            Slope = COL_site - L_boundary[LongLeft - 1]; 

            float k = (float)Slope / delta_y;

            switch(static_cast<int>(Circle.CircleSize))
            {
                case 1: k *= Circle.CircleR100_P; break; 
                // case 2: k *= Circle.Circle_R60ROut; break;
                // case 3: k *= Circle.Circle_R80Out;  break;
                // case 4: k *= Circle.Circle_R100Out; break;
            }
            if(k > 2.5f)  k = 2.5f;
            if(k < -2.5f) k = -2.5f;

            Slope = k * (DataInfo.forwardLook - LongLeft + 1); 

            if(fabs(Slope) > L_boundary[LongLeft - 1])    L_boundary[DataInfo.forwardLook] = 0;
            else                                          L_boundary[DataInfo.forwardLook] = L_boundary[LongLeft - 1] + (int)Slope;

            CarInfo.dirOffset = L_boundary[DataInfo.forwardLook] - MIDDLE;

            if(CarInfo.dirOffset > 15) 
            {
                CarInfo.dirOffset = 15;
            }
            else if(CarInfo.dirOffset < -90) 
            {
                CarInfo.dirOffset = -90;
            }
        }

    }
    else if(RoadFlag.circle==14)
    {
        CarInfo.dirOffset = ((L_boundary[55] - Standard_Haf_Road_Wide[55] - MIDDLE/2 ) * 1.0);  
        // if(LongLeft >= DataInfo.forwardLook)
        // {
        //     CarInfo.dirOffset = (int16)((L_boundary[DataInfo.forwardLook] - Left_Normal) );//2.0   
        //     // printf("1\n");         
        // }

        // else
        // {
        //     CarInfo.dirOffset = (int16)((MIDDLE - Left - Left_Normal) );      
        //     // printf("2\n");
        // }
        // if(LongRight >= DataInfo.forwardLook)
            // CarInfo.dirOffset = ((L_boundary[DataInfo.forwardLook] - Standard_Haf_Road_Wide[DataInfo.forwardLook]*0.7 - MIDDLE/2 ) * 1);
    // CarInfo.dirOffset = ((L_boundary[45] - Standard_Haf_Road_Wide[45]*0.8 - MIDDLE/2 ) * 1);
        // else
        //     CarInfo.dirOffset = (L_boundary[50] - Standard_Haf_Road_Wide[50]);
    }


/**************************************************左转控制*************************************************************/
/**********************************************************************************************************************/
    else if(RoadFlag.circle==1||RoadFlag.circle==3)
    //    CarInfo.dirOffset = (int16)(R_boundary[DataInfo.forwardLook] + Standard_Haf_Road_Wide[DataInfo.forwardLook] - MIDDLE/2 );
       CarInfo.dirOffset = (int16)(R_boundary[30] + Standard_Haf_Road_Wide[30] - MIDDLE/2 );
    else if(RoadFlag.circle==5)
    {
        if(LongLeft >= 60)
        {
            CarInfo.dirOffset = Right;
            if(CarInfo.dirOffset<25)
            {
                CarInfo.dirOffset = 80;
            }
        }
        else
        {
            // printf("Circle.Circle_R60LIn=%f\n",Circle.Circle_R60LIn);
            switch(static_cast<int>(Circle.CircleSize))
            {
                 case 0:   RoadFlag.circle=0; break;
                 case 1:   CarInfo.dirOffset = (int)((80 - LongLeft) * Circle.CircleR50_LP); break;
                //  case 2:   CarInfo.dirOffset = (int)((48 - LongLeft) * Circle.Circle_R60LIn); break;
                //  case 3:   CarInfo.dirOffset = (int)((38 - LongLeft) * Circle.Circle_R80In); break;
                //  case 4:   CarInfo.dirOffset = (int)((40 - LongLeft) * Circle.Circle_R100In); break;
                 default:break;
            }
        }

        if(CarInfo.dirOffset < 0)    CarInfo.dirOffset = -CarInfo.dirOffset;
    }

    else if(RoadFlag.circle==7)
    {
        CarInfo.dirOffset = (L_boundary[DataInfo.forwardLook - 10] + R_boundary[DataInfo.forwardLook - 10] - MIDDLE)*2;
        if(CarInfo.dirOffset < 50) //30
        {
                CarInfo.dirOffset = 90; //80
        }
    }
    else if(RoadFlag.circle == 9)
    // {
    //     // 1. --- 寻找远端目标点 (从右往左扫) ---
    //     for(i = COL - 1; i >= 0; i--)
    //     {
    //         if(WhiteCol[i] > 60)//22
    //         {
    //             COL_site = i;
    //             break;
    //         }
    //     }

    //     // 2. --- 寻找近端拐点 (左圆环判断逻辑相反，找大于) ---
    //     for(i = 5; i < LongRight - 1; i++)
    //     {
    //         if(MiddleLine[i] > MiddleLine[i + 1])
    //         {
    //             Angular = i;
    //             break;
    //         }
    //     }

    //     // 3. --- 分段补线逻辑 (镜像对称) ---
    //     if(Angular >= DataInfo.forwardLook)
    //     {
    //         // 拐点在前方，直接使用前瞻行的右边界计算偏差
    //         CarInfo.dirOffset = (int16)((R_boundary[DataInfo.forwardLook] - MIDDLE) * 0.8);
    //     }
    //     else if(Angular > 0)
    //     {
    //         // ?? 优化 1：分母安全保护，防止除零
    //         int16 delta_y = WhiteCol[COL_site] - Angular;
    //         if(delta_y < 5) delta_y = 5; 

    //         Slope = COL_site - R_boundary[Angular]; 

    //         // 使用浮点数暂存真实斜率
    //         float k = (float)Slope / delta_y; 

    //         switch(static_cast<int>(Circle.CircleSize))
    //         {
    //             case 1: k *= Circle.Speed_R50;  break; // 乘上左圆环专属系数
    //             // case 2: k *= Circle.Circle_R60LOut;  break;
    //             // case 3: k *= Circle.Circle_R80LOut;  break;
    //             // case 4: k *= Circle.Circle_R100LOut; break;
    //             default:break;
    //         }

    //         // ?? 优化 2：斜率限幅，防止补线“横切”赛道
    //         if(k > 2.5f)  k = 2.5f;
    //         if(k < -2.5f) k = -2.5f;

    //         Slope = k * (DataInfo.forwardLook - Angular); 

    //         // 右边界限幅保护（防止补线补到屏幕外去）
    //         int16 target_R = R_boundary[Angular] + (int)Slope;
    //         if(target_R >= COL - 1)  R_boundary[DataInfo.forwardLook] = COL - 1;
    //         else if(target_R < 0)    R_boundary[DataInfo.forwardLook] = 0;
    //         else                     R_boundary[DataInfo.forwardLook] = target_R;

    //         CarInfo.dirOffset = R_boundary[DataInfo.forwardLook] - MIDDLE;

    //         // ?? 优化 3：镜像软限幅 (右半图 R_boundary - MIDDLE 为正数)
    //         if(CarInfo.dirOffset < -15) 
    //         {
    //             CarInfo.dirOffset = -15; // 【内切救车】：限制左打角，防止车头过早扎入内弯
    //         }
    //         else if(CarInfo.dirOffset > 90) 
    //         {
    //             CarInfo.dirOffset = 90;  // 【外抛限幅】：限制右打角，防止出环时直接冲出右侧跑道
    //         }
    //     }
    //     else
    //     {
    //         // 无拐点状态
    //         // ?? 优化 1：同理进行分母保护
    //         int16 delta_y = WhiteCol[COL_site] - LongRight + 1;
    //         if(delta_y < 5) delta_y = 5;

    //         Slope = COL_site - R_boundary[LongRight - 1]; 

    //         float k = (float)Slope / delta_y;

    //         switch(static_cast<int>(Circle.CircleSize))
    //         {
    //             case 1: k *= Circle.Speed_R50; break; 
    //             // case 2: k *= Circle.Circle_R60LOut; break;
    //             // case 3: k *= Circle.Circle_R80LOut; break;
    //             // case 4: k *= Circle.Circle_R100LOut; break;
    //             default:break;
    //         }
            
    //         // ?? 优化 2：斜率双向限幅
    //         if(k > 2.5f)  k = 2.5f;
    //         if(k < -2.5f) k = -2.5f;

    //         Slope = k * (DataInfo.forwardLook - LongRight + 1); 

    //         // 右边界限幅保护
    //         int16 target_R = R_boundary[LongRight - 1] + (int)Slope;
    //         if(target_R >= COL - 1)  R_boundary[DataInfo.forwardLook] = COL - 1;
    //         else if(target_R < 0)    R_boundary[DataInfo.forwardLook] = 0;
    //         else                     R_boundary[DataInfo.forwardLook] = target_R;

    //         CarInfo.dirOffset = R_boundary[DataInfo.forwardLook] - MIDDLE;

    //         // ?? 优化 3：镜像软限幅
    //         if(CarInfo.dirOffset < -15) 
    //         {
    //             CarInfo.dirOffset = -15;
    //         }
    //         else if(CarInfo.dirOffset > 90) 
    //         {
    //             CarInfo.dirOffset = 90;
    //         }
    //     }
    // }
    // {
    //     for(i = COL-1; i > 0; i --)
    //     {
    //         if(WhiteCol[i] > 32)//22
    //         {
    //             COL_site = i;
    //             break;
    //         }
    //     }

    //     for(i = 5; i < LongRight - 1; i++){
    //         if(MiddleLine[i] > MiddleLine[i+1]){
    //             Angular = i;
    //             break;
    //         }
    //     }
    //     for(r=0;r<=35;r++)
    //     {
    //         for(c=100;c<198;c++)
    //         {
    //             if(Bmp[r][c]==255 &&Bmp[r][c+1]==0 && Bmp[r][c+2]==0)
    //             {
    //                 break;
    //             }
    //             else
    //             {
    //                 Circle_WhiteRow[r]++;
    //             }
    //         }
    //         for(c=100;c>2;c--)
    //         {
    //             if(Bmp[r][c]==255 && Bmp[r][c-1]==0 && Bmp[r][c-2]==0)
    //             {
    //                 break;
    //             }
    //             else
    //             {
    //                 Circle_WhiteRow[r]++;
    //             }
    //         }
    //         if(Circle_WhiteRow[r]>=175)
    //         {
    //             line++;
    //         }
    //     }

    //    ShowAngular = Angular;
    //     if(Angular >= DataInfo.forwardLook)
    //     {
    //             CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 1.05;//0.8
    //     }
    //     else if(Angular > 0)
    //     {
    //        Slope = COL_site - R_boundary[Angular]; //两点间距离作差 y-y0
    //        ShowSlope=Slope;

    //        switch(static_cast<int>(Circle.CircleSize))
    //        {
    //            case 1:Slope = (Slope / (WhiteCol[COL_site] - Angular)) * Circle.Speed_R50; break;//算出斜率 k=(y-y0)/(x-x0)
    //         //    case 2:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R60LOut; break;
    //         //    case 3:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R80Out; break;
    //         //    case 4:Slope = (Slope / (WhiteCol[COL_site] - Angular)) *Circle.Circle_R100Out; break;
    //        }
    //        Slope = Slope * (DataInfo.forwardLook - Angular); //算出变化数

    //        if(line>=10 && ShowSlope<=80.0)
    //       {
    //            switch(static_cast<int>(Circle.CircleSize))
    //            {
    //                case 1:Slope = ((COL-15)/ (19 - Angular)) * Circle.Speed_R50;break;
    //             //    case 2:Slope = ((COL-15)/ (19 - Angular))*Circle.Circle_R60LOut;break;
    //             //    case 3:Slope = ((COL-15)/ (19 - Angular))*Circle.Circle_R80Out;break;
    //             //    case 4:Slope = ((COL-15)/ (19 - Angular))*Circle.Circle_R100Out;break;
    //            }
    //       }

    //        if((R_boundary[Angular] + (int)Slope) >= COL - 1)
    //            R_boundary[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
    //        else
    //        {
    //            R_boundary[DataInfo.forwardLook] = R_boundary[Angular] + (int)Slope;
    //            if(R_boundary[DataInfo.forwardLook]<110 && line>=10 && ShowSlope<=80.0 )
    //            {
    //                R_boundary[DataInfo.forwardLook]=COL-1;
    //            }
    //        }
    //            CarInfo.dirOffset = (189 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 0.95;
    //            if(CarInfo.dirOffset<20)    CarInfo.dirOffset = 50;
    //    }
    //     else{


    //         Slope = COL_site - R_boundary[LongRight - 1]; //两点间距离作差 y-y0

    //         switch(static_cast<int>(Circle.CircleSize))
    //         {
    //             case 1:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Speed_R50; break;//算出斜率 k=(y-y0)/(x-x0)
    //             // case 2:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R60LOut; break;
    //             // case 3:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R80Out; break;
    //             // case 4:Slope = (Slope / (WhiteCol[COL_site] - LongRight + 1)) *Circle.Circle_R100Out; break;
    //         }
    //         Slope = (Slope * (DataInfo.forwardLook - LongRight + 1)); //算出变化数



    //         if((R_boundary[Angular] + (int)Slope) >= COL - 1)
    //             R_boundary[DataInfo.forwardLook] = COL - 1; //将此函数作为右边界，相当于补线
    //         else
    //             R_boundary[DataInfo.forwardLook] = R_boundary[LongRight - 1] + (int)Slope;

    //         if(Circle.CircleSize >= 2)
    //         {
    //             CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 0.85;//0.85
    //             if( CarInfo.dirOffset > 85)            CarInfo.dirOffset = 48;
    //             else if(CarInfo.dirOffset < 20)        CarInfo.dirOffset = 40;
    //         }
    //         else if(Circle.CircleSize == 1)
    //             CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 1;
    //             if(CarInfo.dirOffset < 0)        CarInfo.dirOffset = -CarInfo.dirOffset;
    //             else if(CarInfo.dirOffset < 20)        CarInfo.dirOffset = 40;



    //     }
    // }
    {
        // ? 1. 变量初始化，防止继承上一帧的脏数据
        COL_site = 0;
        Angular = 0;
        line = 0;

        // 从远到近找特征点，倒装架构下 i越大越远
        for(i = COL-1; i > 0; i--)
        {
            if(WhiteCol[i] > 32)
            {
                COL_site = i;
                break;
            }
        }

        // ? 2. 保护安全边界，防止下标为负引发 Bus error
        int safe_LongRight = LongRight;
        if(safe_LongRight < 2) safe_LongRight = 2; 

        // 找拐点
        for(i = 5; i < safe_LongRight - 1; i++){
            if(MiddleLine[i] > MiddleLine[i+1]){
                Angular = i;
                break;
            }
        }

        // ? 3. 修复大宽道死循环累加 BUG：每次循环必须清零！
        for(r = 0; r <= 35; r++)
        {
            Circle_WhiteRow[r] = 0; // 【关键修复：不清零会假死】
            
            for(c = 100; c < 198; c++)
            {
                if(Bmp[r][c]==255 && Bmp[r][c+1]==0 && Bmp[r][c+2]==0) break;
                else Circle_WhiteRow[r]++;
            }
            for(c = 100; c > 2; c--)
            {
                if(Bmp[r][c]==255 && Bmp[r][c-1]==0 && Bmp[r][c-2]==0) break;
                else Circle_WhiteRow[r]++;
            }
            if(Circle_WhiteRow[r] >= 175) 
            {
                line++;
            }
        }

        ShowAngular = Angular;

        if(Angular >= DataInfo.forwardLook)
        {
            CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 1.05;
        }
        else if(Angular > 0)
        {
            float temp_Slope = COL_site - R_boundary[Angular]; 
            ShowSlope = temp_Slope;

            // ? 4. 修复除 0 风险
            int dy = WhiteCol[COL_site] - Angular;
            if (dy == 0) dy = 1;

            switch(static_cast<int>(Circle.CircleSize))
            {
                // ? 5. 修复整数相除变为 0 的 BUG，强制转换为浮点运算
                case 1: temp_Slope = (temp_Slope / (float)dy) * Circle.Speed_R50; break;
            }
            
            temp_Slope = temp_Slope * (DataInfo.forwardLook - Angular); 

            if(line >= 10 && ShowSlope <= 80.0)
            {
                // ? 6. 修复倒装架构下 Angular>19 导致车反向扎入圆环内岛的死亡 BUG
                int den2 = 19 - Angular;
                if(den2 <= 0) den2 = 1; // 强制为正数，保证线是往右扩的

                switch(static_cast<int>(Circle.CircleSize))
                {
                    case 1: temp_Slope = ((COL - 15.0f) / (float)den2) * Circle.Speed_R50; break;
                }
            }

            int target_R = R_boundary[Angular] + (int)temp_Slope;

            if(target_R >= COL - 1)
                R_boundary[DataInfo.forwardLook] = COL - 1; 
            else
            {
                R_boundary[DataInfo.forwardLook] = target_R;
                if(R_boundary[DataInfo.forwardLook] < 110 && line >= 10 && ShowSlope <= 80.0)
                {
                    R_boundary[DataInfo.forwardLook] = COL - 1;
                }
            }
            
            CarInfo.dirOffset = (189 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 0.95;
            if(CarInfo.dirOffset < 20) CarInfo.dirOffset = 50;
        }
        else 
        {
            float temp_Slope = COL_site - R_boundary[safe_LongRight - 1]; 

            // 分母保护
            int dy = WhiteCol[COL_site] - safe_LongRight + 1;
            if(dy == 0) dy = 1;

            switch(static_cast<int>(Circle.CircleSize))
            {
                case 1: temp_Slope = (temp_Slope / (float)dy) * Circle.Speed_R50; break;
            }
            
            temp_Slope = temp_Slope * (DataInfo.forwardLook - safe_LongRight + 1); 

            int target_R = R_boundary[safe_LongRight - 1] + (int)temp_Slope;

            if(target_R >= COL - 1)
                R_boundary[DataInfo.forwardLook] = COL - 1; 
            else
                R_boundary[DataInfo.forwardLook] = target_R;

            if(Circle.CircleSize >= 2)
            {
                CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 0.85;
                if( CarInfo.dirOffset > 85)            CarInfo.dirOffset = 48;
                else if(CarInfo.dirOffset < 20)        CarInfo.dirOffset = 40;
            }
            else if(Circle.CircleSize == 1)
            {
                CarInfo.dirOffset = (COL-1 + R_boundary[DataInfo.forwardLook] - MIDDLE) * 1.0;
                if(CarInfo.dirOffset < 0) 
                    CarInfo.dirOffset = -CarInfo.dirOffset;
                else if(CarInfo.dirOffset < 20) 
                    CarInfo.dirOffset = 40;
            }
        }
    }
    else if(RoadFlag.circle==13)
    {
    CarInfo.dirOffset = (int16)(R_boundary[55] + Standard_Haf_Road_Wide[55] - MIDDLE/2 );
        // if(LongRight >= DataInfo.forwardLook)
        // {
        //     CarInfo.dirOffset = (int16)((R_boundary[DataInfo.forwardLook] - Right_Normal) );//2.0   
        //     // printf("1\n");         
        // }
        // else
        // {
        //     CarInfo.dirOffset = (int16)((MIDDLE - Right - Right_Normal) );      
        //     // printf("2\n");
        // }
    }
}

