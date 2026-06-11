#ifndef CODE_IMAGE_INC_IMAGECIRCLE_H_
#define CODE_IMAGE_INC_IMAGECIRCLE_H_



#define  CIR_STRAIGHT   45      //切割行
#define  CIRCLE_COL     16      //圆环正常赛道列数
#define  CIRCLE_WHITE   80      //圆环中弯道白点数
#define  Release_Left   45      //右环岛释放正常巡线的左值
#define  Release_Right  90      //左环岛释放正常巡线的右值

#pragma once

typedef struct
{
        char Circle_size1;
        char Circle_size2;
        char Circle_size3;
        char Circle_size4;
        char CircleSize;
        //R50参数
        float Circle_R50In;
        float Circle_R50Out;
        //R60
        float Circle_R60In;
        float Circle_R60Out;
        //R80
        float Circle_R80In;
        float Circle_R80Out;
        //R100
        float Circle_R100In;
        float Circle_R100Out;

        //R60和R80PD，R60和R50PD可通用， R80和R100PD可通用
        float CircleR50_LP;
        float CircleR50_LD;
        float CircleR50_RP;
        float CircleR50_RD;

        float CircleR60_LP;
        float CircleR60_RP;
        float CircleR60_D;

        float CircleR80_EP;
        float CircleR80_ED;
        float CircleR80_P;
        float CircleR80_D;

//        float CircleR100_EP;
//        float CircleR100_ED;
        float CircleR100_P;
//        float CircleR100_D;

        //辅助定距
        float CircleR50_Dis;
        float CircleR60_Dis;
        float CircleR80_Dis;
        float CircleR100_Dis;

        //圆环速度
        float Speed_R50;
        float Speed_R60;
        float Speed_R80;

}ImageCircleType;

extern ImageCircleType Circle;

extern ImageCircleType Circle;
extern uint8 CircleTime;
extern void ImageCircle_init(void);
extern void Check_Circle(void);
extern void Circle_Control(void);

#endif