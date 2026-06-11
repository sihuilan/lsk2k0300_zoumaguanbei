#ifndef CODE_IMAGE_INC_IMAGE_H_
#define CODE_IMAGE_INC_IMAGE_H_



#define COL     CROP_W
#define ROW     CROP_H
#define MID     (COL >> 1)

#define border_min      1
#define border_max      CROP_W-2
#define USE_num	        1000      //400个提取点，看情况修改
// #define MAX_TRACK_STEPS 1000     //最大跟踪步数

// #define ENDROW 16
#define ENDROW DataInfo.forwardLook

#pragma once

extern cv::Mat frame_binary;

extern uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; //左右边界、中线
extern uint8 LeftBlack1[ROW], RightBlack1[ROW]; //左右边界、中线
extern uint8 LongLeft, LongRight;                              //左右有边界行数

extern uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像纵方向白点数

extern uint8 WhiteSum;                                         //前瞻行白点数
extern uint8 LastRight, LastLeft;                              //历史左右边界
extern uint8 Right, Left;                                      //当前左右边界

extern uint8 Width[ROW];
extern uint8 Cross_WhiteCol[COL];

extern uint8 obstacle_L_WhiteRow[ROW];                     //左出弯时行白点数
extern uint8 obstacle_R_WhiteRow[ROW];                     //右出弯时行白点数

extern uint8 Cross_L_WhiteRow[ROW];                        //十字左侧列白点数
extern uint8 Cross_R_WhiteRow[ROW];                        //十字右侧列白点数

extern uint8 Search_Stop_Line;                               //搜索截止行
extern uint8 Longest_White_Column_Left[2];                 //左侧最长白列【0】是白列长度，【1】是下标，第j列
extern uint8 Longest_White_Column_Right[2];                //右侧最长白列【0】是白列长度，【1】是下标，第j列
extern uint8_t Road_HighestPoint;
extern const uint8 Standard_Road_Wide[CROP_H];                 //标准赛宽  //用与坡道识别
void Prospect_Border(void);
void Prospect_Border1(void);
void Fitting_MiddleLine(void);
void CountColWhite(void);
void CountRowWhite(void);
void Cross_CountColWhite(void);
void obstacle_CountRowWhite(void);
void Prospect_forwardLook(void);
uint8 get_start_point(uint8 start_row);
void search_l_r(uint16 break_flag, uint8(*image)[COL], 
                uint16 *l_stastic, uint16 *r_stastic, 
                uint8 l_start_x, uint8 l_start_y, 
                uint8 r_start_x, uint8 r_start_y, 
                uint8*hightest);
void get_left(uint16 total_L);
void get_right(uint16 total_R);
void image_process(void);


#endif