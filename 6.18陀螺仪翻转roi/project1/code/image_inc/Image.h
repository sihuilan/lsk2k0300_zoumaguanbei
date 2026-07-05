#ifndef CODE_IMAGE_INC_IMAGE_H_
#define CODE_IMAGE_INC_IMAGE_H_



#define COL     CROP_W
#define ROW     CROP_H
#define MID     (COL >> 1)
#define IMAGE_ROW 75

#define LIMIT(val, min, max)        (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))

#define Middle_Row  10
#define Deal_Left   COL - 1    //定义最左边界
#define Deal_Right  0              //定义最右边界
#define Deal_Top    75//????  

#define Deal_Bottom 0

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

struct Edgeline {
    int16_t losspoint;
    int16_t firstLossY[2];
};

extern struct Edgeline left_edgeline;
extern struct Edgeline right_edgeline;

extern cv::Mat frame_binary;
extern const uint8 Standard_Haf_Road_Wide[CROP_H];
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
extern uint8 l_stop;
extern uint8 r_stop;
extern uint16 L_boundary[ROW];  // 左边界数组
extern uint16 R_boundary[ROW];  // 右边界数组
extern uint16 Row_Middle_line[ROW]; //按行记录的中线x坐标值
extern uint16 dir_r[(uint16)USE_num];//用来存储右边生长方向
extern uint16 dir_l[(uint16)USE_num];//用来存储左边生长方向

extern int16 L_firstLossY;
extern int16 R_firstLossY;
extern int16 L_lossPointsum;
extern int16 R_lossPointsum;

/*********************************八邻域变量区*********************************/  
extern uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW];
extern uint8 WhiteRow[ROW], WhiteCol[COL];    
extern uint8 LeftBlack1[ROW], RightBlack1[ROW]; //最长白列的左右边界、中线

extern uint8 Longest_White_Column_Left[2];                 //左侧最长白列【0】是白列长度，【1】是下标，第j列
extern uint8 Longest_White_Column_Right[2];                //右侧最长白列【0】是白列长度，【1】是下标，第j列
extern uint8 Search_Stop_Line;

extern uint8 Right, Left;
extern uint8 LastRight, LastLeft; 
extern uint8 LongLeft, LongRight;   
extern uint8 WhiteSum;  
extern uint8 get_start_point(uint8 start_row);


void Prospect_Border1(void);
void search_l_r(uint16 break_flag, uint8(*image)[COL],  uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint16* l_extrema, uint16* r_extrema, uint8 is_test, uint8 search_dir);
void L_R_boundaries(void);
void image_draw_rectan(uint8(*image)[COL]);
void Fitting_MiddleLine(void); 
void losspoint_Calculate(void);
void image_process(void);
void Prospect_forwardLook(void);
uint8 find_best_start_points(uint8 start_row, uint8 max_scan_rows);


#endif