#ifndef CODE_IMAGE_INC_IMAGECIRCLE_H_
#define CODE_IMAGE_INC_IMAGECIRCLE_H_



#define  CIR_STRAIGHT   70      //切割行
#define  CIRCLE_COL     16      //圆环正常赛道列数
#define  CIRCLE_WHITE   80      //圆环中弯道白点数
#define  Release_Left   45      //右环岛释放正常巡线的左值
#define  Release_Right  90      //左环岛释放正常巡线的右值

#pragma once

extern uint16 v_point_l_index; 
extern uint16 v_point_r_index;

uint8 point_check(void);
void IntoRing_buxian(void);
void OuttoRing_buxian(void);

extern uint8 CircleTime;
extern void ImageCircle_init(void);
extern void Check_Circle(void);
extern void Circle_Control(void);

// void buxian(void);
#endif