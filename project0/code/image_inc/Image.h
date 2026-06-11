#ifndef CODE_IMAGE_INC_IMAGE_H_
#define CODE_IMAGE_INC_IMAGE_H_



#define COL     CROP_W
#define ROW     CROP_H
#define MID     (COL >> 1)
#define IMAGE_ROW 75

#define LIMIT(val, min, max)        (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

#define Middle_Row  20
#define Deal_Left   COL - 1    //定义最左边界
#define Deal_Right  0              //定义最右边界
#define Deal_Top    75//????  

#define border_min      1
#define border_max      CROP_W-2
#define USE_num	        250      //400个提取点，看情况修改
// #define MAX_TRACK_STEPS 1000     //最大跟踪步数
#define MAX_CANDIDATES 2  // 单行最多记录几个候选点
#define MAX_MEET_JUMPS 3  //最多相遇跳行拼接次数
// #define ENDROW 16
#define ENDROW DataInfo.forwardLook

#define White     255
#define Black     0

#pragma once

typedef struct {
    uint8 col;
    uint16 max_height;
} EdgeCandidate;

typedef struct{
    uint16 x;
    uint16 y;
}Edge;

enum{
    Middle_edge = 0,
    Left_edge,
    Right_edge,
    half_left_edge,
    half_right_edge
};
extern cv::Mat frame_binary;
/*********************************八邻域变量区*********************************/
extern int points_l[(uint16)USE_num][2] ;//左线
extern int points_r[(uint16)USE_num][2] ;//右线
extern int Middle_line[(uint16)USE_num][2];//中线
extern uint16 data_statics_l ;//统计左边找到的点
extern uint16 data_statics_r ;//统计右边找到的点
extern uint16 data_statics_m ;//统计中线找到的点
extern int center_point_l[2];//八邻域中心坐标点
extern int center_point_r[2] ;//八邻域中心坐标点`
extern uint8 start_point_r[2] ; //右边起点的x，y值
extern uint8 start_point_l[2] ; //左边起点的x，y值
extern uint16 leftmost[2];//最左点记录
extern uint16 rightmost[2];//最右点记录
extern uint16 topmost[2] ;//最顶点记录
extern uint8 l_stop;
extern uint8 r_stop;
extern uint16 L_boundary[ROW];  // 左边界数组
extern uint16 R_boundary[ROW];  // 右边界数组
extern uint16 Row_Middle_line[ROW]; //按行记录的中线x坐标值
/*********************************八邻域变量区*********************************/  
extern uint8 max_cnt_row_l; //点数最多的行
extern uint8 max_cnt_row_r;
extern uint8 max_cnt_col_l; //点数最多的列
extern uint8 max_cnt_col_r;
extern uint8 max_row_cnt_l;//点数最多的行的计数
extern uint8 max_row_cnt_r;
extern uint8 max_col_cnt_l;//点数最多的列的计数
extern uint8 max_col_cnt_r;

extern uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW];
extern uint8 WhiteRow[ROW], WhiteCol[COL];    
extern uint8 Right, Left;
extern uint8 LastRight, LastLeft; 
extern uint8 LongLeft, LongRight;   
extern uint8 WhiteSum;  
extern uint8 get_start_point(uint8 start_row);
void search_l_r(uint16 break_flag, uint8(*image)[COL],  uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint16*l_hightest,uint16*r_hightest, uint8 is_test);
void L_R_boundaries(void);
void image_draw_rectan(uint8(*image)[COL]);
void Fitting_MiddleLine(void); 
void Row_Width_Calculate(void);
void Point_Width_Calculate(void);
void Find_Max_RCol_Count(void);
void image_process(void);
void Prospect_forwardLook(void);
uint8 find_best_start_points(uint8 start_row, uint8 max_scan_rows);


#endif