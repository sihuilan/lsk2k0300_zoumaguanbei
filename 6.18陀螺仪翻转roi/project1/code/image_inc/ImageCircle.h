#ifndef CODE_IMAGE_INC_IMAGECIRCLE_H_
#define CODE_IMAGE_INC_IMAGECIRCLE_H_



#define  CIR_STRAIGHT   55      //切割行
#define  CIRCLE_COL     16      //圆环正常赛道列数
#define  CIRCLE_WHITE   80      //圆环中弯道白点数
#define  Release_Left   45      //右环岛释放正常巡线的左值
#define  Release_Right  90      //左环岛释放正常巡线的右值

#pragma once


extern uint8 CircleTime;
extern void ImageCircle_init(void);
extern void Check_Circle(void);
extern void Circle_Control(void);

#endif