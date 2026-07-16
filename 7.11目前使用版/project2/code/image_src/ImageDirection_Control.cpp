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
    {
        AIControl();
        return;
    }
   else if(RoadFlag.BrickDetected)
   {
    RedBlock_control();
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
        if(LongLeft > 85 && LongRight > 85)
        {
            CarInfo.dirOffset =(L_boundary[70] + R_boundary[70] - MIDDLE)*0.3+ (L_boundary[60] + R_boundary[60] - MIDDLE)*0.7 + (L_boundary[24] + R_boundary[24] - MIDDLE)*0;  //现在使用的两行前瞻为42和36
        }

        //左右边界长度均大于前瞻行，无需做任何特殊处理
        else if(LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook)
            CarInfo.dirOffset = (L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.7 + (L_boundary[DataInfo.forwardLook - 6] + R_boundary[DataInfo.forwardLook - 6] - MIDDLE)*0.3;
        // else if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //右边丢线向右转
        // {
        //     CarInfo.dirOffset =(L_boundary[35] + R_boundary[35] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4 ;
        // }
        // else if(LongLeft > LongRight && LongLeft <= 65 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE)        //左边丢线拉近向左转
        // {
        //     CarInfo.dirOffset =(L_boundary[35] + R_boundary[35] - MIDDLE)*0.6 + (L_boundary[LongLeft - 3] + R_boundary[LongLeft - 3] - MIDDLE)*0.4;
        // }
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
}
void imageDirectionPID(void)
{
    float diff = 0.0;
    float DirP = 0;//400/90
    float DirD = 0.0;
    imageDirectionControl();
      //  if (RoadFlag.BrickDetected) {
        // printf("Brick offset applied: before=%d, offset=%d\n", CarInfo.dirOffset, RoadFlag.BrickOffset);
      //  CarInfo.dirOffset += RoadFlag.BrickOffset;
        // CarInfo.dirOffset = LIMIT(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
        // printf("After: %d\n", CarInfo.dirOffset);
  //  }

    if(RoadFlag.circle || RoadFlag.inside_CrossRound == 1)
    {
        DirP = DataInfo.Ring_CurP; 
        DirD = DataInfo.Ring_CurD;
    }
    else if(!RoadFlag.circle && fabs(CarInfo.dirOffset)<=45 && fabs(CarInfo.dirOffset)>=15)
    {
       DirP =DataInfo.image_bigP;
       DirD =DataInfo.image_bigD;
    }    
    else if(!RoadFlag.circle && fabs(CarInfo.dirOffset)>45)
    {
        DirP = DataInfo.image_superP;
        DirD = DataInfo.image_superD;
    }
    else
    {
        DirP = DataInfo.image_Str_P; // 直道
        DirD = DataInfo.image_Str_D;   
    }

    if(std::fabs(CarInfo.dirOffset)<=60 && !RoadFlag.AI)
    {    
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 10, -10);//0,1.6,0.1,-0.1 1.5 2&& RoadFlag.inside_CrossRound == 0
        direD1 = ChangeLimit(DirD, direD1, 45, -45);// 5
    }
    else if(std::fabs(CarInfo.dirOffset)>60 && !RoadFlag.AI )
    {
    //限幅，防止PD变化过大引起抖动
        direP1 = ChangeLimit(DirP, direP1, 45, -45);//0,1.6,0.1,-0.1 1.5 2&& RoadFlag.inside_CrossRound == 0
        direD1 = ChangeLimit(DirD, direD1, 80, -80);// 5        
    } 
    // else
    // {
    //     direP1 = DirP;
    //     direD1 = DirD;
    // } 
    else if(RoadFlag.AI)
   {
    if (RoadFlag.AI == 1 || RoadFlag.AI == 3) {
        // 左转绕行：用更大的 P
        direP1 = 2.2f;      
        direD1 = 0.0f;
    } else {
        // 右转绕行：用原来的参数
        direP1 = DataInfo.AI_CurP_R;   // ← 1.7
        direD1 = DataInfo.AI_CurD_R;
    }
  }

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
