#include "zf_common_headfile.h"

cv::Mat frame_binary;

/*****************************最长白列扫线参数****************/
uint8 forwardLook[COL];                                 //前瞻行像素点反转
uint8 WhiteSum;                                         //前瞻行白点数
uint8 LastRight, LastLeft;                              //历史左右边界
uint8 Right = COL - 1, Left = 0;                        //当前左右边界
uint8 LeftBlack1[ROW], RightBlack1[ROW];                //最长白列的左右边界、中线  
uint8 Search_Stop_Line=0;                               //搜索截止行
uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像横、纵方向白点数
uint8 WhiteRow_Red[ROW],WhiteCol_Red[ROW];                     //存储图像横、纵方向白点数

uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; //左右边界、中线
uint8 LongLeft, LongRight;                              //左右有边界行数

uint8 Longest_White_Column_Left[2]={0};                 //左侧最长白列【0】是白列长度，【1】是下标，第j列
uint8 Longest_White_Column_Right[2]={0};                //右侧最长白列【0】是白列长度，【1】是下标，第j列

/******************************迷宫法巡线参数*****************************/
/************************************************************************/
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
uint16 leftmost[2] ={0};//最左点记录
uint16 rightmost[2] ={0};//最右点记录

uint8 Cross_start_point_l[2] = {0};//十字时屏幕上方的起点x，y值
uint8 Cross_start_point_r[2] = {0};//十字时屏幕上方的起点的x，y值

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
//定义八个生长方向
static int8 seeds_l[8][2] = {{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};
static int8 seeds_r[8][2] = {{0, -1}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1} };

uint16 Point_Width[(uint16)USE_num] = {0};//按点记录的宽度值
uint16 Point_Max_Width = 0;//最大宽度值

//八领域的变量
uint16 L_boundary[ROW] = { 0 };  // 左边界数组
uint16 R_boundary[ROW] = { 0 };  // 右边界数组

uint8 l_stop = 0;      //用于标记左边界停止了八邻域
uint8 r_stop = 0;      //用于标记右边界停止了八邻域

const uint8 Standard_Road_Wide[CROP_H]=                 //标准赛宽这个数组每次调整摄像头和图像都要重新测量  //用与坡道识别
{155,154,152,151,150,149,148,147,146,145,
 143,143,141,140,139,138,136,135,134,132,
 132,130,129,128,126,126,124,122,122,120,
 119,118,116,115,114,112,112,110,108,107,
 106,105,103,102,101,100,98,97,96,95,
 93,92,91,89,88,87,85,84,83,81,
 80,79,77,77,75,73,73,71,69,69,
 67,66,65,63,62,61,59,58,57,56,
 54,53,52,50,48,48,46,44,44,42,
 40,40,38,36,36,34,32,32,30,28,
 27,26,24,23,22};
const uint8 Standard_Haf_Road_Wide[CROP_H]=                 //标准赛宽这个数组每次调整摄像头和图像都要重新测量  //用与坡道识别
{ 78,77,76,76,75,75,74,73,73,72,
  72,71,70,70,69,69,68,67,67,66,
  66,65,64,64,63,63,62,61,61,60,
  60,59,58,58,57,56,56,55,54,54,
  53,52,52,51,50,50,49,48,48,47,
  46,46,45,44,44,43,43,42,41,41,
  40,39,39,38,37,37,36,35,35,34,
  33,33,32,32,31,30,30,29,28,28,
  27,26,26,25,24,24,23,22,22,21,
  20,20,19,18,18,17,16,16,15,14,
  14,13,12,11,11};

struct Edgeline left_edgeline;
struct Edgeline right_edgeline;
/**********************************************************/
/*****************迷宫法巡线参数*****************************/
int Middle_line[(uint16)USE_num][2] ={ { 0 } };//中间线的x，y值
uint16 Row_Middle_line[ROW] = {0};//按行记录的中线x坐标值

//为写好十字补充的变量
int16 L_firstLossY;
int16 R_firstLossY;
int16 L_lossPointsum;
int16 R_lossPointsum;

//-------------------------------------------------------------------------------------------------------------------
// //  @brief      最长白列扫线
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------
void Prospect_Border1(void)
{
    uint8 i=0,j=0;
    // 左右边界、中线归边
    std::memset(MiddleLine, 0, sizeof(MiddleLine));
    std::memset(LeftBlack1, COL - 1, sizeof(LeftBlack));
    std::memset(RightBlack1, 0, sizeof(RightBlack));
    std::memset(Longest_White_Column_Left,0,sizeof(Longest_White_Column_Left));        
    std::memset(Longest_White_Column_Right,0,sizeof(Longest_White_Column_Right));   

   //从左到右找左边最长白列
    Longest_White_Column_Left[0] = 0;
    for(i = COL - 60; i >= 60; i--) // i 这里代表列
    {
        if (Longest_White_Column_Left[0] < WhiteCol[i])
        {
           Longest_White_Column_Left[0] = WhiteCol[i];
            Longest_White_Column_Left[1] = i; 
       }
    }

// 从右到左找最长白列 (同理)
    Longest_White_Column_Right[0] = 0;
    for(i = 60; i <= COL - 60; i++)
    {
        if (Longest_White_Column_Right[0] < WhiteCol[i])
        {
            Longest_White_Column_Right[0] = WhiteCol[i];
            Longest_White_Column_Right[1] = i;
     }
    }
    if(Longest_White_Column_Left[0]>Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止行
    }
    else if(Longest_White_Column_Left[0]<Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
    }
    else if(Longest_White_Column_Left[0]==Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
    }
    // printf("Longest_White_Column_Left[1]- Longest_White_Column_Right[1]= %d\n",Longest_White_Column_Left[1]- Longest_White_Column_Right[1]);
    if(!RoadFlag.ramp)
    {
        for (i = 0; i <Search_Stop_Line; i++)//常规巡线
        {
            //从最下面一行，访问到我的有效是视野行
            for (j = Longest_White_Column_Right[1]; j >= 2; j--)
            {
                if (Bmp[i][j] ==255 && Bmp[i][j - 1] == 0 && Bmp[i][j - 2] == 0)//白黑黑，找到右边界
                {
                    RightBlack1[i]  = j;
                    break;
                }
                else if(j<=2)//没找到右边界，把屏幕最右赋值给右边界
                {
                    RightBlack1[i]  = j-2;
                    break;
                }

            }
            for (j =Longest_White_Column_Left[1] ; j <=COL-3; j++)//往左边扫描
            {//从最下面一行，访问到我的有效是视野行
                if (Bmp[i][j] ==255 && Bmp[i][j + 1] == 0 && Bmp[i][j + 2] == 0)//白黑黑认为到达左边界
                {
                    LeftBlack1[i] = j;
                    break;
                }
                else if(j>=COL-3)
                {
                    LeftBlack1[i] = j+2;//找到头都没找到边，就把屏幕最左右当做边界
                    break;
                }
            }
        }
    }
    else if(RoadFlag.ramp)
    {
        for (i = 0; i <10; i++)//常规巡线
        {
            //从最下面一行，访问到我的有效是视野行
            for (j = Longest_White_Column_Right[1]; j >= 2; j--)
            {
                if (Bmp[i][j] ==255 && Bmp[i][j - 1] == 0 && Bmp[i][j - 2] == 0)//白黑黑，找到右边界
                {
                    RightBlack1[i]  = j;
                    break;
                }
                else if(j<=2)//没找到右边界，把屏幕最右赋值给右边界
                {
                    RightBlack1[i]  = j-2;
                    break;
                }
            }
            for (j =Longest_White_Column_Left[1] ; j <=COL-3; j++)//往左边扫描
            {//从最下面一行，访问到我的有效是视野行
                if (Bmp[i][j] ==255 && Bmp[i][j + 1] == 0 && Bmp[i][j + 2] == 0)//黑黑白认为到达左边界
                {
                    LeftBlack1[i] = j;
                    break;
                }
                else if(j>=COL-3)
                {
                    LeftBlack1[i] = j+2;//找到头都没找到边，就把屏幕最左右当做边界
                    break;
                }
            }
        }        
    }
    // printf("%d\n",Longest_White_Column_Left[1]);
}

//-------------------------------------------------------------------------------------------------------------------
// //  @brief      功能说明：寻找两个边界的边界点作为八邻域循环的起始点
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------
uint8 get_start_point(uint8 start_row)
{
    uint8 max_check_rows = 10;  // 最多检查35行
    uint8 check_count;         // 已检查的行数
    uint8 current_row;        // 当前检查的行
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
        // uint8 l_Col[2]={0};
		//从中间往右边，先找起点  //因为摄像头倒置，所以找的是左边的边界点
        //寻左边起点
        for (c = COL/2  ; c <border_max; c++)  // c>0避免c-1越界
        {
            // 条件：当前点是黑色（背景），左侧相邻点（c-1）是白色（线）→ 左边界
            if (Bmp[current_row][c] == White && Bmp[current_row][c + 1] == Black)
            {
                l_col = c;       // 记录右边界列坐标
                l_found = 1;     
                break;                  
            }
        }
        // -------------------------- 右边界搜索（当前行） --------------------------
        // 从最右侧（c=1）向左侧（c增大）搜索：黑→白跳变（黑色线到白色背景）
        for (c = COL/2 ; c > border_min; c--)  // c<COL-1避免c+1越界
        {
            // 条件：当前点是黑色（线），左侧相邻点（c-1）是白色（背景）→ 右边界
            if (Bmp[current_row][c] == White && Bmp[current_row][c-1] == Black)
            {
                r_col = c;       // 记录右边界列坐标
                r_found = 1;     
                break;          
            }
        }
        //寻左边起点
        // for (c = COL - 1  ; c > 0; c--)  // c>0避免c-1越界
        // {
        //     // 条件：当前点是黑色（背景），左侧相邻点（c-1）是白色（线）→ 左边界
        //     if (Bmp[current_row][c] == Black && Bmp[current_row][c - 1] == White)
        //     {
        //         l_col = c;       // 记录右边界列坐标
        //         l_found = 1;     
        //         break;                  
        //     }
        // }

        // // -------------------------- 右边界搜索（当前行） --------------------------
        // // 从最右侧（c=1）向左侧（c增大）搜索：黑→白跳变（黑色线到白色背景）
        // for (c = 0; c < COL - 1; c++)  // c<COL-1避免c+1越界
        // {
        //     // 条件：当前点是黑色（线），左侧相邻点（c-1）是白色（背景）→ 右边界
        //     if (Bmp[current_row][c + 1] == White && Bmp[current_row][c] == Black)
        //     {
        //         r_col = c;       // 记录右边界列坐标
        //         r_found = 1;     
        //         break;          
        //     }
        // }

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
uint8 find_best_start_points(uint8 start_row, uint8 max_scan_rows)
{
    uint8 l_found = 0; // 左起点锁定标志
    uint8 r_found = 0; // 右起点锁定标志

    // 坐标系(0,0)在右下角，向上看远处行号 y 是递增的
    for (uint8 step = 0; step < max_scan_rows; step++) 
    {
        uint8 current_row = start_row + step;

        // ======================== 【左边独立向上搜索与择优】 ========================
        if (!l_found) 
        {
            EdgeCandidate l_cands[MAX_CANDIDATES];
            uint8 l_count = 0;

            // 1. 全行收集左候选点 
            for (uint8 c = COL - 1 ; c  >= 2; c--) 
            // for (uint8 c = COL/2 ; c < border_max; c++) 
            {
                if (Bmp[current_row][c] == Black && Bmp[current_row][c - 1] == White && Bmp[current_row][c - 2] == White)
                {
                    if (l_count < MAX_CANDIDATES) 
                    {
                        l_cands[l_count].col = c;
                        l_cands[l_count].max_height = 0;
                        l_count++;
                    }
                }
            }

            // 2. 评估左候选点
            if (l_count == 1) 
            {
                start_point_l[0] = l_cands[0].col;
                start_point_l[1] = current_row;
                l_found = 1; // 锁定左边！
            } 
            else if (l_count > 1) 
            {
                uint8 best_index = 0;
                uint16 highest_record = 0; 
                
                for (uint8 i = 0; i < l_count; i++) 
                {
                    uint16 l_highest = 0;
                    uint16 r_highest = 0;
                    
                    // 试探爬线，右边给无效参数(0,0)屏蔽干扰
                    search_l_r((uint16)USE_num, Bmp, l_cands[i].col, current_row, 0, 0, &l_highest, &r_highest,1,1);
                    
                    // 因为是竞争左起点，只比较左边爬出的高度 l_highest
                    if (l_highest > highest_record) 
                    {
                        highest_record = l_highest;
                        best_index = i;
                    }
                }
                
                // 试探完毕，因为y向上是增大的，爬出的最高度必须大于当前行+阈值才算有效
                if (highest_record > current_row + 10) 
                {
                    start_point_l[0] = l_cands[best_index].col;
                    start_point_l[1] = current_row;
                    l_found = 1; // 锁定左边！
                }
            }
        }

        // ======================== 【右边独立向上搜索与择优】 ========================
        if (!r_found) 
        {
            EdgeCandidate r_cands[MAX_CANDIDATES];
            uint8 r_count = 0;

            // 1. 全行收集右候选点 
            for (uint8 c = 0; c < COL - 2; c++)
            // for (uint8 c = COL/2 ; c > border_min; c--)  // c<COL-1避免c+1越界
            {
                if (Bmp[current_row][c] == Black && Bmp[current_row][c + 1] == White && Bmp[current_row][c + 2] == White)
                {
                    if (r_count < MAX_CANDIDATES) 
                    {
                        r_cands[r_count].col = c;
                        r_cands[r_count].max_height = 0;
                        r_count++;
                    }
                }
            }

            // 2. 评估右候选点
            if (r_count == 1) 
            {
                start_point_r[0] = r_cands[0].col;
                start_point_r[1] = current_row;
                r_found = 1; // 锁定右边！
            } 
            else if (r_count > 1) 
            {
                uint8 best_index = 0;
                uint16 highest_record = 0; 
                
                for (uint8 i = 0; i < r_count; i++) 
                {
                    uint16 l_highest = 0;
                    uint16 r_highest = 0;
                    
                    // 试探爬线，左边给无效参数(Deal_Left)屏蔽干扰
                    search_l_r((uint16)USE_num, Bmp, Deal_Left, 0, r_cands[i].col, current_row, &l_highest, &r_highest,1,1);
                    
                    // 因为是竞争右起点，只比较右边爬出的高度 r_highest
                    if (r_highest > highest_record) 
                    {
                        highest_record = r_highest;
                        best_index = i;
                    }
                }
                
                // 同样做高度防伪
                if (highest_record > current_row + 10) 
                {
                    start_point_r[0] = r_cands[best_index].col;
                    start_point_r[1] = current_row;
                    r_found = 1; // 锁定右边！
                }
            }
        }

        // 如果左右两边的起点都已经锁定，就不需要再往上找了，提前结束循环
        if (l_found && r_found) 
        {
            break; 
        }
    }

    // 只要至少有一边找到了可靠起点，就返回 1
    return (l_found || r_found) ? 1 : 0; 
}

uint8 get_Cross_start_point(uint8 start_row)
{
    uint8 max_check_rows = 6;  // 最多检查35行
    uint8 check_count;         // 已检查的行数
    uint8 current_row;        // 当前检查的行
	//清零
	Cross_start_point_l[0] = 0;//x
	Cross_start_point_l[1] = 0;//y

	Cross_start_point_r[0] = 0;//x
	Cross_start_point_r[1] = 0;//y

        // 循环检查5行（或直到图像最大行）
    for (check_count = 0; check_count < max_check_rows; check_count++)
    {
        current_row = start_row - check_count;

        uint8 c;
        uint8 l_found = 0, r_found = 0;  // 当前行的左右边界标记
        uint8 l_col = 0, r_col = 0;      // 当前行找到的左右边界列坐标
        // uint8 l_Col[2]={0};
		//从中间往右边，先找起点  //因为摄像头倒置，所以找的是左边的边界点
        //寻左边起点
        for (c = Search_Stop_Line  ; c <border_max; c++)  // c>0避免c-1越界
        {
            // 条件：当前点是黑色（背景），左侧相邻点（c-1）是白色（线）→ 左边界
            if (Bmp[current_row][c] == White && Bmp[current_row][c + 1] == Black)
            {
                l_col = c;       // 记录右边界列坐标
                l_found = 1;     
                break;                  
            }
        }
        // -------------------------- 右边界搜索（当前行） --------------------------
        // 从最右侧（c=1）向左侧（c增大）搜索：黑→白跳变（黑色线到白色背景）
        for (c = Search_Stop_Line ; c > border_min; c--)  // c<COL-1避免c+1越界
        {
            // 条件：当前点是黑色（线），左侧相邻点（c-1）是白色（背景）→ 右边界
            if (Bmp[current_row][c] == White && Bmp[current_row][c-1] == Black)
            {
                r_col = c;       // 记录右边界列坐标
                r_found = 1;     
                break;          
            }
        }
        if (l_found && r_found)
        {
            Cross_start_point_l[0] = l_col;    // 左边界列坐标
            Cross_start_point_l[1] = current_row;  // 左边界行坐标（当前行）
            Cross_start_point_r[0] = r_col;    // 右边界列坐标
            Cross_start_point_r[1] = current_row;  // 右边界行坐标（当前行）
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
void search_l_r(uint16 break_flag, uint8(*image)[COL],  uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint16* l_extrema, uint16* r_extrema, uint8 is_test, uint8 search_dir)
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

            if (index_l)            //用ai修改代码的部分
            {
                // 默认第一个候选点
                center_point_l[0] = temp_l[0][0]; 
                center_point_l[1] = temp_l[0][1];
                dir_l[data_statics_l - 1] = temp_dir_l[0]; 
                
                for (uint8 j = 0; j < index_l; j++) 
                {
                    // ===【修改点1：防掉头机制】===
                    // 如果候选点是上一次爬过的老坐标，直接跳过，防止在水平线上原地返回
                    if (data_statics_l > 1 && 
                        temp_l[j][0] == points_l[data_statics_l - 2][0] && 
                        temp_l[j][1] == points_l[data_statics_l - 2][1]) 
                    {
                        continue;
                    }

                    // ===【修改点2：融合生长方向判定】===
                    // search_dir == 1 时向上(Y更大)；search_dir == 0 时向下(Y更小)
                    uint8 condition = search_dir ? (center_point_l[1] < temp_l[j][1]) : (center_point_l[1] > temp_l[j][1]);
                    
                    if (condition) 
                    {
                        center_point_l[0] = temp_l[j][0]; 
                        center_point_l[1] = temp_l[j][1];
                        dir_l[data_statics_l - 1] = temp_dir_l[j]; 
                    }
                }
            }

            // 3. 左边状态更新与停止条件判断
            if (center_point_l[0] == Deal_Left) RoadFlag.L_left_touch = 1;
            else if (center_point_l[0] == Deal_Right) RoadFlag.L_right_touch = 1;
            
            // ===【修改点3：融合撞线停止判断】===
            else if (search_dir == 1 && center_point_l[1] == Deal_Top) RoadFlag.L_top_touch = 1;
            // else if (search_dir == 0 && center_point_l[1] == Deal_Bottom) RoadFlag.L_bottom_touch = 1; // 碰到底部边界

            // ===【修改点4：融合极值记录】===
            if (search_dir == 1) {
                if (center_point_l[1] > *l_extrema) *l_extrema = (uint16)center_point_l[1];
            } else {
                if (center_point_l[1] < *l_extrema) *l_extrema = (uint16)center_point_l[1];
            }

            // 更新记录最左点
            if (center_point_l[0] < leftmost[0]) 
            {
                leftmost[0] = (uint16)center_point_l[0];
                leftmost[1] = (uint16)center_point_l[1];
            }

            points_l[data_statics_l][0] = center_point_l[0]; 
            points_l[data_statics_l][1] = center_point_l[1]; 

            // 判断死循环 (连续三次在同一个点)
            if (data_statics_l >= 3 && 
                points_l[data_statics_l-1][0] == points_l[data_statics_l - 2][0] && points_l[data_statics_l-1][0] == points_l[data_statics_l - 3][0] &&
                points_l[data_statics_l-1][1] == points_l[data_statics_l - 2][1] && points_l[data_statics_l-1][1] == points_l[data_statics_l - 3][1]) 
            {
                left_active = 0; 
                l_stop = 1;
            }
            // 判断撞到图像边界
            if (center_point_l[0] == Deal_Left && l_start_x < Deal_Left - 2) 
            {
                left_active = 0; 
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
                center_point_r[0] = temp_r[0][0]; 
                center_point_r[1] = temp_r[0][1];
                dir_r[data_statics_r - 1] = temp_dir_r[0];
                
                for (uint8 j = 0; j < index_r; j++) 
                {
                    // ===【修改点1：防掉头机制】===
                    if (data_statics_r > 1 && 
                        temp_r[j][0] == points_r[data_statics_r - 2][0] && 
                        temp_r[j][1] == points_r[data_statics_r - 2][1]) 
                    {
                        continue;
                    }

                    // ===【修改点2：融合生长方向判定】===
                    uint8 condition = search_dir ? (center_point_r[1] < temp_r[j][1]) : (center_point_r[1] > temp_r[j][1]);
                    
                    if (condition) 
                    {
                        center_point_r[0] = temp_r[j][0]; 
                        center_point_r[1] = temp_r[j][1];
                        dir_r[data_statics_r - 1] = temp_dir_r[j]; 
                    }
                }
            }

            // 3. 右边状态更新与停止条件判断
            if (center_point_r[0] == Deal_Right) RoadFlag.R_right_touch = 1;
            else if (center_point_r[0] == Deal_Left) RoadFlag.R_left_touch = 1;
            
            // ===【修改点3：融合撞线停止判断】===
            else if (search_dir == 1 && center_point_r[1] == Deal_Top) RoadFlag.R_top_touch = 1;
            // else if (search_dir == 0 && center_point_r[1] == Deal_Bottom) RoadFlag.R_bottom_touch = 1;

            // ===【修改点4：融合极值记录】===
            if (search_dir == 1) {
                if (center_point_r[1] > *r_extrema) *r_extrema = (uint16)center_point_r[1];
            } else {
                if (center_point_r[1] < *r_extrema) *r_extrema = (uint16)center_point_r[1];
            }

            // 更新记录最右点
            if (center_point_r[0] > rightmost[0]) 
            {
                rightmost[0] = (uint16) center_point_r[0];
                rightmost[1] = (uint16) center_point_r[1];
            }

            points_r[data_statics_r][0] = center_point_r[0]; 
            points_r[data_statics_r][1] = center_point_r[1]; 

            // 判断死循环 (连续三次在同一个点)
            if (data_statics_r >= 3 &&
                points_r[data_statics_r][0] == points_r[data_statics_r-1][0] && points_r[data_statics_r][0] == points_r[data_statics_r - 2][0] &&
                points_r[data_statics_r][1] == points_r[data_statics_r - 1][1] && points_r[data_statics_r][1] == points_r[data_statics_r - 2][1]) 
            {
                right_active = 0; 
                r_stop = 1;
            }
            // 判断撞到图像边界
            if (center_point_r[0] == Deal_Right && r_start_x > Deal_Right + 2) 
            {
                right_active = 0; 
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
	// for (i = 0; i < COL; i++)
	// {
	// 	image[0][i] = 0;
	// 	image[1][i] = 0;
    //     image[COL-1][i] = 0;
	// 	image[COL-2][i] = 0;

	// }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计列白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按列扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountColWhite(void)
{
    uint8 r, c;
    //列白点数组清0

    memset(WhiteCol, 0, sizeof(WhiteCol));

    for(c = 2; c < COL-2; c++)
    {
        for(r = 0; r < ROW; r++)
        {
            if(Bmp[r][c])       WhiteCol[c]++;
            else                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计行白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按行扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountRowWhite(void)
{
    uint8 r, c;
    uint8 *p = NULL;

    //行白点数组清0
    memset(WhiteRow, 0, sizeof(WhiteRow));
    // memset(Cross_L_WhiteRow, 0, sizeof(Cross_L_WhiteCol));
    // memset(Cross_R_WhiteRow, 0, sizeof(Cross_R_WhiteCol));

    for(r = 0; r < ROW; r++)
    {
        p = &Bmp[r][0];
        for(c = MID; c > 0; c--)
        {
            if(*(p + c) == 255)       
            {
                WhiteRow[r]++;
                // Cross_L_WhiteRow[r]++;
            }
                else                
                break;
        }
        for(c = MID + 1; c < COL; c++)
        {
            if(*(p + c) == 255)
            {
                WhiteRow[r]++;
                // Cross_R_WhiteRow[r]++;
            }
            else 
            break;
        }
    }
}
void Red_CountRowWhite(void)
{
    uint8 r, c;
    uint8 *p = NULL;

    //行白点数组清0
    memset(WhiteRow_Red, 0, sizeof(WhiteRow_Red));
    // memset(Cross_L_WhiteRow, 0, sizeof(Cross_L_WhiteCol));
    // memset(Cross_R_WhiteRow, 0, sizeof(Cross_R_WhiteCol));

    for(r = 0; r < ROW; r++)
    {
        p = &Bmp[r][0];
        for(c = L_boundary[r]; c > MID; c--)
        {
            if(*(p + c) == 255)       
            {
                WhiteRow_Red[r]++;
                // Cross_L_WhiteRow[r]++;
            }
                else                
                break;
        }
        for(c = R_boundary[r]; c < MID; c++)
        {
            if(*(p + c) == 255)       
            {
                WhiteRow_Red[r]++;
                // Cross_L_WhiteRow[r]++;
            }
                else                
                break;
        }

    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      提取有效左右边界函数
//-------------------------------------------------------------------------------------------------------------------// -------------------------------------------------------------------------------------------------------------------
//  @brief      按行映射原始数组
// -------------------------------------------------------------------------------------------------------------------
void L_R_boundaries(){
    // 1. 初始化数组
    uint8 left_cnt = 0;
    uint8 right_cnt = 0;
    uint8 Loss_pointSum_l = 0;
    uint8 Loss_pointSum_r = 0;

    uint8 firstloss_flag_l = 0;
    uint8 firstloss_flag_r = 0;
    L_lossPointsum = 0;
    R_lossPointsum = 0;

    left_edgeline.firstLossY[1] = 0;
    right_edgeline.firstLossY[1] = 0;

    for (uint8 i = 0; i < ROW; i++) {
        L_boundary[i] = border_max;
        R_boundary[i] = border_min;
    }

    // 2. 映射左边界
    for (uint8 i = 0; i < data_statics_l; i++) {
        uint8 x = (uint8)points_l[i][0];
        uint8 y = (uint8)points_l[i][1];
        
        // 边界保护
        if (y >= 0 && y < ROW && x >= 0 && x < COL) {
            // 选更靠近中间的点
            if ( L_boundary[y] == border_max || x < L_boundary[y]) {
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
            // 选更靠近中间的点
            if (R_boundary[y] == border_min || x > R_boundary[y]) {
                R_boundary[y] = x;
            }
        }
    }
    // 4. 统计有效边界的行数

    for (uint8 i = 0; i < ROW; i++) {
        if(firstloss_flag_l == 0) {  
            if (L_boundary[i] == border_max && L_boundary[i+1] == border_max) {
                left_edgeline.firstLossY[0] = i;
                left_edgeline.firstLossY[1] = 1;
                firstloss_flag_l = 1;} 
            } 

        if (L_boundary[i] != border_max)   left_cnt++; // 统计左边有效行      
        // if (L_boundary[i] == border_max)   Loss_pointSum_l++;  
        if(L_boundary[i] != border_max && L_boundary[i+1] == border_max && L_boundary[i+2] == border_max)   break;
    }

    for (uint8 i = 0; i < ROW; i++) {
        if(firstloss_flag_r == 0) {  
            if (R_boundary[i] == border_min && R_boundary[i+1] == border_min) {
                right_edgeline.firstLossY[0] = i;
                right_edgeline.firstLossY[1] = 1;
                firstloss_flag_r = 1;} 
            } 

        if (R_boundary[i] != border_min)    right_cnt++; // 统计右边有效行
        // if (R_boundary[i] == border_min)    Loss_pointSum_r++; 
        if(R_boundary[i] != border_min && R_boundary[i+1] == border_min && R_boundary[i+2] == border_min)   break;
    }

    for (uint8 i = 0; i < ROW; i++) {  
        //左长右短
        if (L_boundary[i] != border_max && R_boundary[i] == border_min) {
            MiddleLine[i] = L_boundary[i] - Standard_Haf_Road_Wide[i]; 
        }
        //左短右长
        else if (L_boundary[i] == border_max && R_boundary[i] != border_min) {
                MiddleLine[i] = R_boundary[i] + Standard_Haf_Road_Wide[i]; 
        } 
        else 
        {
            MiddleLine[i] = (L_boundary[i] + R_boundary[i]) / 2;
        }
    }
    // 5. 最终一次性赋值给全局变量
    LongLeft = left_cnt;
    LongRight = right_cnt;
    // left_edgeline.losspoint  = Loss_pointSum_l;
    // right_edgeline.losspoint = Loss_pointSum_r;
    // printf("l = %d ,r = %d\n",LongLeft,LongRight);        
    // // printf("l = %d ,r = %d\n",left_edgeline.losspoint,LongRight,right_edgeline.losspoint);      
    // printf("l = %d ,r = %d\n",left_edgeline.firstLossY[0],right_edgeline.firstLossY[0]);   
    // printf("l_flag = %d ,r_flag = %d\n",left_edgeline.firstLossY[1],right_edgeline.firstLossY[1]);     

}
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
    LastLeft = Left;
    LastRight = Right;

    //前瞻行左右调转
    forwardMove();

    Left = COL - 1 - L_boundary[DataInfo.forwardLook];
    Right =COL - 1 - R_boundary[DataInfo.forwardLook]; 

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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      合成中线函数
//-------------------------------------------------------------------------------------------------------------------
void Fitting_MiddleLine(void)
{   
    uint8 i,r ;
    memset(Row_Middle_line,0,sizeof(Row_Middle_line));
    // 原始边界的中线
    for (i = 0;i<data_statics_m;i++)
    {
        Middle_line[i][0] = (points_l[i][0] + points_r[i][0]) >> 1;         //记录列 ，就是x坐标
        Middle_line[i][1] = (points_l[i][1] + points_r[i][1]) >> 1;         //记录行就是y坐标
    }
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
        image_draw_rectan(Bmp);
        CountColWhite();
        CountRowWhite();
        Red_CountRowWhite();
        Prospect_Border1();
       //找到起点了,再执行八领域，没找到就一直找get_start_point(0)
    //    find_best_start_points(0,)
     if ( find_best_start_points(0,Middle_Row) == 1)//找到起点了，再执行八领域，没找到就一直找
    {
        // printf("l = (%d,%d)\n",start_point_l[0],start_point_l[1]);
        // printf("r = (%d,%d)\n",start_point_r[0],start_point_r[1]);
        search_l_r((uint16)USE_num, Bmp,  start_point_l[0], start_point_l[1], start_point_r[0], start_point_r[1], &l_highest,&r_highest,0,1);   //1800_1000us
        // search_l_r((uint16)USE_num, Bmp,  Cross_start_point_l[0], Cross_start_point_l[1], Cross_start_point_r[0], Cross_start_point_r[1], &l_highest,&r_highest,0,0);   //1800_1000us
        L_R_boundaries();  //计算边界

        // // 前瞻行处理
        Prospect_forwardLook();
        Fitting_MiddleLine();  //拟合中线
     }
}
