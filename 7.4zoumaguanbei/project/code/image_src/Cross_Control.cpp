#include "zf_common_headfile.h"

float calculate_slopeK(int y1, int y2, int x1, int x2)
{
    if(x1 - x2 == 0) {
        return 0.0f;
    }
    return (y1 - y2) * 1.0 / (x1 - x2) * 1.0;
}

float calculate_slopeB(float k, int y, int x)
{
    return (y * 1.0 - k * x);
}

int16 limit_a_b(int16 x, int a, int b)
{
    if(x<a) x = a;
    if(x>b) x = b;
    return x;
}

// /** 
// * @brief 最小二乘法
// * @param uint8 begin				输入起点
// * @param uint8 end					输入终点
// * @param uint8 *border				输入需要计算斜率的边界首地址
// *  @see CTest		Slope_Calculate(start, end, border);//斜率
// * @return 返回说明
// *     -<em>false</em> fail
// *     -<em>true</em> succeed
// */
float Slope_Calculate(uint8 begin, uint8 end, uint16 *border)
{
	float xsum = 0, ysum = 0, xysum = 0, x2sum = 0;
	int16 i = 0;
	float result = 0;
	static float resultlast;

	for (i = begin; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		xysum += i * (border[i]);
		x2sum += i * i;

	}
	if ((end - begin)*x2sum - xsum * xsum) //判断除数是否为零
	{
		result = ((end - begin)*xysum - xsum * ysum) / ((end - begin)*x2sum - xsum * xsum);
		resultlast = result;
	}
	else
	{
		result = resultlast;
	}
	return result;
}
// /** 
// * @brief 计算斜率截距
// * @param uint8 start				输入起点
// * @param uint8 end					输入终点
// * @param uint8 *border				输入需要计算斜率的边界
// * @param float *slope_rate			输入斜率地址
// * @param float *intercept			输入截距地址
// *  @see CTest		calculate_s_i(start, end, r_border, &slope_l_rate, &intercept_l);
// * @return 返回说明
// *     -<em>false</em> fail
// *     -<em>true</em> succeed
// */
void calculate_s_i(uint8 start, uint8 end, uint16 *border, float *slope_rate, float *intercept)
{
	uint16 i, num = 0;
	uint16 xsum = 0, ysum = 0;
	float y_average, x_average;

	num = 0;
	xsum = 0;
	ysum = 0;

	for (i = start; i < end; i++)
	{
		xsum += i;
		ysum += border[i];
		num++;
	}

	//计算各个平均数
	if (num)
	{
		x_average = (float)(xsum / num);
		y_average = (float)(ysum / num);

	}
    else
    {
        *slope_rate = 0;
        *intercept = 0;
        return;
    }
	/*计算斜率*/
	*slope_rate = Slope_Calculate(start, end, border);//斜率
	*intercept = y_average - (*slope_rate)*x_average;//截距
}

//正入十字检测
// static 
int check_CrossRoad(void)
{
        dispose_line();
        Cross_check();
    // if( maxWhite.maxNum >= ROWSIMAGE - rowCutUp - rowCutBottom
    // &&  maxWhite.lineNum > 10)
    // {
        if( left_points_turnDown[2] && right_points_turnDown[2]
        &&  abs(left_points_turnDown[1] - right_points_turnDown[1]) < 30
        &&  left_points_turnDown[1] > 13 && right_points_turnDown[1] > 13)
        {
            return 1;
        }

        if( (left_edgeline.firstLossY[0] < 40 && right_points_turnDown[2]
        &&  left_edgeline.firstLossY[1]  == 1 
        // && right_edgeline.firstLossY[1] == 1
        &&  right_up_mutation[2])  //&& right_edgeline.firstLossY[0] > right_points_turnDown[0]
        &&  left_edgeline.losspoint != 0 && left_edgeline.losspoint > 30
        // &&  (!left_edgeline.is_weak_alone || (left_edgeline.is_weak_alone ))
        )
        {
            return 2;
        }

        if( (right_edgeline.firstLossY[0] < 40 && left_points_turnDown[2] 
        // &&  left_edgeline.firstLossY[1]  == 1 
        && right_edgeline.firstLossY[1] == 1
        &&  left_up_mutation[2] )//&& left_edgeline.firstLossY[0] > left_points_turnDown[0]
        &&  right_edgeline.losspoint != 0  && right_edgeline.losspoint > 30
        // &&  (!right_edgeline.is_weak_alone || (right_edgeline.is_weak_alone && ))
        )
        {
            return 3;
        }
    // }
    
    return 0;
}

//十字中央
// static 
int check_In_CrossRoad(void)
{
    if( left_edgeline.firstLossY[0] < 10 && right_edgeline.firstLossY[0] < 10
    &&  left_edgeline.firstLossY[1]  == 1 && right_edgeline.firstLossY[1] == 1
    &&  left_up_mutation[2] && right_up_mutation[2])
    {
        return 1;
    }

    return 0;
}

//十字正中间补线
void front_in_crossRoad_Line(void);
//斜入十字左连线
void front_in_Lup_LDown_Link(void);
//斜入十字右连线
void front_in_Rup_RDown_Link(void);
//十字中央拉线
void in_crossRoad_pullLine(void);

//十字所有状态处理
void crossRoad_processing(void)
{
    switch (check_CrossRoad())
    {
    case 1:
        front_in_crossRoad_Line();
        left_plus_right_2midline();     //左右边线合成中线
        break;
    
    case 2:
        front_in_Rup_RDown_Link();
        rightLine_plus_halfWide();
        break;
    
    case 3:
        front_in_Lup_LDown_Link();
        leftLine_plus_halfWide();       //左右边线合成中线
        break;
    default:
        break;
    }
}

//正入十字
void front_in_crossRoad_Line(void)
{
    // int L_target_x = left_up_mutation[0];
    // int L_target_y = left_up_mutation[1];
    // int L_base_x = left_points_turnDown[0];
    // int L_base_y = left_points_turnDown[1];
    // float lk = calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
    // float lb = calculate_slopeB(lk, L_base_y, L_base_x);
    // for(uint16 i = left_points_turnDown[1];i < left_up_mutation[1];i++)
    // {
    //     L_boundary[i] = static_cast<uint16>(i*lk+lb); 
    //     // points_l[i][1] = ((99 - i) - lb) / lk;
    // }
		int16 x_down_l = left_points_turnDown[0];
        int16 y_down_l = left_points_turnDown[1];
        int16 x_up_l   = left_up_mutation[0];
        int16 y_up_l   = left_up_mutation[1];

        if (y_up_l != y_down_l)
        {
            // 算出左边上下角点的两点式斜率 k (使用浮点数防止精度丢失截断)
            float k_l = (float)(x_up_l - x_down_l) / (float)(y_up_l - y_down_l);
            
            // 确定循环区间：因为倒装，确保从 Y 小的地方循环到 Y 大的地方
            int16 y_start = (y_down_l < y_up_l) ? y_down_l : y_up_l;
            int16 y_end   = (y_down_l > y_up_l) ? y_down_l : y_up_l;

            // 顺着 Y 轴，将两点之间的空旷区用直线方程强行覆盖
            for (uint8 i = y_start; i <= y_end; i++)
            {
                if (i >= 0 && i < CROP_H) // 边界安全保护
                {
                    // 直线方程：X = k * (Y - Y_down) + X_down
                    int32 fill_x = (int32)(k_l * (i - y_down_l) + x_down_l);
                    L_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
                }
            }
        }

    //右边补线
        int16 x_down_r = right_points_turnDown[0];
        int16 y_down_r = right_points_turnDown[1];
        int16 x_up_r   = right_up_mutation[0];
        int16 y_up_r   = right_up_mutation[1];

        if (y_up_r != y_down_r)
        {
            // 算出右边上下角点的两点式斜率
            float k_r = (float)(x_up_r - x_down_r) / (float)(y_up_r - y_down_r);
            
            int16 y_start = (y_down_r < y_up_r) ? y_down_r : y_up_r;
            int16 y_end   = (y_down_r > y_up_r) ? y_down_r : y_up_r;

            // 顺着 Y 轴，对右边撕裂处进行强行填补
            for (uint8 i = y_start; i <= y_end; i++)
            {
                if (i >= 0 && i < CROP_H) // 边界安全保护
                {
                    // 直线方程：X = k * (Y - Y_down) + X_down
                    int32 fill_x = (int32)(k_r * (i - y_down_r) + x_down_r);
                    R_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
                }
            }
        }
    // int R_target_x = right_up_mutation[0];
    // int R_target_y = right_up_mutation[1];
    // int R_base_x = right_points_turnDown[0];
    // int R_base_y = right_points_turnDown[1];   
    // float rk = calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
    // float rb = calculate_slopeB(rk, R_base_y, R_base_x);
    // for(uint16 i = right_points_turnDown[1];i < right_up_mutation[1];i++)
    // {
    //     // L_R_boundary
    //     R_boundary[i] = static_cast<uint16>(i*rk+rb); 
    //     // points_r[i][0] = 99 - i;
    //     // points_r[i][1] = ((99 - i) - rb) / rk;
    // }
}

//十字中央拉线
void in_crossRoad_pullLine(void)
{
    uint8 start, end;
    float slope_l_rate = 0, intercept_l = 0;
    if(check_In_CrossRoad()){


    //计算斜率
    start = left_up_mutation[1] + 5;
    // start = limit_a_b(start, 0, ROW);
    end = left_up_mutation[1] + 15;
    calculate_s_i(start, end, L_boundary, &slope_l_rate, &intercept_l);
    //printf("slope_l_rate:%d\nintercept_l:%d\n", slope_l_rate, intercept_l);
    for (uint8 i = 0; i < (left_up_mutation[1] + 20); i++)
    {
        L_boundary[i] = slope_l_rate * (i)+intercept_l;//y = kx+b
        L_boundary[i] = limit_a_b(L_boundary[i], border_min, border_max);//限幅
    }

    //计算斜率
    start = right_up_mutation[1] + 5;//起点
    start = limit_a_b(start, 0, ROW);//限幅
    end = right_up_mutation[1] + 15;//终点
    calculate_s_i(start, end, R_boundary, &slope_l_rate, &intercept_l);

    for (uint8 i = 0 ; i < (right_up_mutation[1] + 20); i++)
    {
        R_boundary[i] = slope_l_rate * (i)+intercept_l;
        R_boundary[i] = limit_a_b(R_boundary[i], border_min, border_max);
    }
	CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
    // float lk = calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
    // float lb = calculate_slopeB(lk, L_base_y, L_base_x);
    // for(int i = 0; i < ROW; i++)
    // {
    //     points_l[i][0] = 99 - i;
    //     points_l[i][1] = ((99 - i) - lb) / lk;
    // }

    // int R_base_y = points_r[right_up_mutation[3]-10][0];
    // int R_base_x = points_r[right_up_mutation[3]-10][1];
    // int R_target_y = right_up_mutation[0];
    // int R_target_x = right_up_mutation[1];
    // float rk = calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
    // float rb = calculate_slopeB(rk, R_base_y, R_base_x);
    // for(int i = 0; i < ROW; i++)
    // {
    //     points_r[i][0] = 99 - i;
    //     points_r[i][1] = ((99 - i) - rb) / rk;
    // }    
    }
    
}

//斜入十字左连线
void front_in_Lup_LDown_Link(void)
{
    int i;
    // int L_target_y = left_up_mutation[0];
    // int L_target_x = left_up_mutation[1];
    // int L_base_y = left_points_turnDown[0];
    // int L_base_x = left_points_turnDown[1];
    // float lk = calculate_slopeK(L_target_y, L_base_y, L_target_x, L_base_x);
    // float lb = calculate_slopeB(lk, L_base_y, L_base_x);
    // for(int i = 0; i < data_statics_l; i++)
    // {
    //     points_l[i][0] = 99 - i;
    //     points_l[i][1] = ((99 - i) - lb) / lk;
    // }
    // {
        int16 x_down_l = left_points_turnDown[0];
        int16 y_down_l = left_points_turnDown[1];
        int16 x_up_l   = left_up_mutation[0];
        int16 y_up_l   = left_up_mutation[1];

        if (y_up_l != y_down_l)
        {
            // 算出左边上下角点的两点式斜率 k (使用浮点数防止精度丢失截断)
            float k_l = (float)(x_up_l - x_down_l) / (float)(y_up_l - y_down_l);
            
            // 确定循环区间：因为倒装，确保从 Y 小的地方循环到 Y 大的地方
            int16 y_start = (y_down_l < y_up_l) ? y_down_l : y_up_l;
            int16 y_end   = (y_down_l > y_up_l) ? y_down_l : y_up_l;

            // 顺着 Y 轴，将两点之间的空旷区用直线方程强行覆盖
            for (i = y_start; i <= y_end; i++)
            {
                if (i >= 0 && i < CROP_H) // 边界安全保护
                {
                    // 直线方程：X = k * (Y - Y_down) + X_down
                    int32 fill_x = (int32)(k_l * (i - y_down_l) + x_down_l);
                    L_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
                }
            }
        }
    // CarInfo.dirOffset = L_boundary[DataInfo.forwardLook - 5] + 20;
    // }
}

//斜入十字右连线
void front_in_Rup_RDown_Link(void)
{
    int16 x_down_r = right_points_turnDown[0];
    int16 y_down_r = right_points_turnDown[1];
    int16 x_up_r   = right_up_mutation[0];
    int16 y_up_r   = right_up_mutation[1];

    if (y_up_r != y_down_r)
    {
        // 算出右边上下角点的两点式斜率
        float k_r = (float)(x_up_r - x_down_r) / (float)(y_up_r - y_down_r);
        
        int16 y_start = (y_down_r < y_up_r) ? y_down_r : y_up_r;
        int16 y_end   = (y_down_r > y_up_r) ? y_down_r : y_up_r;

        // 顺着 Y 轴，对右边撕裂处进行强行填补
        for (uint8 i = y_start; i <= y_end; i++)
        {
            if (i >= 0 && i < CROP_H) // 边界安全保护
            {
                // 直线方程：X = k * (Y - Y_down) + X_down
                int32 fill_x = (int32)(k_r * (i - y_down_r) + x_down_r);
                R_boundary[i] = limit_a_b(fill_x, border_min, border_max); // 限幅
            }
        }
    }
    // int R_target_y = right_up_mutation[0];
    // int R_target_x = right_up_mutation[1];
    // int R_base_y = right_points_turnDown[0];
    // int R_base_x = right_points_turnDown[1];
    // float rk = calculate_slopeK(R_target_y, R_base_y, R_target_x, R_base_x);
    // float rb = calculate_slopeB(rk, R_base_y, R_base_x);
    // for(int i = 0; i < data_statics_r; i++)
    // {
    //     points_r[i][0] = 99 - i;
    //     points_r[i][1] = ((99 - i) - rb) / rk;
    // }

}
void left_plus_right_2midline(void)
{
    CarInfo.dirOffset = (L_boundary[30] + R_boundary[30] - MIDDLE);
    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
    
    // printf("a");
}
void rightLine_plus_halfWide(void)
{
    CarInfo.dirOffset = R_boundary[30] - Standard_Haf_Road_Wide[30];
    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
}
void leftLine_plus_halfWide(void)
{
    CarInfo.dirOffset = L_boundary[30] + Standard_Haf_Road_Wide[30];
    CarInfo.dirOffset = ValueLimitint(CarInfo.dirOffset, OFFSET_MAX, -OFFSET_MAX);
    // int32 sum_offset = 0;
    // int valid_rows = 0;
    // for(int i = 15; i <= 25; i++) {
    //     sum_offset += (L_boundary[i] + (Standard_Haf_Road_Wide[i]*0.8));
    //     valid_rows++;
    // }
    // CarInfo.dirOffset = sum_offset / valid_rows;
}