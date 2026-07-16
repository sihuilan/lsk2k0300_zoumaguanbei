#include "zf_common_headfile.h"

std::atomic<bool> Go_Flag(true);
//uint8 StopCar = FALSE;
uint8 gotime;
uint8 LastSummation;
uint8 endSummation;
uint8 endSummation1;
float Offset1,Offset2;

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
    int16 target = RoadFlag.inside_CrossRound ? 0 : -28; //急救包1 眼镜也可 加了右转保护不出界 //-27
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
void RedBlock_control(void) 
{
    if (RoadFlag.RedBlockDis >= 1.0f) {
        RoadFlag.BrickDetected = 0;
        RoadFlag.BrickOffset = 0;
        RoadFlag.RedBlockDis = 0;
        RoadFlag.Red =0;
    }
}   
void ZebraControl(void)
{
    if(RoadFlag.zebra ==1)
   {
        CarInfo.dirOffset = (int16)( RightBlack[DataInfo.forwardLook] +LeftBlack[DataInfo.forwardLook]-MIDDLE);
        if(fabs(CarInfo.dirOffset)>10)  CarInfo.dirOffset=0;
   }
}
// else if (RoadFlag.AI == 2) {
//     int16 offset = GetRightOffset();
//     int16 target = RoadFlag.inside_CrossRound ? 5: -27;//-5
//     int16 error = offset + target;
//     int16 turn = (int16)(1.0f * error);
    
//     // 始终看到右边界：如果右边界快丢了，停止右转或微左转
//     if (Right <= 50) {
//         // 右边界丢失或太边缘，停止右转
//         if (turn < 0) turn = 30;  // 禁止右转
//         // 或者微左转拉回来：turn = 10;
//     }
    
//     CarInfo.dirOffset = LIMIT(turn, -90, 90);
    
//     // 定距退出
//     if (RoadFlag.AIDis > 0.87f) && Right > 30 {//0.62
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//         // 不清零 dirOffset，让巡线自然接管
//     }
// }

// // ===== 右转绕行（寻右边界） =====
// else if (RoadFlag.AI == 2) {
//  int16 offset = GetRightOffset();
//  int16 target =0;
//  if(RoadFlag.inside_CrossRound)
//  {
//     target = -5;   //十字的参数
//  }
//    else{
//     target = -20;   
//    }        // 目标偏移（负值表示车偏右）
//     int16 error = offset + target;

//     int16 turn = (int16)(0.9f * error);  // 比例系数0.8
//     CarInfo.dirOffset = LIMIT(turn, -90, 90);
//     if (RoadFlag.AIDis > 0.54) {//试试改成0.6
//         RoadFlag.AI = 4;
//     }
// }
// else if (RoadFlag.AI == 4) {
//     if (LongRight < 50 || LongLeft < 5) {
//         // 十字：边界极短，大力回正
//         CarInfo.dirOffset = 74;
//     }
//     else {
//         // 直道：边界正常，标准回正
//         CarInfo.dirOffset = 46;
//     }
//   //  printf("AI4: R=%d L=%d LR=%d LL=%d dir=%d\n", 
//      //      Right, Left, LongRight, LongLeft, CarInfo.dirOffset);
    
//     RoadFlag.AI = 6;
// }
// else if(RoadFlag.AI == 6)
// {
//     // 看到两边且距离够 → 退出
//     if(Left != border_min && Right != border_max && RoadFlag.AIDis >= 0.71f) {
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     // 超长保底
//     else if(RoadFlag.AIDis >= 0.85f) {
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
// } 
// else if (RoadFlag.AI == 4) {
//      CarInfo.dirOffset = 46;       // 固定左转回正
//     if (RoadFlag.AIDis > 0.55) {
//         // RoadFlag.AI = 10;
//           RoadFlag.Red = 0;
//         RoadFlag.AI = 0;
//     }
// }
// else if (RoadFlag.AI == 4) {
//     int branch = 0;  // 0=60, 1=45, 2=50
    
//     if (Right == border_max || LongRight < 5) {
//         CarInfo.dirOffset = 60;
//         branch = 0;
//     }
//     else if (Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook) {
//         CarInfo.dirOffset = 45;
//         branch = 1;
//     }
//     else {
//         CarInfo.dirOffset = 50;
//         branch = 2;
//     }
    
//     // 每次进入 AI==4 只打印一次
//     static int last_branch = -1;
//     if (branch != last_branch) {
//         printf("AI4: branch=%d (R=%d, L=%d, LR=%d, LL=%d) -> dir=%d\n",
//                branch, Right, Left, LongRight, LongLeft, CarInfo.dirOffset);
//         last_branch = branch;
//     }
//     RoadFlag.AI = 6;
// }
// else if(RoadFlag.AI == 6)
// {
//     // 看到两边且距离够 → 退出
//     if(Left != border_min && Right != border_max && RoadFlag.AIDis >= 0.6f) {
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     // 超长保底
//     else if(RoadFlag.AIDis >= 0.70f) {
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
// } 

// else if(RoadFlag.AI == 4)
// {
//     if(Right == border_max || LongRight < 5) {
//         CarInfo.dirOffset = 60;
//     }
//     else if(Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook) {
//         CarInfo.dirOffset = 45;
//     }
//     else {
//         CarInfo.dirOffset = 50;
//     }
//     RoadFlag.AI = 6;

// }
// else if(RoadFlag.AI == 6)
// {
//     if(RoadFlag.AIDis >= 0.55f)
//     {
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
// } 
// else if (RoadFlag.AI == 10 && fabs(RoadFlag.AIAngle) > 15) {
//     // 退出条件（保持原样，但将 -40 改为 30 微调左转）
//     if (Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     else if (Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     else if (Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
// }
// else if (RoadFlag.AI == 8) {
//     CarInfo.dirOffset = 90;       // 固定左转回正
//     if (RoadFlag.AIDis > 0.68) {
//         RoadFlag.AI = 10;
//     }
// }
// else if (RoadFlag.AI == 10 && fabs(RoadFlag.AIAngle) > 15) {
//     // 退出条件（保持原样，但将 -40 改为 30 微调左转）
//     if (Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     else if (Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
//     else if (Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.9) {
//         CarInfo.dirOffset = -40;
//         RoadFlag.AI = 0;
//         RoadFlag.Red = 0;
//     }
// }
// }
// // //右转直道的急救包没问题 调眼镜  //相差 5cm
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = -80;
//       //  BuzzerMs(20);
//         RoadFlag.AI = 6;
//     }
//     // else if(RoadFlag.AI == 3 && fabs(RoadFlag.AIAngle) > 30 && RoadFlag.AIDis > 0.4 )
//     // {
//     //     CarInfo.dirOffset = 0;      //打完角后走小段距离    
//     //     RoadFlag.AI = 5;
//     //             BuzzerMs(100);
//     // }
//     else if(RoadFlag.AI == 6)
//     {
//         CarInfo.dirOffset = -66;
//     if(RoadFlag.AIDis > 0.36 && fabs(RoadFlag.AIAngle) > 20)   
//     {
//         CarInfo.dirOffset = 90;        //打回一个角度角
//         RoadFlag.AI = 8;
//             //    BuzzerMs(20);
//     }
//     }
//    else if(RoadFlag.AI == 8)
//    {   
//        CarInfo.dirOffset = 75;            // 持续回正
//     if(RoadFlag.AIDis > 0.65 && fabs(RoadFlag.AIAngle) > 35)
//     {
//         CarInfo.dirOffset = 75;        // 你的75慢慢回正
//         RoadFlag.AI = 10;
//     }
//    }
//     else if(RoadFlag.AI == 10 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         //左右都不丢边
//         if(Left != border_min && Right != border_max && LongLeft >DataInfo.forwardLook && RoadFlag.AIDis >= 0.9) //0.5 1.0 写入flash
//         {
//             CarInfo.dirOffset = -50;//-30  MIDDLE - Left - Right; 25
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(200);
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.9)//1.1
//         {
//             CarInfo.dirOffset = -50;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.9)//1.1 1.2
//         {
//             CarInfo.dirOffset = -50;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//            // BuzzerMs(20);
//         }
//     } 
// }
// // //右转直道的急救包11111没问题
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = -80;
//       //  BuzzerMs(20);
//         RoadFlag.AI = 6;
//     }
//     // else if(RoadFlag.AI == 3 && fabs(RoadFlag.AIAngle) > 30 && RoadFlag.AIDis > 0.4 )
//     // {
//     //     CarInfo.dirOffset = 0;      //打完角后走小段距离    
//     //     RoadFlag.AI = 5;
//     //             BuzzerMs(100);
//     // }
//     else if(RoadFlag.AI == 6)
//     {
//         CarInfo.dirOffset = -70;
//     if(RoadFlag.AIDis > 0.3 && fabs(RoadFlag.AIAngle) > 20)   
//     {
//         CarInfo.dirOffset = 80;        //打回一个角度角
//         RoadFlag.AI = 8;
//             //    BuzzerMs(20);
//     }
//     }
//    else if(RoadFlag.AI == 8)
//    {   
//        CarInfo.dirOffset = 80;            // 持续回正
//     if(RoadFlag.AIDis > 0.58 || fabs(RoadFlag.AIAngle) > 40)
//     {
//         CarInfo.dirOffset = 70;        // 你的75慢慢回正
//         RoadFlag.AI = 10;
//     }
//    }
//     else if(RoadFlag.AI == 10 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         //左右都不丢边
//         if(Left != border_min && Right != border_max && LongLeft >DataInfo.forwardLook && RoadFlag.AIDis >= 0.9) //0.5 1.0 写入flash
//         {
//             CarInfo.dirOffset = -45;//-30  MIDDLE - Left - Right; 25
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(200);
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.9)//1.1
//         {
//             CarInfo.dirOffset = -45;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.9)//1.1 1.2
//         {
//             CarInfo.dirOffset = -45;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//            // BuzzerMs(20);
//         }
//     } 
// void CrossControl(void)
// {
//     Cross_check();
//     cross_fill(Bmp,L_boundary, R_boundary, data_statics_l, data_statics_r, dir_l, dir_r, points_l, points_r);
// }
// 在文件顶部添加
// typedef struct {
//     int16 stage1;   // 初始转向
//     int16 stage2;   // 持续转向
//     int16 stage3;   // 回正开始
//     int16 stage4;   // 回正后期

//     float dis_5_to_7;   // AI==5 -> 7 的距离阈值
//     float dis_7_to_9;   // AI==7 -> 9 的距离阈值
//     float dis_9_exit;   // AI==9 退出距离（可选）
// } AiParams;
// 
// 左转参数（正值）
// static const AiParams left_params[] = {
//     // 直道 (|偏差| <= 20)
//     {90, 85, -90, -80,  0.4f, 0.6f, 1.1f},
//     // 弯道 (21 < |偏差| <= 60)
//     {140, 135, -120, -100, 0.50f, 0.85f, 1.2f},
//     // 急弯 (|偏差| > 60)
//     {160, 155, -140, -120, 0.60f, 0.95f, 1.3f}
// };

// // 右转参数（负值）
// static const AiParams right_params[] = {
//     // 直道
//     {-90, -85, 70, 60,  0.4f, 0.75f, 0.86f},
//     // 弯道
//     {-90, -75, 40, 30,  0.33f, 0.46f, 0.5f},
//     // 急弯
//     {-90, -75, 60, 50, 0.25f, 0.55f, 0.7f}
// };
// // 当前选中的参数组索引
// static uint8_t ai_param_idx = 0;
// void AIControl(void)
// {
//     float offset_abs = fabs(CarInfo.dirOffset);
//     uint8_t idx = 0;

//     // ===== 左转 =====
//     if (RoadFlag.AI == 1)
//     {
//         if (offset_abs <= 20) idx = 0;
//         else if (offset_abs <= 60) idx = 1;
//         else idx = 2;

//         CarInfo.dirOffset = left_params[idx].stage1;
//         RoadFlag.AI = 5;
//     }
//     else if (RoadFlag.AI == 5)
//     {
//         CarInfo.dirOffset = left_params[idx].stage2;
//         // 角度阈值固定为20
//         if (RoadFlag.AIDis > left_params[idx].dis_5_to_7 && fabs(RoadFlag.AIAngle) > 20.0f)
//         {
//             CarInfo.dirOffset = left_params[idx].stage3;
//             RoadFlag.AI = 7;
//         }
//     }
//     else if (RoadFlag.AI == 7)
//     {
//         CarInfo.dirOffset = left_params[idx].stage3;
//         // 角度阈值固定为40
//         if (RoadFlag.AIDis > left_params[idx].dis_7_to_9 && fabs(RoadFlag.AIAngle) > 40.0f)
//         {
//             CarInfo.dirOffset = left_params[idx].stage4;
//             RoadFlag.AI = 9;
//         }
//     }
//     else if (RoadFlag.AI == 9 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         // 退出距离阈值也分档
//         if (Left != border_min && Right != border_max && LongRight > DataInfo.forwardLook && RoadFlag.AIDis >= left_params[idx].dis_9_exit)
//         {
//             CarInfo.dirOffset = 50;
//             RoadFlag.AI = 0;
//             RoadFlag.Red = 0;
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 1.1)//1.1
//         {
//             CarInfo.dirOffset = 45;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 1.1)//1.1 1.2
//         {
//             CarInfo.dirOffset = 45;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//            // BuzzerMs(20);
//         }
//     }

//     // ===== 右转（对称） =====
//     else if (RoadFlag.AI == 2)
//     {
//         if (offset_abs <= 20) idx = 0;
//         else if (offset_abs <= 60) idx = 1;
//         else idx = 2;
        
//         CarInfo.dirOffset = right_params[idx].stage1;
//           BuzzerMs(200);
//         RoadFlag.AI = 6;
//     }
//     else if (RoadFlag.AI == 6)
//     {
//         CarInfo.dirOffset = right_params[idx].stage2;
//         // 角度阈值固定为25
//         if (RoadFlag.AIDis > right_params[idx].dis_5_to_7 && fabs(RoadFlag.AIAngle) > 20.0f)//0.3回正
//         {
//             CarInfo.dirOffset = right_params[idx].stage3;
//             //   BuzzerMs(200);
//             RoadFlag.AI = 8;
//         }
//     }
//     else if (RoadFlag.AI == 8)
//     {
//         CarInfo.dirOffset = right_params[idx].stage4;
//         // 右转阶段8不用角度，只用距离
//         if (RoadFlag.AIDis > right_params[idx].dis_7_to_9)
//         {
//             // BuzzerMs(200);
//             RoadFlag.AI = 10;
//         }
//     }
//     else if (RoadFlag.AI == 10)
//     {
//           BuzzerMs(200);
//         if (Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook && RoadFlag.AIDis >= right_params[idx].dis_9_exit)
//         {
//             CarInfo.dirOffset = -35;
//             RoadFlag.AI = 0;
//             // RoadFlag.Red = 0;
//         }
//       //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.5)//1.1
//         {
//             CarInfo.dirOffset = -35;//15 30
//             RoadFlag.AI = 0;
//             // RoadFlag.Red=0;
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.5)//1.1 1.2
//         {
//             CarInfo.dirOffset = -35;//-30 20 15
//             RoadFlag.AI = 0;
//             // RoadFlag.Red=0;
//            // BuzzerMs(20);
//     }
//   }
// }
//-------------------------------------------------------------------------------------------------------------------
//  @brief      AI绕行控制
//-------------------------------------------------------------------------------------------------------------------
// void AIControl(void)
// {//左转111普通直道(小弯 +-30)
//    if(RoadFlag.AI == 1)
//     {

//         CarInfo.dirOffset = 120;
//       //  BuzzerMs(20);
//         RoadFlag.AI = 5;
//     }
//     // else if(RoadFlag.AI == 3 && fabs(RoadFlag.AIAngle) > 30 && RoadFlag.AIDis > 0.4 )
//     // {
//     //     CarInfo.dirOffset = 0;      //打完角后走小段距离    
//     //     RoadFlag.AI = 5;

//     //             BuzzerMs(100);
//     // }
//     else if(RoadFlag.AI == 5)
//     {
//         CarInfo.dirOffset = 115;
//     if(RoadFlag.AIDis > 0.42 && fabs(RoadFlag.AIAngle) > 20)   
//     {
//         CarInfo.dirOffset = -115;        //打回一个角度角
//         RoadFlag.AI = 7;
//             //    BuzzerMs(20);
//     }
//     }
//    else if(RoadFlag.AI == 7)
//    {   
//        CarInfo.dirOffset = -100;            // 持续回正
//     if(RoadFlag.AIDis > 0.75 && fabs(RoadFlag.AIAngle) > 40)
//     {
//         CarInfo.dirOffset = -80;        // 你的75慢慢回正
//         RoadFlag.AI = 9;
//     }
//    }
//     else if(RoadFlag.AI == 9 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         //左右都不丢边
//         if(Left != border_min && Right != border_max && LongRight >DataInfo.forwardLook && RoadFlag.AIDis >= 1.1) //0.5 1.0 写入flash
//         {
//             CarInfo.dirOffset = 50;//-30  MIDDLE - Left - Right; 25
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(200);
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 1.1)//1.1
//         {
//             CarInfo.dirOffset = 45;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 1.1)//1.1 1.2
//         {
//             CarInfo.dirOffset = 45;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//            // BuzzerMs(20);
//         }
//     } 

// // //右转
//     if(RoadFlag.AI == 2)
//     {
//         CarInfo.dirOffset = -80; //急救包75
//         // BuzzerMs(200);
//         RoadFlag.AI = 6;
//     }
//     // else if(RoadFlag.AI == 3 && fabs(RoadFlag.AIAngle) > 30 && RoadFlag.AIDis > 0.4 )
//     // {
//     //     CarInfo.dirOffset = 0;      //打完角后走小段距离    
//     //     RoadFlag.AI = 5;

//     //             BuzzerMs(100);
//     // }
//     else if(RoadFlag.AI == 6)
//     {
//         CarInfo.dirOffset = -75;  //急救包70
//     if(RoadFlag.AIDis > 0.33 && fabs(RoadFlag.AIAngle) > 25)   
//     {
//         // BuzzerMs(200);
//         CarInfo.dirOffset = 40;        //打回一个角度角
//         RoadFlag.AI = 8;
//             //    BuzzerMs(20);
//     }
//     }
//    else if(RoadFlag.AI == 8)
//    {   
//        CarInfo.dirOffset = 30;            // 持续回正
//     if(RoadFlag.AIDis > 0.46)
//     {
//         BuzzerMs(200);
//         // CarInfo.dirOffset = 70;        // 你的75慢慢回正&& fabs(RoadFlag.AIAngle) > 40
//         RoadFlag.AI = 10;

//     }
//    }
//     else if(RoadFlag.AI == 10 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         //左右都不丢边
//         if(Left != border_min && Right != border_max && LongLeft >DataInfo.forwardLook && RoadFlag.AIDis >= 0.5) //0.5 1.0 写入flash
//         {
//             CarInfo.dirOffset = -40;//-30  MIDDLE - Left - Right; 25
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(200);
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 0.5)//1.1
//         {
//             CarInfo.dirOffset = -40;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 0.5)//1.1 1.2
//         {
//             CarInfo.dirOffset = -40;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//            // BuzzerMs(20);
//         }
//     } 
// }
// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      AI绕行控制十字
// //-------------------------------------------------------------------------------------------------------------------
// void AIControl(void)
// {//左转十字
//     if(RoadFlag.AI == 1)
//     {
//         CarInfo.dirOffset = 80;
//       //  BuzzerMs(20);
//         RoadFlag.AI = 5;
//     }
//     // else if(RoadFlag.AI == 3 && fabs(RoadFlag.AIAngle) > 30 && RoadFlag.AIDis > 0.4 )
//     // {
//     //     CarInfo.dirOffset = 0;      //打完角后走小段距离    
//     //     RoadFlag.AI = 5;

//     //             BuzzerMs(100);
//     // }
//     else if(RoadFlag.AI == 5)
//     {
//         CarInfo.dirOffset = 80;
//         if(RoadFlag.AIDis > 0.40 && fabs(RoadFlag.AIAngle) > 25) 
//         {  
//            CarInfo.dirOffset = -90;     //打回一个角度角
//            RoadFlag.AI = 7; 
//         }
//        //      BuzzerMs(20);
//     }
//     else if(RoadFlag.AI == 7 && RoadFlag.AIDis > 0.75)//0.8 0.9 0.7 0.6     写入flash
//     {
//                 // RoadFlag.protect = 1;
//         CarInfo.dirOffset = -70;        //打角回赛道 45 35
//         RoadFlag.AI = 9;
//              //   BuzzerMs(20);
//     }
//     else if(RoadFlag.AI == 9 && fabs(RoadFlag.AIAngle) > 15)
//     {
//         //左右都不丢边
//         if(Left != border_min && Right != border_max && LongLeft >DataInfo.forwardLook && RoadFlag.AIDis >= 1.1) //0.5 1.0 写入flash
//         {
//             CarInfo.dirOffset = 50;//-30  MIDDLE - Left - Right; 25
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左右都丢边
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 1.1)//1.1
//         {
//             CarInfo.dirOffset = 50;//15 30
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
// //            BuzzerMs(20);
//         }
//         //左边丢边右边不丢边
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 1.1)//1.1 1.2
//         {
//             CarInfo.dirOffset = 50;//-30 20 15
//             RoadFlag.AI = 0;
//             RoadFlag.Red=0;
//         //    BuzzerMs(20);
//         }
//     } 
// //右转急救包
// if(RoadFlag.AI == 2)
// {
//     CarInfo.dirOffset = -95;       // 大角度右转，因为急救包识别早，空间大
//     RoadFlag.AI = 60;
// }
// else if(RoadFlag.AI == 60)
// {
//     CarInfo.dirOffset = -85;       // 保持右转
//     if(RoadFlag.AIDis > 0.25 && fabs(RoadFlag.AIAngle) > 18)
//     {
//         CarInfo.dirOffset = 100;   // 柔和回正
//         RoadFlag.AI = 80;
//     }
// }
// else if(RoadFlag.AI == 80)
// {
//     CarInfo.dirOffset = 100;
//     if(RoadFlag.AIDis > 0.6 || fabs(RoadFlag.AIAngle) > 40)
//     {
//         CarInfo.dirOffset = 98;   // 慢慢回正
//         RoadFlag.AI = 100;
//     }
// }
// else if(RoadFlag.AI == 100)
// {
//     if (RoadFlag.AIDis >= 1.4) {   // 超时强制退出
//         CarInfo.dirOffset = 0; RoadFlag.AI = 0; RoadFlag.Red = 0;
//     }
//     else if(fabs(RoadFlag.AIAngle) > 20)
//     {
//         if(Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -35; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -35; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -35; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//     }
// }
// //眼镜
// // ======================== 眼镜右转（AI=20 → 62 → 82 → 102） ========================
// if(RoadFlag.AI == 20)
// {
//     CarInfo.dirOffset = -100;      // 保持你的初始右转
//     RoadFlag.AI = 62;
// }
// else if(RoadFlag.AI == 62)
// {
//     CarInfo.dirOffset = -90;       // 保持你的保持右转
//     // ★ 把距离从 0.2 降到 0.15，角度从 20 降到 18，让回正更早触发
//     if(RoadFlag.AIDis > 0.15 || fabs(RoadFlag.AIAngle) > 18)
//     {
//         CarInfo.dirOffset = 115;   // ★ 回正力度从 110 提到 115
//         RoadFlag.AI = 82;
//     }
// }
// else if(RoadFlag.AI == 82)
// {
//     CarInfo.dirOffset = 110;       // ★ 持续回正力度从 105 提到 110
//     // ★ 距离从 0.6 降到 0.45，角度从 40 降到 35，让慢慢回正更早进入
//     if(RoadFlag.AIDis > 0.45 || fabs(RoadFlag.AIAngle) > 35)
//     {
//         CarInfo.dirOffset = 90;    // ★ 慢慢回正力度从 100 降到 90，防止甩尾
//         RoadFlag.AI = 102;
//     }
// }
// else if(RoadFlag.AI == 102)
// {
//     CarInfo.dirOffset = 90;        // 持续慢慢回正
//     // 超时强制退出保持不变
//     if (RoadFlag.AIDis >= 1.4) {
//         CarInfo.dirOffset = 0; RoadFlag.AI = 0; RoadFlag.Red = 0;
//     }
//     else if(fabs(RoadFlag.AIAngle) > 20)
//     {
//         if(Left != border_min && Right != border_max && LongLeft > DataInfo.forwardLook && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -30; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//         else if(Left == border_min && Right == border_max && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -30; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//         else if(Left == border_min && Right != border_max && RoadFlag.AIDis >= 1.1)
//         { CarInfo.dirOffset = -30; RoadFlag.AI = 0; RoadFlag.Red = 0; }
//     }
// }
// }


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
