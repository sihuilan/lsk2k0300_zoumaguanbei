#ifndef CODE_INC_DISPLAY_H_
#define CODE_INC_DISPLAY_H_



#define KEY_0       "/dev/zf_driver_gpio_key_0"
#define KEY_1       "/dev/zf_driver_gpio_key_1"
#define KEY_2       "/dev/zf_driver_gpio_key_2"
#define KEY_3       "/dev/zf_driver_gpio_key_3"
#define KEY_4       "/dev/zf_driver_gpio_key_4"


// //按键
// #define     UP      gpio_get_level(KEY_2)   //KEY_1  KEY_1
// #define     DOWN    gpio_get_level(KEY_1)   //KEY_2  KEY_2
// #define     LEFT    gpio_get_level(KEY_0)   //KEY_0  KEY_3
// #define     RIGHT   gpio_get_level(KEY_4)   //KEY_3  KEY_4
// #define     SET     gpio_get_level(KEY_3)   //KEY_4  KEY_0

#define     RIGHT     gpio_get_level(KEY_2)   //KEY_1  KEY_1
#define     LEFT       gpio_get_level(KEY_1)   //KEY_2  KEY_2
#define     UP        gpio_get_level(KEY_0)   //KEY_0  KEY_3
#define     DOWN       gpio_get_level(KEY_4)   //KEY_3  KEY_4
#define     SET       gpio_get_level(KEY_3)   //KEY_4  KEY_0

#define    Screen_Width   252
#define    Screen_Height  75

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      需要调参部分
//  @explain    1、增减页面需要修改PAGE_MAX宏定义
//              2、修改某一页的参数显示行数、列数只需在PageXPara_ROW、PageXPara_COL后修改
//              3、增减页面需要增减PageXPara_ROW、PageXPara_COL宏定义，同时需要将其增减至MaxSelect数组中
//              4、在调参参数结构体中增减相关内容，添加页面需要将以下内容复制到结构体中
//                {{"Para2_0", "Para2_1", "Para2_2", "Para2_3", "Para2_4", "Para2_5", "Para2_6", "Para2_7"},  //参数名
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL}}                                                //第2列参数地址
//              （注意：参数名数组需要初始化八个，无内容则需初始化为"0"，void指针需要初始化七个，无内容则需初始化为NULL）
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------


//显示页数
#define PAGE_MAX  8

// 单页最大行数
#define  MAX_ROW  9

//每页参数行数（最大行数为8）
#define Page0Para_ROW  4
#define Page1Para_ROW  2
#define Page2Para_ROW  1
#define Page3Para_ROW  8//5
#define Page4Para_ROW  8//5
#define Page5Para_ROW  8//5
#define Page6Para_ROW  8
#define Page7Para_ROW  8
#define Page8Para_ROW  8
#define Page9Para_ROW  8

//每页参数列数（最大列数为2）
#define Page0Para_COL  1
#define Page1Para_COL  1
#define Page2Para_COL  1
#define Page3Para_COL  2
#define Page4Para_COL  2
#define Page5Para_COL  2
#define Page6Para_COL  2
#define Page7Para_COL  2
#define Page8Para_COL  2
#define Page9Para_COL  2
//-------------------------------------------------------------------------------------------------------------------
//  @brief      使用宏定义对参数进行自加自减
//  @param      Parameter       需要写入的参数
//  @param      Increase        需要加减的数值
//  @param      type            加减参数的类型（只能为int或float）
//  Sample usage:               ParameterIncrease(*Page0Para[Channel], Precision[PrecisionSelect] ,int);
//-------------------------------------------------------------------------------------------------------------------
#define     ParameterIncrease(Parameter, Increase, type)    (Parameter += (type)(Increase))
#define     ParameterDecrease(Parameter, Decrease, type)    (Parameter -= (type)(Decrease))


// 参数相关数据结构体
class ParameterType {
public:
    char ParaName[MAX_ROW + 1][18];
    void* ParaOne[MAX_ROW];
    void* ParaTwo[MAX_ROW];
};

#pragma once

// 全局变量
extern std::atomic<bool> Start;
extern uint8_t CountStartTime;
extern uint8_t StopTime;    

void Key_Scan(void);
void IPS200_display(void);
void draw_single_boundary(int points[][2], int point_count, uint16 color, int x_offset);
void draw_row_boundary(uint16 boundary[], uint16 color, int x_offset);
void Key_Text(void);
void key_test(void);

#endif