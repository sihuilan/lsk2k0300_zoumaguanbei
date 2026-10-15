#include "zf_common_headfile.h"

std::atomic<bool> Go_Flag(true);
//uint8 StopCar = FALSE;
uint8 gotime;
uint8 LastSummation;
uint8 endSummation;
uint8 endSummation1;
float Offset1,Offset2;


//-------------------------------------------------------------------------------------------------------------------
//  @brief      AI绕行控制
//-------------------------------------------------------------------------------------------------------------------

// 获取右边界相对于图像中心的平均偏差（像素）
int16 GetRightOffset(void)
{
    int16 sum = 0, cnt = 0;
    for (int r = DataInfo.forwardLook - 2; r <= DataInfo.forwardLook + 2; r++) {
        if (R_boundary[r] > 5 && R_boundary[r] < COL - 5) {
            sum += (int16)(R_boundary[r] - MIDDLE/2);
            cnt++;
        }
    }

    static int16 last = 0;
    static uint8_t loss_cnt = 0;

    if (cnt == 0) {
        loss_cnt++;
        if (loss_cnt == 1)      return (int16)(last * 0.8f);//0.8 //可行 这样就是为了防止看不到边界一直右转 出赛道
        else if (loss_cnt == 2) return (int16)(last * 0.4f);//0.5
        else                    return 40;
    } else {
        loss_cnt = 0;
        last = sum / cnt;
        return last;
    }
}
// 获取左边界相对于图像中心的平均偏差（像素）
// 返回值：正 = 左边界在中心右侧（车偏左），负 = 左边界在中心左侧（车偏右）
int16 GetLeftOffset(void)
{
    int16 sum = 0, cnt = 0;
    for (int r = DataInfo.forwardLook - 2; r <= DataInfo.forwardLook + 2; r++) {
        if (L_boundary[r] > 5 && L_boundary[r] < COL - 5) {
            sum += (int16)(L_boundary[r] - MIDDLE/2);   // 左边界列 - 中心列
            cnt++;
        }
    }
     static int16 last = 0;
    static uint8_t loss_cnt = 0;

    if (cnt == 0) {
        loss_cnt++;
        if (loss_cnt == 1)      return (int16)(last * 0.6f);  // 第一次丢失：保留70%力度
        else if (loss_cnt == 2) return (int16)(last * 0.3f);  // 第二次：保留40%
        else                    return -5;                    // 第三次及以后：给一个很小的左转（防止直行偏右）
    } else {
        loss_cnt = 0;
        last = sum / cnt;
        return last;
    }
}
void AIControl(void)
{
// ===== 左转绕行（寻左边界） =====//OK!!
if (RoadFlag.AI == 1) {
    int16 offset = GetLeftOffset();
    int16 target = 32;           // 目标偏移：希望左边界在中心右侧20像素（车偏左） 30
    int16 error = target + offset;   // 当 offset 小于 target 时误差为正，左转
    int16 turn = (int16)(1.3 * error);   // 比例系数0.9

    CarInfo.dirOffset = LIMIT(turn, -90, 90);   // 左转为正值，限幅0~90
    if (RoadFlag.AIDis >= 0.60)//0.63
     {   
        RoadFlag.AI = 0;
        RoadFlag.Red = 0;
    }
}

// ===== 右转绕行（寻右边界） =====
else if (RoadFlag.AI == 2) {
    int16 offset = GetRightOffset();
    int16 target = RoadFlag.inside_CrossRound ? 0 : -29; //急救包1 眼镜也可 加了右转保护不出界 //-27
    int16 error = offset + target;
    int16 turn = (int16)(0.9f * error);
    CarInfo.dirOffset = LIMIT(turn, -90, 90);
    // 直接在这里判断退出，不需要转 AI=4
    if (RoadFlag.inside_CrossRound) {
        // 十字/环岛：需要看到右边界才退出
        if (RoadFlag.AIDis >= 1.0f && Right > 50) {//急救包 0.9 50
            RoadFlag.AI = 0;
            RoadFlag.Red = 0;
        }
        else if (RoadFlag.AIDis >= 1.4f) { //急救包 1.3
            RoadFlag.AI = 0;
            RoadFlag.Red = 0;
        }
    }
    else {
        // 直道：定距直接退出
        if (RoadFlag.AIDis > 0.60f) {//快速回正就缩短距离 //0.62
            RoadFlag.AI = 0;
            RoadFlag.Red = 0;
        }
    }
}
}

// void CrossControl(void)
// {
//     Cross_check();
//     cross_fill(Bmp,L_boundary, R_boundary, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
// }
// void AIControl(void)
// {
//     if(RoadFlag.AI == 1)
//     {
//         CarInfo.dirOffset = -80;    // 往外拐的偏差 (注意根据左绕还是右绕调整正负)
//         BuzzerMs(50);               // 短鸣笛提示进入绕行
//         RoadFlag.AIDis = 0;      
//         RoadFlag.AI = 3;            // 进入循边界状态
//     }
    
//     // 状态3：循着边界走一小段
//     else if(RoadFlag.AI == 3 && abs(RoadFlag.AIAngle) > 25)
//     {
        
//         //边界循线逻辑 

//         // 走一小段距离后（比如 0.8米），准备切回赛道
//         if(RoadFlag.AIDis > 0.40)   // 距离阈值写入flash方便调参
//         {
//             RoadFlag.AI = 5;        // 进入往回打状态
//         }
//     }
    
//     // 状态5：往赛道内打一帧，并退出正常寻中线
//     else if(RoadFlag.AI == 5)
//     {
//         CarInfo.dirOffset = 80;     // 往赛道内打的偏差（与状态1反向）
//         BuzzerMs(100);              // 长鸣笛提示结束绕行
//         RoadFlag.AI = 0;            // 退出绕行标志位，下一帧系统会自动回到正常的寻中线逻辑
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
//                CarInfo.dirOffset = (int16)((L_boundary[DataInfo.forwardLook] - Left_Normal) * 2.0);
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
//  @brief      坡道控制
//-------------------------------------------------------------------------------------------------------------------
void RampControl(void)
{
    uint8 i = 0;
    int IT_offset = 0;

    for (i = 0;i < 4;i++)//4 12
    {
        IT_offset +=  (L_boundary[i] + R_boundary[i] - MIDDLE);
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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      十字控制
//-------------------------------------------------------------------------------------------------------------------

void CrossControl(void)
{
    uint8 i;
    float L_k=0,L_b=0,R_k=0,R_b=0; 

// //角点补线法
//补线
    if(RoadFlag.cross==1)//&& l_down[1] >= 0 && r_down[1] >= 0
    {
        //左侧补线
        if(l_up[1]!=l_down[1])
        L_k = static_cast<float>(l_up[0]-l_down[0])/ (l_up[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up[1]==l_down[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=l_down[1]+1;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
        for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);  //暂时只补了用于offset计算的两行，即是为了减少运算时间，也是为了预防一下过度补线
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_up[1]!=r_down[1])
        R_k = static_cast<float>(r_up[0]-r_down[0])/ (r_up[1]-r_down[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_up[1]==r_down[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=r_down[1]+1;i<r_up[1];i++)
        {
             R_boundary[i]=static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==2)
    {   
        //左侧补线
        if(l_up_1[1]!=l_up[1])
        L_k = static_cast<float>(l_up_1[0]-l_up[0])/ (l_up_1[1]-l_up[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up_1[1]==l_up[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=0;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
          for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;

        //右侧补线
        if(r_up_1[1]!=r_up[1])
        R_k = static_cast<float>(r_up_1[0]-r_up[0])/ (r_up_1[1]-r_up[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_up_1[1]==r_up[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=0;i<r_up[1];i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
        
    }
    else if(RoadFlag.cross==3)
    {   
        //左侧补线
        if(l_down[1]!=l_down_1[1])
        L_k = static_cast<float>(l_down_1[0]-l_down[0])/ (l_down_1[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_down[1]==l_down_1[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_down[0]-L_k*l_down[1];
         for(i=l_down[1]+1;i<ROW;i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);    
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
        // L_boundary[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // L_boundary[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(L_boundary[DataInfo.forwardLook-2]>COL-1 || L_boundary[DataInfo.forwardLook-2]<0)
        // L_boundary[DataInfo.forwardLook-2]=COL-1;

        // if(L_boundary[DataInfo.forwardLook+2]>COL-1 || L_boundary[DataInfo.forwardLook+2]<0)
        // L_boundary[DataInfo.forwardLook+2]=COL-1;

        //右侧补线
        if(r_down[1]!=r_down_1[1])
        R_k = static_cast<float>(r_down[0]-r_down_1[0])/ (r_down[1]-r_down_1[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_down[1]==r_down_1[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_down[0]-R_k*r_down[1];
         for(i=r_down[1]+1;i<ROW;i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
        
    }
    else if(RoadFlag.cross==4)//&& l_down[1] >= 0 && r_down[1] >= 0
    {
        //左侧补线
        if(l_up[1]!=l_down[1])
        L_k = static_cast<float>(l_up[0]-l_down[0])/ (l_up[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up[1]==l_down[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=l_down[1]+1;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
          for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // L_boundary[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);  //暂时只补了用于offset计算的两行，即是为了减少运算时间，也是为了预防一下过度补线
        // L_boundary[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(L_boundary[DataInfo.forwardLook-2]>COL-1 || L_boundary[DataInfo.forwardLook-2]<0)
        // L_boundary[DataInfo.forwardLook-2]=COL-1;
        // if(L_boundary[DataInfo.forwardLook+2]>COL-1 || L_boundary[DataInfo.forwardLook+2]<0)
        // L_boundary[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_up_1[1]!=r_up[1])
        R_k = static_cast<float>(r_up_1[0]-r_up[0])/ (r_up_1[1]-r_up[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_up_1[1]==r_up[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=0;i<r_up[1];i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==5)
    {
        //左侧补线
        if(l_up_1[1]!=l_up[1])
        L_k = static_cast<float>(l_up_1[0]-l_up[0])/ (l_up_1[1]-l_up[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up_1[1]==l_up[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=0;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
           for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }       
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_up[1]!=r_down[1])
        R_k = static_cast<float>(r_up[0]-r_down[0])/ (r_up[1]-r_down[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_up[1]==r_down[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=r_down[1]+1;i<r_up[1];i++)
        {
             R_boundary[i]=static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }        
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==6)
    {
          //左侧补线
        if(l_up[1]!=l_down[1])
        L_k = static_cast<float>(l_up[0]-l_down[0])/ (l_up[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up[1]==l_down[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=l_down[1]+1;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
          for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);  //暂时只补了用于offset计算的两行，即是为了减少运算时间，也是为了预防一下过度补线
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_down[1]!=r_down_1[1])
        R_k = static_cast<float>(r_down[0]-r_down_1[0])/ (r_down[1]-r_down_1[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_down[1]==r_down_1[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_down[0]-R_k*r_down[1];
         for(i=r_down[1]+1;i<ROW;i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==7)
    {
     //左侧补线
        if(l_down[1]!=l_down_1[1])
        L_k = static_cast<float>(l_down_1[0]-l_down[0])/ (l_down_1[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_down[1]==l_down_1[1])
     //      L_k=CarInfo.last_Lk;

        L_b=l_down[0]-L_k*l_down[1];
        
         for(i=l_down[1]+1;i<ROW;i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);    
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;

        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
           //右侧补线
  //右侧补线
        if(r_up[1]!=r_down[1])
        R_k = static_cast<float>(r_up[0]-r_down[0])/ (r_up[1]-r_down[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     // printf("r_up[0]-r_down[0]=%d\r\n",r_up[0]-r_down[0]);
     // printf("r_up[1]-r_down[1]=%d\r\n",r_up[1]-r_down[1]);

     //    printf("右下角点: (%d, %d)\n",  r_down[0], r_down[1]);
     //    printf("右上角点: (%d, %d)\n",  r_up[0], r_up[1]);
     //    printf("R_k=%f\r\n",R_k);
     //    else if(R_k<0.0f || r_up[1]==r_down[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=r_down[1]+1;i<r_up[1];i++)
        {
             R_boundary[i]=static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==8)//&& l_down[1] >= 0 && r_down[1] >= 0
    {
        //左侧补线
        if(l_up[1]!=l_down[1])
        L_k = static_cast<float>(l_up[0]-l_down[0])/ (l_up[1]-l_down[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
     //    else if(L_k>0.0f || l_up[1]==l_down[1])
     //      L_k=CarInfo.last_Lk;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=l_down[1]+1;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
          for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);  //暂时只补了用于offset计算的两行，即是为了减少运算时间，也是为了预防一下过度补线
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
       
    }
     else if(RoadFlag.cross==9)//&& l_down[1] >= 0 && r_down[1] >= 0
    {
        //右侧补线
        if(r_up[1]!=r_down[1])
        R_k = static_cast<float>(r_up[0]-r_down[0])/ (r_up[1]-r_down[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
     //    else if(R_k<0.0f || r_up[1]==r_down[1])
     //      R_k=CarInfo.last_Rk;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=r_down[1]+1;i<r_up[1];i++)
        {
             R_boundary[i]=static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
        else if(RoadFlag.cross==10)
    {
        //左侧补线
        if(l_down[1]!=l_down_1[1])
        L_k = static_cast<float>(l_down[0]-l_down_1[0])/ (l_down[1]-l_down_1[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
        L_b=l_down[0]-L_k*l_down[1];
         for(i=l_down[1]+1;i<ROW;i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);    
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;

        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_up_1[1]!=r_up[1])
        R_k = static_cast<float>(r_up_1[0]-r_up[0])/ (r_up_1[1]-r_up[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
        R_b=r_up[0]-R_k*r_up[1];
         for(i=0;i<r_up[1];i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
          for(i=r_up[1];i<=Search_Stop_Line;i++)
        {
          R_boundary[i]=RightBlack1[i];
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
    else if(RoadFlag.cross==11)
    {
        //左侧补线
        if(l_up_1[1]!=l_up[1])
        L_k = static_cast<float>(l_up_1[0]-l_up[0])/ (l_up_1[1]-l_up[1]);
        else
        L_k=0;
        if(L_k>0.0f)
        return;
        L_b=l_up[0]-L_k*l_up[1];
         for(i=0;i<l_up[1];i++)
        {
             L_boundary[i]=static_cast<uint8>(i*L_k+L_b);        
             if(L_boundary[i]>border_max || L_boundary[i]<border_min)
             L_boundary[i]=border_max;
        }
          for(i=l_up[1];i<=Search_Stop_Line;i++)
        {
          L_boundary[i]=LeftBlack1[i];
        }
        // LeftBlack[DataInfo.forwardLook-2]=static_cast<uint8>(L_k*(DataInfo.forwardLook-2)+L_b);
        // LeftBlack[DataInfo.forwardLook+2]=static_cast<uint8>(L_k*(DataInfo.forwardLook+2)+L_b);
        // if(LeftBlack[DataInfo.forwardLook-2]>COL-1 || LeftBlack[DataInfo.forwardLook-2]<0)
        // LeftBlack[DataInfo.forwardLook-2]=COL-1;
        // if(LeftBlack[DataInfo.forwardLook+2]>COL-1 || LeftBlack[DataInfo.forwardLook+2]<0)
        // LeftBlack[DataInfo.forwardLook+2]=COL-1;
        //右侧补线
        if(r_down[1]!=r_down_1[1])
        R_k = static_cast<float>(r_down[0]-r_down_1[0])/ (r_down[1]-r_down_1[1]);
        else
        R_k=0;
        if(R_k<0.0f)
        return;
        R_b=r_down[0]-R_k*r_down[1];
         for(i=r_down[1]+1;i<ROW;i++)
        {
             R_boundary[i]= static_cast<uint8>(i*R_k+R_b);
             if(R_boundary[i]>border_max || R_boundary[i]<border_min)
             R_boundary[i]=border_min;
        }
        // RightBlack[DataInfo.forwardLook-2]=static_cast<uint8>(R_k*(DataInfo.forwardLook-2)+R_b);
        // RightBlack[DataInfo.forwardLook+2]=static_cast<uint8>(R_k*(DataInfo.forwardLook+2)+R_b);        
        // if(RightBlack[DataInfo.forwardLook-2]>COL-1 || RightBlack[DataInfo.forwardLook-2]<0)
        // RightBlack[DataInfo.forwardLook-2]=0;
        // if(RightBlack[DataInfo.forwardLook+2]>COL-1 || RightBlack[DataInfo.forwardLook+2]<0)
        // RightBlack[DataInfo.forwardLook+2]=0;
    }
     //     if(L_k>0.0f)
     //     CarInfo.last_Lk=L_k;
     //     if(R_k<0.0f)
     //     CarInfo.last_Rk=R_k;
        // for(i = 0; i < ROW; i++)
        // {
        //     MiddleLine[i] = (L_boundary[i] + RightBlack[i]) >> 1;
        // }
     // if(RoadFlag.cross==8)
     // {
     //      CarInfo.dirOffset =((2*LeftBlack[DataInfo.forwardLook+2] - - MIDDLE) + (2*LeftBlack[DataInfo.forwardLook-2] -  - MIDDLE)) / 2;
     // }
     // else if(RoadFlag.cross==9)
     // {
     //      CarInfo.dirOffset =((2*RightBlack[DataInfo.forwardLook+2] + - MIDDLE) + (2*RightBlack[DataInfo.forwardLook-2] +  - MIDDLE)) / 2;
     // }
     // else
     // {
     // CarInfo.dirOffset =((LeftBlack[DataInfo.forwardLook+2] + RightBlack[DataInfo.forwardLook+2] - MIDDLE) + (LeftBlack[DataInfo.forwardLook-2] + RightBlack[DataInfo.forwardLook-2] - MIDDLE)) / 2;
     // CarInfo.dirOffset =((LeftBlack[DataInfo.forwardLook+4] + RightBlack[DataInfo.forwardLook+4] - MIDDLE) + (LeftBlack[DataInfo.forwardLook+10] + RightBlack[DataInfo.forwardLook+10] - MIDDLE)+ (LeftBlack[DataInfo.forwardLook-2] + RightBlack[DataInfo.forwardLook-2] - MIDDLE) + (LeftBlack[DataInfo.forwardLook-8] + RightBlack[DataInfo.forwardLook-8] - MIDDLE)) / 4;
    //  CarInfo.dirOffset =((LeftBlack[DataInfo.forwardLook] + RightBlack[DataInfo.forwardLook] - MIDDLE) + (LeftBlack[DataInfo.forwardLook-6] + RightBlack[DataInfo.forwardLook-6] - MIDDLE))/2;    
     CarInfo.dirOffset =((L_boundary[DataInfo.forwardLook] + R_boundary[DataInfo.forwardLook] - MIDDLE)*0.7 + (L_boundary[DataInfo.forwardLook-6] + R_boundary[DataInfo.forwardLook-6] - MIDDLE)*0.3);    
    // }
     //     RoadFlag.Last_cross=1;
     
}
