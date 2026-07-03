#include "zf_common_headfile.h"

//左下拐点
int left_points_turnDown[4];
//右上拐点
int right_points_turnUp[4];
//右下拐点
int right_points_turnDown[4];
//左上拐点
int left_points_turnUp[4];

//最长白列左扫线下突变点
// int left_up_mutation[4];
// //最长白列右扫线上突变点
// int right_up_mutation[4];
//最长白列左扫线上突变点
int left_up_mutation[4];
//最长白列右扫线上突变点
int right_up_mutation[4];

int Ring_left_mutation[4];
//扫线算法找到俩个上突变点
void dispose_line(void)
{
    left_up_mutation[2] = 0;
    for(int i = ROW - 10; i >= 10; i --)
    {
    if(LeftBlack1[i]-LeftBlack1[i-2] <= -8 && LeftBlack1[i]-LeftBlack1[i+1] >= 0 && LeftBlack1[i]-LeftBlack1[i+1] <= 5)
        {
            left_up_mutation[0] = LeftBlack1[i];
            left_up_mutation[1] = i;
            left_up_mutation[2] = 1;
            left_up_mutation[3] = i;
            // printf("l = (%d,%d\n)",left_up_mutation[0],left_up_mutation[1]);
            break;
        }
    }

    right_up_mutation[2] = 0;
    for(int i = ROW - 10; i >= 10; i --)
    {
        if(RightBlack1[i]-RightBlack1[i-2] >= 8 && RightBlack1[i]-RightBlack1[i+1] <= 0 && RightBlack1[i]-RightBlack1[i+1] >= -3)
        {
            right_up_mutation[0] = RightBlack1[i];
            right_up_mutation[1] = i;
            right_up_mutation[2] = 1;
            right_up_mutation[3] = i;
            // printf("r_mutation = (%d,%d)\n",right_up_mutation[0],right_up_mutation[1]);
            break;
        }
    }
}
void Ring_Find_Point(void)
{
    Ring_left_mutation[2] = 0;
    if( data_statics_r != 0)
    {
        for(int i = 5; i < data_statics_l - 35; i++)
        {
            //上左
            if( dir_l[i-0] == 5
            &&  (dir_l[i+1] == 2 || dir_l[i+1]  == 3)
            )
            {
                //检测下面dir为0的状态数量是否不够
                int dir0conts = 0;
                for(int j = 2; j <= 5; j++) {
                    if( dir_l[i-j]  == 5
                    ||  dir_l[i-j]  == 4) {
                        dir0conts++;
                    }
                }
                // printf("1 = %d\n",dir0conts);
                if(dir0conts < 3) {
                    continue;
                }

                //检测上面dir为3的状态数量是否不够
                int dir3conts = 0;
                for(int j = 2; j <= 5; j++) {
                    if( dir_l[i+j]  == 2
                    ||  dir_l[i+j]  == 1) {
                        dir3conts++;
                    }
                }
                // printf("2 = %d\n",dir3conts);
                if(dir3conts < 3) {
                    continue;
                }

                Ring_left_mutation[0] = points_l[i][0];
                Ring_left_mutation[1] = points_l[i][1];
                Ring_left_mutation[2] = 1;
                Ring_left_mutation[3] = i;
                // printf("l = (%d,%d)\n",Ring_left_mutation[0],Ring_left_mutation[1]);
                break;
            }
        }
    }

}
   

// //爬线算法找四个角点
// void Cross_check(void)
// {
//     //左下拐点
//     left_points_turnDown[2] = 0;
//     if( data_statics_r != 0)
//     {
//         for(int i = 5; i < data_statics_l - 35; i++)
//         {
//             //左下拐点特征  上左
//             if( dir_l[i-0] == 4
//             &&  (dir_l[i+1] == 2 || dir_l[i+1]  == 3)
//             )
//             {
//                 //检测下面dir为0的状态数量是否不够
//                 int dir0conts = 0;
//                 for(int j = 2; j <= 5; j++) {
//                     if( dir_l[i-j]  == 5
//                     ||  dir_l[i-j]  == 4) {
//                         dir0conts++;
//                     }
//                 }
//                 // printf("1 = %d\n",dir0conts);
//                 if(dir0conts < 3) {
//                     continue;
//                 }

//                 //检测上面dir为3的状态数量是否不够
//                 int dir3conts = 0;
//                 for(int j = 2; j <= 5; j++) {
//                     if( dir_l[i+j]  == 2
//                     ||  dir_l[i+j]  == 1) {
//                         dir3conts++;
//                     }
//                 }
//                 // printf("2 = %d\n",dir3conts);
//                 if(dir3conts < 3) {
//                     continue;
//                 }

//                 left_points_turnDown[0] = points_l[i][0];
//                 left_points_turnDown[1] = points_l[i][1];
//                 left_points_turnDown[2] = 1;
//                 left_points_turnDown[3] = i;
//                 // printf("l = (%d,%d)\n",left_points_turnDown[0],left_points_turnDown[1]);
//                 break;
//             }
//         }
//     }
//     //右下拐点
//     right_points_turnDown[2] = 0;
//     if( data_statics_l != 0)
//     {
//         for(int i = 5; i < data_statics_r - 35; i++)
//         {
//             //右下拐点特征  上右
//             if( dir_r[i+0]  == 4
//             &&  ( dir_r[i+1]  == 3 || dir_r[i+1]  == 2)  //
//             )
//             {
//                 //检测下面dir为0的状态数量是否不够
//                 int dir0conts = 0;
//                 for(int j = 2; j <= 5; j++) {
//                     if( dir_r[i-j]  == 5
//                     ||  dir_r[i-j]  == 4) {
//                         dir0conts++;
//                     }
//                 }
//                 if(dir0conts < 3) {
//                     continue;
//                 }

//                 //检测上面dir为1的状态数量是否不够
//                 int dir1conts = 0;
//                 for(int j = 2; j <= 5; j++) {
//                     if( dir_r[i+j]  == 3
//                     ||  dir_r[i+j]  == 2) {
//                         dir1conts++;
//                     }
//                 }
//                 if(dir1conts < 3) {
//                     continue;
//                 }

//                 right_points_turnDown[0] = points_r[i][0];
//                 right_points_turnDown[1] = points_r[i][1];
//                 right_points_turnDown[2] = 1;
//                 right_points_turnDown[3] = i;
//                 // printf("r = (%d,%d)\n",right_points_turnDown[0],right_points_turnDown[1]);
//                 break;
//             }
//         }
//     }
//     //左上拐点
//     left_points_turnUp[2] = 0; 
//     int upper_corner_found = 0; // 引入局部安全锁标志

//     if(data_statics_l != 0)
//     {
//         for(int i = data_statics_l - 11; i >= 10; i--)
//         {
//             if(upper_corner_found == 1) {
//                 break; 
//             }
//             if(dir_l[i] == 4 && (dir_l[i+1] == 4 || dir_l[i+1] == 5))
//             {
//             // 检测拐角前 dir 为 6 的数量
//             int dir0conts1 = 0;
//             for(int j = 2; j <= 10; j++) {
//                 if(dir_l[i-j] == 6) {
//                     dir0conts1++;
//                 }
//             }
//             if(dir0conts1 < 4) continue;

//             // 检测拐角后 dir 为 4 或 5 的数量
//             int dir3conts1 = 0;
//             for(int j = 2; j <= 8; j++) {
//                 if(dir_l[i+j] == 4 || dir_l[i+j] == 5) {
//                     dir3conts1++;
//                 }
//             }
//             if(dir3conts1 < 4) continue;

//             // ==================================================
//             // ??? 严格注意：只有绝对通过了上面两个 continue，才会走到这里！
//             // ==================================================
            
//             // 打印真正锁定的数据
//             // printf("【判定成功】锁定上角点！ly = %d, dir0 = %d, dir3 = %d\n", i, dir0conts1, dir3conts1);

//             // 写入数组
//             left_points_turnUp[0] = points_l[i][0];
//             left_points_turnUp[1] = points_l[i][1];
//             left_points_turnUp[2] = 1; // 标志位写1
//             left_points_turnUp[3] = i;
//             // printf("l = (%d,%d)\n",left_points_turnUp[0],left_points_turnUp[1]);
//             upper_corner_found = 1; // ? 激活安全锁
//             break; // 强制跳出 i 循环
//         }
//         }
//     }
//     right_points_turnUp[2] = 0; 
//     int right_upper_corner_found = 0; // 引入局部安全锁标志

//     if(data_statics_r != 0)
//     {
//         for(int i = data_statics_r - 11; i >= 10; i--)
//         {
//             if(right_upper_corner_found == 1) {
//                 break; 
//             }
//             if(dir_r[i] == 4 && (dir_r[i+1] == 4 || dir_r[i+1] == 5))
//             {
//             // 检测拐角前 dir 为 6 的数量
//             int dir0conts1 = 0;
//             for(int j = 2; j <= 10; j++) {
//                 if(dir_r[i-j] == 6) {
//                     dir0conts1++;
//                 }
//             }
//             if(dir0conts1 < 4) continue;

//             // 检测拐角后 dir 为 4 或 5 的数量
//             int dir3conts1 = 0;
//             for(int j = 2; j <= 8; j++) {
//                 if(dir_r[i+j] == 4 || dir_r[i+j] == 5) {
//                     dir3conts1++;
//                 }
//             }
//             if(dir3conts1 < 4) continue;

//             // ==================================================
//             // ??? 严格注意：只有绝对通过了上面两个 continue，才会走到这里！
//             // ==================================================
            
//             // 打印真正锁定的数据
//             // printf("【判定成功】锁定上角点！ly = %d, dir0 = %d, dir3 = %d\n", i, dir0conts1, dir3conts1);

//             // 写入数组
//             right_points_turnUp[0] = points_r[i][0];
//             right_points_turnUp[1] = points_r[i][1];
//             right_points_turnUp[2] = 1; // 标志位写1
//             right_points_turnUp[3] = i;
//             // printf("r_up = (%d,%d)\n",right_points_turnUp[0],right_points_turnUp[1]);
//             right_upper_corner_found = 1; // ? 激活安全锁
//             break; // 强制跳出 i 循环
//         }
//         }
//     }
// }

    // right_points_turnUp[2] = 0;
    // int right_upper_corner_found = 0; // 引入局部安全锁标志

    // if(data_statics_l != 0)
    // {
    // // 从大到小搜索，优先找到高行数的右上方点
    // for(int i = data_statics_r - 11; i >= 10; i--)
    // {
    //     // 如果已经安全锁定了，直接退出
    //     if(right_upper_corner_found == 1) {
    //         break; 
    //     }

    //     if(dir_r[i] == 4 && (dir_r[i+1] == 4 || dir_r[i+1] == 5))
    //     {
    //         // 检测拐点后，是不是有一段稳定的 5 趋势（允许毛刺）
    //         int dir5conts = 0;
    //         for(int j = 1; j <= 5; j++) {
    //             if(dir_r[i+j] == 5 || dir_r[i+j] == 4) {
    //                 dir5conts++;
    //             }
    //         }
    //         if(dir5conts < 3) continue;

    //         // 检测拐点前，是不是有预期的 3 或 4 的趋势
    //         int dir3conts = 0;
    //         for(int j = 2; j <= 5; j++) {
    //             if(dir_r[i-j] == 5 || dir_r[i-j] == 6) {
    //                 dir3conts++;
    //             }
    //         }
    //         if(dir3conts < 3) continue;

    //         // ==================================================
    //         // 只有绝对通过了上面两个 continue，才会走到这里！
    //         // ==================================================
            
    //         // 打印真正锁定的数据
    //         // printf("【判定成功】锁定右上方点！ry = %d, dir5conts = %d, dir3conts = %d\n", 
    //         //        i, dir5conts, dir3conts);

    //         // 写入数组
    //         right_points_turnUp[0] = points_r[i][0];
    //         right_points_turnUp[1] = points_r[i][1];
    //         right_points_turnUp[2] = 1; // 标志位写1
    //         right_points_turnUp[3] = i;

    //         right_upper_corner_found = 1; // 激活安全锁
    //         break; // 强制跳出 i 循环
    //     }
    // }
    // }
    
// }




























// float calculate_slopeK(int y1, int y2, int x1, int x2)
// {
//     if(x1 - x2 == 0) {
//         return 0.0f;
//     }
//     return (y1 - y2) * 1.0 / (x1 - x2) * 1.0;
// }

// float calculate_slopeB(float k, int y, int x)
// {
//     return (y * 1.0 - k * x);
// }

// //正入十字检测
// static int check_Zheng_CrossRoad(void)
// {
//     // if( maxWhite.maxNum >= ROWSIMAGE -  rowCutUp -  rowCutBottom
//     // &&  maxWhite.lineNum > 10)
//     // {
//         if( left_points_turnDown[2] && right_points_turnDown[2]
//         &&  abs(left_points_turnDown[0] - right_points_turnDown[0]) < 30
//         &&  left_points_turnDown[0] > 30 && right_points_turnDown[0] > 30)
//         {
//             return 1;
//         }

//         if( (L_firstLossY > 80 && right_points_turnDown[2] 
//         // &&  right_up_mutation[2] 
//         && R_firstLossY < right_points_turnDown[0]) 
//         &&  L_lossPointsum != 0 
//         // &&  (!left_edgeline.is_weak_alone || (left_edgeline.is_weak_alone && left_edgeline.lossPoint > 30))
//         )
//         {
//             return 2;
//         }

//         if( (R_firstLossY > 80 && left_points_turnDown[2] 
//         // &&  left_up_mutation[2]
//         && L_firstLossY < left_points_turnDown[0])
//         &&  R_lossPointsum != 0 
//         // &&  (!right_edgeline.is_weak_alone || (right_edgeline.is_weak_alone && right_edgeline.lossPoint > 30))
//         )
//         {
//             return 3;
//         }
//     // }
    

//     return 0;
// }

// //十字中央
// static int check_In_CrossRoad(void)
// {
//     if( L_firstLossY > 80 && R_firstLossY > 80
//     // &&  left_up_mutation[2] && right_up_mutation[2]//扫线突变，我目前还没有写进去
//     )
//     {
//         return 1;
//     }

//     return 0;
// }

// //十字正中间补线
// void front_in_crossRoad_Line(void);
// //斜入十字左连线
// void front_in_Lup_LDown_Link(void);
// //斜入十字右连线
// void front_in_Rup_RDown_Link(void);
// //十字中央拉线
// void in_crossRoad_pullLine(void);

// //十字所有状态处理
// void crossRoad_processing(void)
// {
//     switch (cross_state)
//     {
//     case front_crossRoad_1:
//         front_in_crossRoad_Line();
//         left_plus_right_2midline();
//         break;
    
//     case front_crossRoad_2:
//         front_in_Rup_RDown_Link();
//         rightLine_plus_halfWide();
//         break;
    
//     case front_crossRoad_3:
//         front_in_Lup_LDown_Link();
//         leftLine_plus_halfWide();
//         break;

//     case in_crossRoad_1:
//         in_crossRoad_pullLine();
//         left_plus_right_2midline();
//         break;

//     default:
//         break;
//     }
// }

// //正入十字
// void front_in_crossRoad_Line(void)
// {
//     int L_target_y = 0;
//     int L_target_x = maxWhite.starlineX;
//     int L_base_y = left_points_turnDown[0];
//     int L_base_x = left_points_turnDown[1];
//     float lk = calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
//     float lb =  calculate_slopeB(lk, L_base_y, L_base_x);
//     for(int i = 0; i < data_statics_l; i++)
//     {
//         points_l[i][0] = 99 - i;
//         points_l[i][1] = ((99 - i) - lb) / lk;
//     }

//     int R_target_y = 0;
//     int R_target_x = maxWhite.endlineX;
//     int R_base_y = right_points_turnDown[0];
//     int R_base_x = right_points_turnDown[1];
//     float rk =  calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
//     float rb =  calculate_slopeB(rk, R_base_y, R_base_x);
//     for(int i = 0; i < data_statics_r; i++)
//     {
//         points_r[i][0] = 99 - i;
//         points_r[i][1] = ((99 - i) - rb) / rk;
//     }
// }

// //十字中央拉线
// void in_crossRoad_pullLine(void)
// {
//     int L_target_y = points_l[left_up_mutation[3]+10][0];
//     int L_target_x = points_l[left_up_mutation[3]+10][1];
//     int L_base_y = left_up_mutation[0];
//     int L_base_x = left_up_mutation[1];
//     float lk =  calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
//     float lb =  calculate_slopeB(lk, L_base_y, L_base_x);
//     for(int i = 0; i < data_statics_l; i++)
//     {
//         points_l[i][0] = 99 - i;
//         points_l[i][1] = ((99 - i) - lb) / lk;
//     }

//     int R_target_y = points_r[right_up_mutation[3]+10][0];
//     int R_target_x = points_r[right_up_mutation[3]+10][1];
//     int R_base_y = right_up_mutation[0];
//     int R_base_x = right_up_mutation[1];
//     float rk =  calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
//     float rb =  calculate_slopeB(rk, R_base_y, R_base_x);
//     for(int i = 0; i < data_statics_r; i++)
//     {
//         points_r[i][0] = 99 - i;
//         points_r[i][1] = ((99 - i) - rb) / rk;
//     }
// }

// //斜入十字左连线
// void front_in_Lup_LDown_Link(void)
// {
//     int L_target_y = left_up_mutation[0];
//     int L_target_x = left_up_mutation[1];
//     int L_base_y = left_points_turnDown[0];
//     int L_base_x = left_points_turnDown[1];
//     float lk =  calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
//     float lb =  calculate_slopeB(lk, L_base_y, L_base_x);
//     for(int i = 0; i < data_statics_l; i++)
//     {
//         points_l[i][0] = 99 - i;
//         points_l[i][1] = ((99 - i) - lb) / lk;
//     }
// }

// //斜入十字右连线
// void front_in_Rup_RDown_Link(void)
// {
//     int R_target_y = right_up_mutation[0];
//     int R_target_x = right_up_mutation[1];
//     int R_base_y = right_points_turnDown[0];
//     int R_base_x = right_points_turnDown[1];
//     float rk =  calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
//     float rb =  calculate_slopeB(rk, R_base_y, R_base_x);
//     for(int i = 0; i < data_statics_r; i++)
//     {
//         points_r[i][0] = 99 - i;
//         points_r[i][1] = ((99 - i) - rb) / rk;
//     }
// }