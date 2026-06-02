#include "zf_common_headfile.h"

cv::Mat frame_binary;

uint8 forwardLook[COL];                                 //前瞻行像素点反转
uint8 WhiteSum;                                         //前瞻行白点数
uint8 LastRight, LastLeft;                              //历史左右边界
uint8 Right = COL - 1, Left = 0;                        //当前左右边界  
// uint8 Width[ROW];
uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像横、纵方向白点数

//目前凑数用的参数
uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; //左右边界、中线
uint8 LongLeft, LongRight;                              //左右有边界行数

uint8 Longest_White_Column_Left[2]={0};                 //左侧最长白列【0】是白列长度，【1】是下标，第j列
uint8 Longest_White_Column_Right[2]={0};                //右侧最长白列【0】是白列长度，【1】是下标，第j列

uint8  Max_row = 0;//最大宽度所在行数
uint8 max_cnt_row_l = 0;//点数最多的行
uint8 max_cnt_row_r = 0;
uint8 max_cnt_col_l = 0;//点数最多的列
uint8 max_cnt_col_r = 0;
uint8 max_row_cnt_l = 0;//点数最多的行的计数
uint8 max_row_cnt_r = 0;
uint8 max_col_cnt_l = 0;//点数最多的列的计数
uint8 max_col_cnt_r = 0;
//迷宫法巡线参数
uint8 start_point_l[2] = {0};//左边起点的x，y值
uint8 start_point_r[2] = {0};//右边起点的x，y值
int points_l[(uint16)USE_num][2] = {{0}};//左线
int points_r[(uint16)USE_num][2] = {{0}};//右线
uint16 dir_r[(uint16)USE_num] = {0};//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = {0};//用来存储左边生长方向
uint16 data_statics_l = 0;//统计左边找到的点
uint16 data_statics_r = 0;//统计右边找到的点
uint16 data_statics_m = 0;//统计中线找到的点
uint16 l_highest = 0;//最高点
uint16 r_highest = 0;//最高点
uint8 Road_HighestPoint = 0;//最高点
uint16 leftmost[2] ={0};//最左点记录
uint16 rightmost[2] ={0};//最右点记录


    //左边变量
    uint8 search_filds_l[8][2] = {{0}};
    uint16 index_l = 0;
    uint8 temp_l[8][2] = {{0}};
    int center_point_l[2] = {0};//中心坐标点 
	//右边变量
    uint8 search_filds_r[8][2] = {{0}};
    uint16 index_r = 0;
    uint8 temp_r[8][2] = {{0}};
    int center_point_r[2] = {0};//中心坐标点
	//定义八个邻域
	static int8 seeds_l[8][2] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
    static int8 seeds_r[8][2] = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1} };
    //注意一点，找到的第一个生长方向是1，不是0，最后一个8才是0;


uint16 Width[ROW] = {0};//按行记录的宽度值
uint16 Row_Max_Width = 0;//最大宽度值
uint16 Point_Width[(uint16)USE_num] = {0};//按点记录的宽度值
uint16 Point_Max_Width = 0;//最大宽度值

//八领域的变量
uint16 L_boundary[ROW] = { 0 };  // 左边界数组
uint16 R_boundary[ROW] = { 0 };  // 右边界数组

uint8 l_stop = 0;      //用于标记左边界停止了八邻域
uint8 r_stop = 0;      //用于标记右边界停止了八邻域

int Middle_line[(uint16)USE_num][2] ={ { 0 } };//中间线的x，y值
uint16 Row_Middle_line[ROW] = {0};//按行记录的中线x坐标值

//-------------------------------------------------------------------------------------------------------------------
// //  @brief      功能说明：寻找两个边界的边界点作为八邻域循环的起始点
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------
uint8 get_start_point(uint8 start_row)
{
    uint8 max_check_rows = 5;  // 最多检查35行
    uint8 check_count;         // 已检查的行数
    uint8 current_row;        // 当前检查的行
    uint8 l_flag = 0,r_flag = 0;
	//清零
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y

        // 循环检查5行（或直到图像最大行）
    for (check_count = 0; check_count < max_check_rows; check_count++)
    {
        current_row = start_row + check_count;

        uint8 c;
        uint8 l_found = 0, r_found = 0;  // 当前行的左右边界标记
        uint8 l_col = 0, r_col = 0;      // 当前行找到的左右边界列坐标
        uint8 l_Col[2]={0};
		//从中间往右边，先找起点  //因为摄像头倒置，所以找的是左边的边界点
        //寻左边起点
        for (c = COL - 1  ; c > 0; c--)  // c>0避免c-1越界
        {
            // 条件：当前点是黑色（背景），左侧相邻点（c-1）是白色（线）→ 左边界
            if (Bmp[current_row][c] == Black && Bmp[current_row][c - 1] == White)
            {
                l_col = c;       // 记录右边界列坐标
                l_found = 1;     
                break;                  
            //     if(l_flag==0)                //找起点的优化
            //     {
            //     l_Col[0] = c;       // 记录左边界列坐标
            //     l_flag = 1;                    
            //     }
            //     else if(l_flag==1)
            //     {
            //     l_Col[1] = c;       // 记录左边界列坐标
            //     l_flag = 0;                    
            //     }
            // }
            // if(l_Col[0] != 0 && l_Col[1] == 0 && c <= COL/2)
            // {
            //     l_col = l_Col[0];
            //     l_found = 1;
            //     break;    
            // }
            // else if(l_Col[0] != 0 && l_Col[1] != 0 && (l_Col[0] > l_Col[1]) )     
            // {
            //     l_col = l_Col[1];
            //     l_found = 1;
            //     break;  
            }
            // }
        }

        // -------------------------- 右边界搜索（当前行） --------------------------
        // 从最右侧（c=1）向左侧（c增大）搜索：黑→白跳变（黑色线到白色背景）
        for (c = 0; c < COL - 1; c++)  // c<COL-1避免c+1越界
        {
            // 条件：当前点是黑色（线），左侧相邻点（c-1）是白色（背景）→ 右边界
            if (Bmp[current_row][c + 1] == White && Bmp[current_row][c] == Black)
            {
                r_col = c;       // 记录右边界列坐标
                r_found = 1;     
                break;          
            }
        }

        if (l_found && r_found)
        {
            start_point_l[0] = l_col;    // 左边界列坐标
            start_point_l[1] = current_row;  // 左边界行坐标（当前行）
            start_point_r[0] = r_col;    // 右边界列坐标
            start_point_r[1] = current_row;  // 右边界行坐标（当前行）
            return 1; 
        }     
    }

    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// //  @brief      边缘提取函数（迷宫法） 
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
							uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
example：
	search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
				start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
*/
void search_l_r(uint16 break_flag, uint8(*image)[COL],  uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint16*l_hightest,uint16*r_hightest)
{
    l_stop = 0;
    r_stop = 0;
    // 初始化左右索引
    data_statics_l = 0;        // 统计找到得点数方便后续把点画出来
    data_statics_r = 0;
    
    //左右边界爬取独立标志位
    uint8 left_active = 1;  
    uint8 right_active = 1; 

    //用于标记是否撞到了图像边缘
    RoadFlag.L_left_touch = 0;
    RoadFlag.L_right_touch = 0;
    RoadFlag.L_top_touch = 0;
    RoadFlag.R_left_touch = 0;
    RoadFlag.R_right_touch = 0;
    RoadFlag.R_top_touch = 0;
    
    // 第一次更新坐标点 将找到的起点值传进来
    center_point_l[0] = l_start_x; // x
    center_point_l[1] = l_start_y; // y
    center_point_r[0] = r_start_x; // x
    center_point_r[1] = r_start_y; // y
    
    points_l[data_statics_l][0] = center_point_l[0]; // x
    points_l[data_statics_l][1] = center_point_l[1]; // y
    points_r[data_statics_r][0] = center_point_r[0]; // x
    points_r[data_statics_r][1] = center_point_r[1]; // y

    // 只要还有一边处于活跃状态，且没超出最大循环次数，就继续爬线
    while ((left_active || right_active) && break_flag--) 
    {
        // ======================= 【左边独立爬取】 =======================
        if (left_active) 
        {
            // 1. 左边8F初始化：计算当前中心点周围的8个坐标
            for (uint8 i = 0; i < 8; i++) 
            {
                search_filds_l[i][0] = LIMIT(center_point_l[0] + seeds_l[i][0], 0, CROP_W);
                search_filds_l[i][1] = LIMIT(center_point_l[1] + seeds_l[i][1], 0, CROP_H);
            }

            // 2. 左边生长判断
            data_statics_l++; // 计数 + 1
            index_l = 0;      // 候选点计数清零
            uint8 temp_dir_l[8] = {0}; // 用于临时记录候选点对应的方向 i
            
            for (uint8 i = 0; i < 8; i++) 
            {
                temp_l[i][0] = 0; // 先清零,后续存储找到的候选点x坐标
                temp_l[i][1] = 0; // 先清零,后续存储找到的候选点y坐标
            }
                    // printf("b\n");
            // 遍历8个邻域方向
            for (uint8 i = 0; i < 8; i++) 
            {
                // 判断条件：当前位置是黑，且下一个顺延的位置是白
                if ((Bmp[search_filds_l[i][1]][search_filds_l[i][0]] == Black )
                    && (Bmp[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == White )) 
                {
                    // printf("a\n");
                    // 找到了黑白交界处！这就是边界点
                    temp_l[index_l][0] = search_filds_l[(i)][0];
                    temp_l[index_l][1] = search_filds_l[(i)][1];
                    temp_dir_l[index_l] = i; // 记录候选点的生长方向
                    index_l++;
                }
            }

            if (index_l) 
            {
                // 更新坐标 默认第一个候选点
                center_point_l[0] = temp_l[0][0]; 
                center_point_l[1] = temp_l[0][1];
                dir_l[data_statics_l - 1] = temp_dir_l[0]; 
                
                // 遍历所有候选点
                for (uint8 j = 0; j < index_l; j++) 
                {
                    // 选择y值更大的点，如果新点的y更大，那么就更新 (中心点向高处生长)
                    if (center_point_l[1] < temp_l[j][1]) 
                    {
                        center_point_l[0] = temp_l[j][0]; 
                        center_point_l[1] = temp_l[j][1];
                        dir_l[data_statics_l - 1] = temp_dir_l[j]; // 更新为最佳候选点的方向
                    }
                }
            }

            // 3. 左边状态更新与停止条件判断
            if (center_point_l[0] == Deal_Left) RoadFlag.L_left_touch = 1;
            else if (center_point_l[0] == Deal_Right) RoadFlag.L_right_touch = 1;
            else if (center_point_l[1] == Deal_Top) RoadFlag.L_top_touch = 1;

            // 更新最高点(Y最大值)
            if (center_point_l[1] > *l_hightest) 
            {
                *l_hightest = (uint16)center_point_l[1];
                //topmost[0] = (uint16)center_point_l[0];
                //topmost[1] = (uint16)center_point_l[1];
            }
            // 更新记录最左点
            if (center_point_l[0] < leftmost[0]) 
            {
                leftmost[0] = (uint16)center_point_l[0];
                leftmost[1] = (uint16)center_point_l[1];
            }

            // 储存边界数组
            points_l[data_statics_l][0] = center_point_l[0]; // x
            points_l[data_statics_l][1] = center_point_l[1]; // y

            // 判断左边是否陷入死循环 (连续三次在同一个点)
            if (data_statics_l >= 3 && 
                points_l[data_statics_l-1][0] == points_l[data_statics_l - 2][0] && points_l[data_statics_l-1][0] == points_l[data_statics_l - 3][0] &&
                points_l[data_statics_l-1][1] == points_l[data_statics_l - 2][1] && points_l[data_statics_l-1][1] == points_l[data_statics_l - 3][1]) 
            {
                left_active = 0; // 陷入死点，左边停止
                l_stop = 1;
            }
            // 判断左边是否撞到图像边界
            if (center_point_l[0] == Deal_Left && l_start_x < Deal_Left - 2) 
            {
                left_active = 0; // 撞边界，左边停止
                l_stop = 2;
            }
        }

        // ======================= 【右边独立爬取】 =======================
        if (right_active) 
        {
            // 1. 右边8F初始化：计算当前中心点周围的8个坐标
            for (uint8 i = 0; i < 8; i++) 
            {
                search_filds_r[i][0] = LIMIT(center_point_r[0] + seeds_r[i][0], 0, CROP_W);
                search_filds_r[i][1] = LIMIT(center_point_r[1] + seeds_r[i][1], 0, CROP_H);
                // search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
                // search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
            }

            // 2. 右边生长判断
            data_statics_r++; // 索引加一
            index_r = 0;      // 先清零
            uint8 temp_dir_r[8] = {0}; // 用于临时记录候选点对应的方向 i
            
            for (uint8 i = 0; i < 8; i++) 
            {
                temp_r[i][0] = 0;
                temp_r[i][1] = 0;
            }

            for (uint8 i = 0; i < 8; i++) 
            {
                if ((Bmp[search_filds_r[i][1]][search_filds_r[i][0]] == Black )
                    && (Bmp[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == White )) 
                {
                    temp_r[index_r][0] = search_filds_r[(i)][0];
                    temp_r[index_r][1] = search_filds_r[(i)][1];
                    temp_dir_r[index_r] = i; // 记录候选点的生长方向
                    index_r++;
                }
            }

            if (index_r) 
            {
                // 更新坐标 默认第一个候选点
                center_point_r[0] = temp_r[0][0]; 
                center_point_r[1] = temp_r[0][1];
                dir_r[data_statics_r - 1] = temp_dir_r[0];
                
                for (uint8 j = 0; j < index_r; j++) 
                {
                    // 中心点向高处生长
                    if (center_point_r[1] < temp_r[j][1]) 
                    {
                        center_point_r[0] = temp_r[j][0]; 
                        center_point_r[1] = temp_r[j][1];
                        dir_r[data_statics_r - 1] = temp_dir_r[j]; // 更新为最佳候选点的方向
                    }
                }
            }

            // 3. 右边状态更新与停止条件判断
            if (center_point_r[0] == Deal_Right) RoadFlag.R_right_touch = 1;
            else if (center_point_r[0] == Deal_Left) RoadFlag.R_left_touch = 1;
            else if (center_point_r[1] == Deal_Top) RoadFlag.R_top_touch = 1;

            if (center_point_r[1] > *r_hightest) 
            {
                *r_hightest = (uint16)center_point_r[1];
                //topmost[0] = (uint16)center_point_r[0];
                //topmost[1] = (uint16)center_point_r[1];
            }
            // 更新记录最右点
            if (center_point_r[0] > rightmost[0]) 
            {
                rightmost[0] = (uint16) center_point_r[0];
                rightmost[1] = (uint16) center_point_r[1];
            }

            // 储存边界数组
            points_r[data_statics_r][0] = center_point_r[0]; // x
            points_r[data_statics_r][1] = center_point_r[1]; // y

            // 判断右边是否陷入死循环 (连续三次在同一个点)
            if (data_statics_r >= 3 &&
                points_r[data_statics_r][0] == points_r[data_statics_r-1][0] && points_r[data_statics_r][0] == points_r[data_statics_r - 2][0] &&
                points_r[data_statics_r][1] == points_r[data_statics_r - 1][1] && points_r[data_statics_r][1] == points_r[data_statics_r - 2][1]) 
            {
                right_active = 0; // 陷入死点，右边停止
                r_stop = 1;
            }
            // 判断右边是否撞到图像边界
            if (center_point_r[0] == Deal_Right && r_start_x > Deal_Right + 2) 
            {
                right_active = 0; // 撞边界，右边停止
                r_stop = 2;
            }
        }

        // ======================= 判断相遇 =======================
        // 无论哪边在爬，都要检查两者当前头部是否相遇（距离<=1）
        if (abs(points_r[data_statics_r][0] - points_l[data_statics_l][0]) <= 1 && 
            abs(points_r[data_statics_r][1] - points_l[data_statics_l][1]) <= 1) 
        {
            // 左右相遇退出 可能发生在电感,开关等
            left_active = 0;
            right_active = 0;
            l_stop = 3;
            r_stop = 3;
        }
    }
    data_statics_m = data_statics_l < data_statics_r ? data_statics_l : data_statics_r; // 取较小的统计数作为有效算中线点数
}
/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w]	图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[COL])
{

	uint8 i = 0;
	for (i = 0; i < ROW; i++)
	{
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][COL - 1] = 0;
		image[i][COL - 2] = 0;

	}
	for (i = 0; i < COL; i++)
	{
		image[0][i] = 0;
		image[1][i] = 0;
        image[COL-1][i] = 0;
		image[COL-2][i] = 0;

	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      提取有效左右边界函数
//-------------------------------------------------------------------------------------------------------------------// -------------------------------------------------------------------------------------------------------------------
//  @brief      按行映射原始数组
// -------------------------------------------------------------------------------------------------------------------
void L_R_boundaries(){
    // 1. 初始化数组，归边
    for (uint8 i = 0; i < ROW; i++) {
        L_boundary[i] = COL;
        R_boundary[i] = COL;
    }

    // 2. 映射左边界
    for (uint8 i = 0; i < data_statics_l; i++) {
        uint8 x = (uint8)points_l[i][0];
        uint8 y = (uint8)points_l[i][1];
        
        // 边界保护
        if (y >= 0 && y < ROW && x >= 0 && x < COL) {
            // 第一次遇见直接填写，或者新的 x 比已有的 x 更靠左,越左数值越大
            if (L_boundary[y] == COL || x < L_boundary[y]) {
                L_boundary[y] = x;
            }
        }
    }

    // 3. 映射右边界
    for (uint8 i = 0; i < data_statics_r; i++) {
        uint8 x = (uint8)points_r[i][0];
        uint8 y = (uint8)points_r[i][1];
        
        // 边界保护
        if (y >= 0 && y < ROW && x >= 0 && x < COL) {
            // 第一次遇见直接填写，或者新的 x 比已有的 x 更靠右,越右数值越小
            if (R_boundary[y] == COL || x > R_boundary[y]) {
                R_boundary[y] = x;
            }
        }
    }
}

// -------------------------------------------------------------------------------------------------------------------
//  @brief      按行计算宽度函数
// -------------------------------------------------------------------------------------------------------------------
void Row_Width_Calculate(void)
{
    Max_row = 0;
    Row_Max_Width = 0;
    for (uint8 r = 0; r < ROW; r++)
    {
        if ( L_boundary[r] != COL && R_boundary[r] != COL)
        {
            Width[r] =  L_boundary[r] - R_boundary[r];
        }
        else Width[r] = 0;

        if (Width[r] > Row_Max_Width)
        {
            Row_Max_Width = Width[r];
            Max_row = r;
        }
    }
}
// -------------------------------------------------------------------------------------------------------------------
//  @brief      按点计算宽度函数
// -------------------------------------------------------------------------------------------------------------------
void Point_Width_Calculate(void)
{
    Point_Max_Width = 0;
    for (uint8 r = 0;r < data_statics_m;r++)
    {
        Point_Width[r] =abs(points_l[r][0] - points_r[r][0]);
        if (Point_Width[r] > Point_Max_Width)
        {
            Point_Max_Width = Point_Width[r];
        }
    }
}
// -------------------------------------------------------------------------------------------------------------------
//  @brief      找到点最多行和列,计算这一行和列的点数为多少
// -------------------------------------------------------------------------------------------------------------------
void Find_Max_RCol_Count(void)
{
    uint8 Row_cnt_l[ROW]  = {0};//每行的点数
    uint8 Row_cnt_r[ROW]  = {0};
    uint8 Col_cnt_l[COL]  = {0};//每列的点数
    uint8 Col_cnt_r[COL]  = {0};
    max_row_cnt_l = 0;
    max_row_cnt_r = 0;
    max_cnt_row_l = 0;
    max_cnt_row_r = 0;
    max_col_cnt_l = 0;
    max_col_cnt_r = 0;
    max_cnt_col_l = 0;
    max_cnt_col_r = 0;
    //左边的行列计数
    for (uint8 i = 0; i < data_statics_l;i++)
    {
        uint8 x = (uint8)points_l[i][0];
        uint8 y = (uint8)points_l[i][1];
            Row_cnt_l[y]++; // 统计每行的点数,类似一个直方图
            Col_cnt_l[x]++; // 统计每列的点数
            if (Row_cnt_l[y] > max_row_cnt_l)
            {
                max_row_cnt_l = Row_cnt_l[y];
                max_cnt_row_l = y; // 记录点数最多的行号
            }
            if(Col_cnt_l[x] > max_col_cnt_l)
            {
                max_col_cnt_l = Col_cnt_l[x];
                max_cnt_col_l = x; // 记录点数最多的列号
            }
    }
    //右边的行列计数
    for (uint8 i = 0;i < data_statics_r;i++)
    {
        uint8 x = (uint8)points_r[i][0];
        uint8 y = (uint8)points_r[i][1];
            Row_cnt_r[y]++; // 统计每行的点数,类似一个直方图
            Col_cnt_r[x]++; // 统计每列的点数
            if (Row_cnt_r[y] > max_row_cnt_r)
            {
                max_row_cnt_r = Row_cnt_r[y];
                max_cnt_row_r = y;
            }
            if(Col_cnt_r[x] > max_col_cnt_r)
            {
                max_col_cnt_r = Col_cnt_r[x];
                max_cnt_col_r = x;
            }
    }
    // 加上相邻行的点数，防止赛道没贴直
    max_row_cnt_l += Row_cnt_l[max_cnt_row_l + 1 ] + Row_cnt_l[max_cnt_row_l - 1]; 
    max_row_cnt_r += Row_cnt_r[max_cnt_row_r + 1 ] + Row_cnt_r[max_cnt_row_r - 1];
    max_col_cnt_l += Col_cnt_l[max_cnt_col_l + 1 ] + Col_cnt_l[max_cnt_col_l - 1];
    max_col_cnt_r += Col_cnt_r[max_cnt_col_r + 1 ] + Col_cnt_r[max_cnt_col_r - 1];
}   

//-------------------------------------------------------------------------------------------------------------------
//  @brief      提取有效左右边界函数
//-------------------------------------------------------------------------------------------------------------------

/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L	：找到的点的总数
example： get_left(data_stastics_l);
 */
// void get_left(uint16 total_L)
// {
// 	uint16 j = 0;
// 	uint8 h = 0;
//     uint8 Long_Left=0;
//     //初始化数组
//     std::memset(LeftBlack, COL - 1, sizeof(LeftBlack));
//     h=2;
//     for(j=0;j<total_L;j++)
//     {
//         if((points_l[j][0]!=COL-2||points_l[j][0]!=1) && Long_Left == 0 )
//         {
//             LongLeft=points_l[j][1];
//         }
//         if((points_l[j][0]==COL-2||points_l[j][0]==1) && Long_Left == 0 && LongLeft < points_l[j][1])
//         {
//             LongLeft=points_l[j][1];
//             Long_Left=1;
//         }
//         if(points_l[j][1]==h)
//         LeftBlack[h]=points_l[j][0];
//         else
//         continue;
//         h++;
//         if(h>=ROW-3) {
//         if(Long_Left == 0)
//         {
//             LongLeft=h;
//             Long_Left=1;
//         }
//         break;
//         }
//     }
// }
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
example：get_right(data_stastics_r);
 */
// void get_right(uint16 total_R)
// {
// 	uint16 j = 0;
// 	uint8 h = 0;
//     uint8 Long_Right=0;
// 	//初化
//     std::memset(RightBlack, 0, sizeof(RightBlack));
// 	h = 2;
//     if((points_r[j][0]!=COL-2||points_r[j][0]!=1) && Long_Right == 0 )
//     {
//         LongRight=points_r[j][1];
//     }
//     if((points_r[j][0]==COL-2||points_r[j][0]==1) && Long_Right == 0 && LongRight < points_r[j][1])
//     {
//         LongRight=points_r[j][1];
//         Long_Right=1;
//     }
// 	//右边
// 	for (j = 0; j < total_R; j++)
// 	{
// 		if (points_r[j][1] == h)
// 			RightBlack[h] = points_r[j][0];
// 		else 
//         continue;//每行只取一个点，没到下一行就不记录
//         h++;
// 		if (h == ROW - 3) {
//         if(Long_Right == 0)
//         {
//             LongRight=h;
//             Long_Right=1;
//         }    
// 		break;//到最后一行退出始
//         }
// 	}
// }

//-------------------------------------------------------------------------------------------------------------------
//  @brief      前瞻行左右调转函数
//-------------------------------------------------------------------------------------------------------------------
static void forwardMove(void)
{
    uint8 c = 0;

    WhiteSum = 0;

    for(c = 0; c < COL; c++)
    {
        forwardLook[c] = Bmp[DataInfo.forwardLook][COL - 1 - c];

        //统计前瞻行白点数
        if(forwardLook[c])
            WhiteSum++;
    }
}
// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      前瞻边缘提取函数
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    由于forwardLook_move() 函数将前瞻行调转了，此函数中前瞻的左边界是实际的左边界
// //-------------------------------------------------------------------------------------------------------------------
void Prospect_forwardLook(void)
{
    uint8 i = 0,j = 0;
    LastLeft = Left;
    LastRight = Right;

    //前瞻行左右调转
    forwardMove();

    //前瞻行边界大小调转
    Left = 179 -  L_boundary[DataInfo.forwardLook];
    Right =179 -  R_boundary[DataInfo.forwardLook];

        if(fabs(Left - LastLeft) > 65)
        {
            Left = LastLeft;
        }
        if(fabs(Right - LastRight) > 65)
        {
            Right = LastRight;
        }

        if(Left > Right)
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }
//////检查纠正
        if(Left > Right) //两个判断并非程序写错，可以屏蔽掉其中一个跑一下看现象
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }

        if(fabs(Left - LastLeft) > 80)
        {
            Left = LastLeft;
        }
        if(fabs(Right - LastRight) > 80)
        {
            Right = LastRight;
        }
#if 0
        if(Left > Right) //两个判断并非程序写错，可以屏蔽掉其中一个跑一下看现象
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }
#endif
}

// -------------------------------------------------------------------------------------------------------------------
//  @brief      合成中线函数
// -------------------------------------------------------------------------------------------------------------------
// void Fitting_MiddleLine(void)
// {
//     uint8 r;
//     //左边界长度大于右边界长度
//     if(Longest_White_Column_Left[0]>Longest_White_Column_Right[0])
//     {
//         for(r = 0; r < Longest_White_Column_Right[0]; r++)
//         {
//             MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
//         }
//         for(r = Longest_White_Column_Right[0]; r < Longest_White_Column_Left[0];r++)
//         {
//             MiddleLine[r] = LeftBlack[r] >> 1;
//         }
//     }
//     //右边界长度大于左边界长度
//     else if(Longest_White_Column_Left[0] < Longest_White_Column_Right[0])
//     {
//         for(r = 0; r < Longest_White_Column_Left[0]; r++)
//         {
//             MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
//         }
//         for(r = Longest_White_Column_Left[0]; r < Longest_White_Column_Right[0]; r++)
//         {
//             MiddleLine[r] = (RightBlack[r] + COL - 1) >> 1;
//         }
//     }
//     //左右边界长度相等
//     else
//     {
//         for(r = 0; r < Longest_White_Column_Left[0]; r++)
//         {
//             MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
//         }
//     }

// }
//-------------------------------------------------------------------------------------------------------------------
//  @brief      合成中线函数
//-------------------------------------------------------------------------------------------------------------------
void Fitting_MiddleLine(void)
{   uint8 i,r ;
    memset(Row_Middle_line,0,sizeof(Row_Middle_line));
    //原始边界的中线
    // for (i = 0;i<data_statics_m;i++)
    // {
    //     Middle_line[i][0] = (points_l[i][0] + points_r[i][0]) >> 1;         //记录列 ，就是x坐标
    //     Middle_line[i][1] = (points_l[i][1] + points_r[i][1]) >> 1;         //记录行就是y坐标
    // }
    //按行映射后的中线
    for (r=0;r<ROW;r++)
    {
        if (L_boundary[r] != COL && R_boundary[r] != 0)
        {
            Row_Middle_line[r] = (L_boundary[r] + R_boundary[r]) >> 1;         //记录列 ，就是x坐标
        }
    }
}
// -------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------
/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
函数返回：无
修改时间：2024年2月27日
备    注：
example： image_process();
 */
// -------------------------------------------------------------------------------------------------------------------
void image_process(void)
{
    
      //阈值计算
      //uint32 start_time = system_getval_us();
    //   ImageInfo.threshold = Iterative_Threshold_Opt(mt9v03x_image[0], MT9V03X_H * MT9V03X_W , 1);   //800多us
    //    //二值化
    // Binary_image(mt9v03x_image[0],Bmp[0],MT9V03X_H,MT9V03X_W,ImageInfo.threshold - ImageInfo.threshold_offert); //400us
        image_draw_rectan(Bmp);
       //找到起点了,再执行八领域，没找到就一直找get_start_point(0)
     if ( get_start_point(0) == 1)//找到起点了，再执行八领域，没找到就一直找
    {
        search_l_r((uint16)USE_num, Bmp,  start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &l_highest,&r_highest);   //1800_1000us
        // get_left(data_statics_l);
        // get_right(data_statics_r);

        L_R_boundaries();  //计算边界
        // Row_Width_Calculate(); //计算宽度 更新最大宽度和所在行
        // Point_Width_Calculate(); //计算宽度
        Fitting_MiddleLine();  //拟合中线
        // 前瞻行处理
        Prospect_forwardLook();
       // Find_Max_RCol_Count(); //找到点最多的一行,计算这一行点数为多少 
     }
}
