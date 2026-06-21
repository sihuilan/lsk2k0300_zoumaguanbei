// #include "zf_common_headfile.h"


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