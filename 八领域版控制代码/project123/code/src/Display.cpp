#include "zf_common_headfile.h"
// #include "Display.h"


//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//  @brief      需要调参部分
//  @explain    1、增减页面需要修改PAGE_MAX宏定义
//              2、修改某一页的参数显示行数、列数只需在PageXPara_ROW、PageXPara_COL后修改
//              3、增减页面需要增减PageXPara_ROW、PageXPara_COL宏定义，同时需要将其增减至MaxSelect数组中 //这部分代码修改到Display.h中了
//              4、在调参参数结构体中增减相关内容，添加页面需要将以下内容复制到结构体中
//                {{"Para2_0", "Para2_1", "Para2_2", "Para2_3", "Para2_4", "Para2_5", "Para2_6", "Para2_7"},  //参数名
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
//                 {NULL, NULL, NULL, NULL, NULL, NULL, NULL}}                                                //第2列参数地址
//              （注意：参数名数组需要初始化八个，无内容则需初始化为"0"，void指针需要初始化七个，无内容则需初始化为NULL）
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------


//每页参数可选的最大值
const uint8 MaxSelect[PAGE_MAX][2] =
{
    {Page0Para_ROW, Page0Para_COL},
    {Page1Para_ROW, Page1Para_COL},
    {Page2Para_ROW, Page2Para_COL},
    {Page3Para_ROW, Page3Para_COL},
    {Page4Para_ROW, Page4Para_COL},
    {Page5Para_ROW, Page5Para_COL},
    {Page6Para_ROW, Page6Para_COL},
    {Page7Para_ROW, Page7Para_COL}

};

//相关调参参数初始化
//注意：参数名数组需要初始化八个，无内容则需初始化为"0"，void指针需要初始化七个，无内容需则初始化为NULL
struct ParameterType Parameter[PAGE_MAX] =
{
    //第一页
    {{"GOGOGO!!!", "Watch_Viode", "", "", "", "Para0_5", "Para0_6", "Para0_7"},   //参数名
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                              //第1列参数地址
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                                                       //第2列参数地址
    //第二页
    {{"ForwardLook", " Obstacle_line", "Save_Data", "Para1_3", "Para1_4", "Para1_5", "Para1_6", "Para1_7"},      //参数名
     {&DataInfo.forwardLook, &DataInfo.Obstacle_line, NULL, NULL, NULL, NULL, NULL},  //第1列参数地址
     {NULL,                  NULL, NULL, NULL, NULL, NULL, NULL}},                                                                   //第2列参数地址
    //第三页
    {{"Para2_0", "Para2_1", "Para2_2", "Para2_3", "Para2_4", "Para2_5", "Para2_6", "Para2_7"},  //参数名
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL},                                                //第1列参数地址
     {NULL, NULL, NULL, NULL, NULL, NULL, NULL}},                                               //第2列参数地址
    //第四页
     {{"",       "",          "ADC_ET_P",         "Ramp_P_line",        "Breline_SpeedZbr",    "obstleP_Speed",         "Save_Data", "Save_Data"},      //参数名
      {NULL, NULL, NULL,  NULL ,     NULL,     &DataInfo.obstacleP,     NULL},           //第1列参数地址
      {NULL, NULL, NULL,  NULL ,  &DataInfo.zebraSpeed, &DataInfo.obstacleSpeed, NULL}},                         //第2列参数地址
     //第五页
     {{"R50_LR_P",           "Speed_R50_R100P",    "R60_LR_P",           "R80_ET_P",           "Speed_R60_R80",  "Image_str_cur",        "Save_Data", "Save_Data"},  //参数名
      {&Circle.CircleR50_LP, &Circle.Speed_R50,    &Circle.CircleR60_LP, &Circle.CircleR80_EP, &Circle.Speed_R60, &DataInfo.image_straightSpeed,  NULL},    //第1列参数地址
      {&Circle.CircleR50_RP, &Circle.CircleR100_P, &Circle.CircleR60_RP, &Circle.CircleR80_P,  &Circle.Speed_R80, &DataInfo.image_curSpeed,  NULL}},    //第2列参数地址
      //第六页
      {{ " ",              "Imge_tra_ramp",               "AD_tra_cur",              "CircleSize_12",      "CircleSize_34",      "Speed_G0_Time",   "Save_Data", "Save_Data"},  //参数名
        {NULL,          NULL,           NULL,                 &Circle.Circle_size1, &Circle.Circle_size3, &DataInfo.goSpeed, NULL},                                                //第1列参数地址
        {NULL,     &DataInfo.rampSpeed,           NULL,       &Circle.Circle_size2, &Circle.Circle_size4, &DataInfo.GOtime,  NULL}},
      //第七页
       {{"speed_P",       "speed_I",     "speed_D",         "Image_ONE",      "Tradition_turnPD",      "Tradition_speed",      "Save_Data", "Save_Data"},
        {&CarInfo.S_LP, &CarInfo.S_LI, &CarInfo.S_LD, &DataInfo.image_dirEP1, &DataInfo.image_dirTP2,&DataInfo.image_traditionSpeed, NULL},
        {&CarInfo.S_RP, &CarInfo.S_RI, &CarInfo.S_RD, &DataInfo.image_dirED1, &DataInfo.image_dirTD2, NULL, NULL}},
        //第八页
       {{"Cur_speed_P",     "Cur_speed_I ",      "MidCur_turnPD ",        "BigCur_turnPD ",        " ",       " ",      " ", " "},
        {&CarInfo.S_LP_Cur,&CarInfo.S_LI_Cur,  &DataInfo.image_bigP,  &DataInfo.image_superP, NULL, NULL, NULL},
        {&CarInfo.S_RP_Cur,&CarInfo.S_RI_Cur,  &DataInfo.image_bigD,  &DataInfo.image_superD, NULL, NULL, NULL}},
 };
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

std::atomic<bool> Para_Mode(false);                                    //调参模式
int PrecisionSelect = 2;                                   //调参精度等级
float Precision[5] = {100.0f, 10.0f, 1.0f, 0.1f, 0.01f};    //各个调参精度等级加减一次的数值

int Page = 0;                                              //当前页
int Choose_List = 0, Channel = 0;                          //参数选择列、行
int Choose_Mode = 0;                                       //图像显示选择模式

std::atomic<bool> Is_Increase(false);
std::atomic<bool> Is_Decrease(false);             //参数是否加减标志位

std::atomic<bool> Start;                                                //发车按键是否按下
uint8 CountStartTime;                                       //发车延时时间
uint8 StopTime;                                             //反转停车时间


//-------------------------------------------------------------------------------------------------------------------
//  @brief      按键扫描函数
//-------------------------------------------------------------------------------------------------------------------
void Key_Scan(void)
{
    if(!SET){
        //发车按键按下
        if(Page == 0 && !Start_Flag && Channel == 0){
            while(!SET);
            Start = true;
            ips200_clear();
//            ips114_show_string(80,  5, "GOGOGO!!!");
            BuzzerMs(100);
        }
        //发车后重回调参页
        else if(Page == 0 && Start_Flag && Channel == 0){
             Start_Flag = false;
             Start = false;
             while(!SET);
             ips200_clear();
             cleanup();
             BuzzerMs(10);
        }
        //存储Flash参数
        else if(Para_Mode == false && Channel == MaxSelect[Page][0]){
            while(!SET);
            Save_All_Data();
            Channel = 0;
            BuzzerMs(200);
        }
        //在非调参模式同时该页可调参，则进入调参模式
        else if(Para_Mode == false && MaxSelect[Page][1]){
            while(!SET);
            Para_Mode = true;
            ips200_clear();
        }
        else if(Para_Mode == true){
            while(!SET);
            Para_Mode = false;
            ips200_clear();
        }
    }

    //非调参模式，此时按键上下左右移动用于换行换页
    if(Para_Mode == false){
        //向右翻页
        if(!RIGHT){
            while(!RIGHT);

            Choose_List++;

            if(Choose_List >= MaxSelect[Page][1]){
                Page++;
                Choose_List = 0;
                ips200_clear();
            }

            if(Page >= PAGE_MAX){
                Page = 0;
                ips200_clear();
            }
        }
        //向左翻页
        else if(!LEFT){
            while(!LEFT);

            Choose_List--;

            if(Choose_List < 0){
                Page--;

                if(MaxSelect[Page][1])
                    Choose_List = MaxSelect[Page][1] - 1;
                else
                    Choose_List = 0;

                ips200_clear();
            }

            if(Page < 0){
                Page = PAGE_MAX - 1;
                ips200_clear();
            }
        }
        //向下换行
        else if(!DOWN){
            while(!DOWN);

            Channel++;

            if(Channel > MaxSelect[Page][0])
                Channel = 0;
        }
        //向上换行
        else if(!UP){
            while(!UP);

            Channel--;

            if(Channel < 0)
                Channel = MaxSelect[Page][0];
        }
    }
    //调参模式，此时参数上下左右用于参数加减与调参精度选择
    else if(Para_Mode == true){
        //调参精度选择
        if(!RIGHT){
            while(!RIGHT);

            PrecisionSelect++;

            if(PrecisionSelect >= 5)
                PrecisionSelect = 0;
        }
        else if(!LEFT){
            while(!LEFT);

            PrecisionSelect--;

            if(PrecisionSelect < 0)
                PrecisionSelect = 4;
        }
        //参数加减
        else if(!UP){
            while(!UP);

            Is_Increase = true;
        }
        else if(!DOWN){
            while(!DOWN);

            Is_Decrease = true;
         }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      参数加减函数
//-------------------------------------------------------------------------------------------------------------------
static void Change_Para(uint8 type)
{
    //参数加标志位
    if(Is_Increase == true)
    {
        //第0列参数
        if(Choose_List == 0)
        {
            switch(type)
            {
                case 1:     ParameterIncrease((*(int8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterIncrease((*(uint8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterIncrease((*(int16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterIncrease((*(uint16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterIncrease((*(int32*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterIncrease((*(float*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }
        //第1列参数
        else if(Choose_List == 1)
        {
            switch(type)
            {
                case 1:     ParameterIncrease((*(int8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterIncrease((*(uint8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterIncrease((*(int16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterIncrease((*(uint16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterIncrease((*(int32*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterIncrease((*(float*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }

        Is_Increase = false;
    }
    //参数减标志位
    else if(Is_Decrease == true)
    {
        //第0列参数
        if(Choose_List == 0)
        {
            switch(type)
            {
                case 1:     ParameterDecrease((*(int8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterDecrease((*(uint8*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterDecrease((*(int16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterDecrease((*(uint16*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterDecrease((*(int32*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterDecrease((*(float*)(Parameter[Page].ParaOne[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }
        //第1列参数
        else if(Choose_List == 1)
        {
            switch(type)
            {
                case 1:     ParameterDecrease((*(int8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);         break;
                case 2:     ParameterDecrease((*(uint8*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 3:     ParameterDecrease((*(int16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 4:     ParameterDecrease((*(uint16*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);       break;
                case 5:     ParameterDecrease((*(int32*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], int);        break;
                case 6:     ParameterDecrease((*(float*)(Parameter[Page].ParaTwo[Channel])), Precision[PrecisionSelect], float);      break;
                default:    break;
            }
        }

        Is_Decrease = false;
    }
}

/**
 * @brief 绘制单边边界或中线
 * @param points      边界点集二维数组
 * @param point_count 该边界的实际有效点数
 * @param color       画点颜色
 * @param x_offset    X轴加粗模式: 
 * -1 : 向左加粗 1 像素 (常用于左边界)
 * 1 : 向右加粗 1 像素 (常用于右边界)
 * 2 : 左右对称各加粗 1 像素 (常用于中线)
 * 0 : 不横向加粗
 */
void draw_single_boundary(int points[][2], int point_count, uint16 color, int x_offset) 
{
    for(int r = 0; r < point_count; r++) 
    {
        // 1. 坐标缩放与映射
        uint16 screen_x = (CROP_W - points[r][0]) * Screen_Width / CROP_W;
        uint16 screen_y = (CROP_H - points[r][1]) * Screen_Height / CROP_H;

        // 2. 绘制当前行(dy=0)与下一行(dy=1) (实现Y轴向下加粗 1 像素)
        for (int dy = 0; dy <= 1; dy++) 
        {
            // 绘制主点
            ips200_draw_point(screen_x, screen_y + dy, color);
            
            // 根据模式进行横向加粗
            if (x_offset == -1 || x_offset == 1) {
                // 左边界或右边界：单侧加粗
                ips200_draw_point(screen_x + x_offset, screen_y + dy, color);
            } 
            else if (x_offset == 2) {
                // 中线：左右对称加粗 (总宽度3个像素)
                ips200_draw_point(screen_x - 1, screen_y + dy, color);
                ips200_draw_point(screen_x + 1, screen_y + dy, color);
            }
        }
    }
}


void draw_row_boundary(uint16 boundary[], uint16 color, int x_offset) 
{
    // 遍历数组的每一个元素（索引 i 从 0 到 arr_len - 1）
    for(uint8 r = 0; r < ROW; r++) 
    {
        uint8 c = ROW - 1 - r;  // 将数组索引映射到图像行号
        
        // 1. 坐标缩放与映射
        uint16 screen_x = (CROP_W - boundary[c]) * Screen_Width / CROP_W;
        
        uint16 screen_y = (CROP_H - c) * Screen_Height / CROP_H;

        // 2. 绘制当前行(dy=0)与下一行(dy=1) (实现Y轴向下加粗 1 像素)
        for (int dy = 0; dy <= 1; dy++) 
        {
            // 绘制主点
            ips200_draw_point(screen_x, screen_y + dy, color);
            
            // 根据模式进行横向加粗
            if (x_offset == -1 || x_offset == 1) {
                // 左边界或右边界：单侧加粗
                ips200_draw_point(screen_x + x_offset, screen_y + dy, color);
            } 
            else if (x_offset == 2) {
                // 中线：左右对称加粗 (总宽度3个像素)
                ips200_draw_point(screen_x - 1, screen_y + dy, color);
                ips200_draw_point(screen_x + 1, screen_y + dy, color);
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示8位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_int8(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showint8B(134, r, (*(int8*)(Parameter[Page].ParaOne[r])));
            else
                ips200_showint8(134, r, (*(int8*)(Parameter[Page].ParaOne[r])));
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showint8B(192, r, (*(int8*)(Parameter[Page].ParaTwo[r])));
            else
                ips200_showint8(192, r, (*(int8*)(Parameter[Page].ParaTwo[r])));
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示8位无符号数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_uint8(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showuint8B(134, r, (*(uint8*)(Parameter[Page].ParaOne[r])));
            else
                ips200_showuint8(134, r, (*(uint8*)(Parameter[Page].ParaOne[r])));
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showuint8B(192, r, (*(uint8*)(Parameter[Page].ParaTwo[r])));
            else
                ips200_showuint8(192, r, (*(uint8*)(Parameter[Page].ParaTwo[r])));
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示16位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_int16(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showint16B(134, r, (*(int16*)(Parameter[Page].ParaOne[r])));
            else
                ips200_showint16(134, r, (*(int16*)(Parameter[Page].ParaOne[r])));
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showint16B(192, r, (*(int16*)(Parameter[Page].ParaTwo[r])));
            else
                ips200_showint16(192, r, (*(int16*)(Parameter[Page].ParaTwo[r])));
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示16位无符号数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_uint16(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showuint16B(134, r, (*(uint16*)(Parameter[Page].ParaOne[r])));
            else
                ips200_showuint16(134, r, (*(uint16*)(Parameter[Page].ParaOne[r])));
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showuint16B(192, r, (*(uint16*)(Parameter[Page].ParaTwo[r])));
            else
                ips200_showuint16(192, r, (*(uint16*)(Parameter[Page].ParaTwo[r])));
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示32位有符号数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_int32(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showint32B(134, r, (*(int32*)(Parameter[Page].ParaOne[r])), 4);
            else
                ips200_show_int(134, r, (*(int32*)(Parameter[Page].ParaOne[r])), 4);
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showint32B(192, r, (*(int32*)(Parameter[Page].ParaTwo[r])), 4);
            else
                ips200_show_int(192, r, (*(int32*)(Parameter[Page].ParaTwo[r])), 4);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示浮点数函数
//-------------------------------------------------------------------------------------------------------------------
static void Show_float(void)
{
    uint8 r = 0;

    for(r = 0; r < MaxSelect[Page][0]; r++)
    {
        //参数地址为空指针则不显示，显示第一列参数
        if(Parameter[Page].ParaOne[r] != NULL)
        {
            if(r == Channel && Choose_List == 0)
                ips200_showfloatB(134, r, (*(float*)(Parameter[Page].ParaOne[r])), 3, 3);
            else
                ips200_show_float(134, r, (*(float*)(Parameter[Page].ParaOne[r])), 3, 3);
        }

        if(Parameter[Page].ParaTwo[r] != NULL)
        {
            if(r == Channel && Choose_List == 1)
                ips200_showfloatB(192, r, (*(float*)(Parameter[Page].ParaTwo[r])), 3, 3);
            else
                ips200_show_float(192, r, (*(float*)(Parameter[Page].ParaTwo[r])), 3, 3);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      调参页显示函数
//  @param      dat[]       该页面的显示格式 ，同一页面只能以相同类型显示
//                          （可选参数：uint8、char、int8、uint16、int16、int32、float）
//  Sample usage:           Para_Page("int");
//-------------------------------------------------------------------------------------------------------------------
static void Para_Page(const char dat[])
{
    uint8 type = 0;         //参数类型
    uint8 r = 0;

    //判断参数类型
    if(!strcmp(dat, "int8") || !strcmp(dat, "char"))
        type = 1;
    else if(!strcmp(dat, "uint8"))
        type = 2;
    else if(!strcmp(dat, "int16"))
        type = 3;
    else if(!strcmp(dat, "uint16"))
        type = 4;
    else if(!strcmp(dat, "int32"))
        type = 5;
    else if(!strcmp(dat, "float"))
        type = 6;

    //参数加减
    Change_Para(type);

    //进入调参模式，显示调参精度等级
    if(Para_Mode == true)  ips200_showfloatB(90, 12, Precision[PrecisionSelect], 3, 3); //0

    //显示参数名
    for(r = 0; r < MaxSelect[Page][0] + 1; r++)
    {
        //变量名非0才显示
        if(strcmp("0", Parameter[Page].ParaName[r]))
        {
            if(r == Channel)
                ips200_show_stringB(0, r, Parameter[Page].ParaName[r]);
            else
                ips200_show_string(0, r, Parameter[Page].ParaName[r]);
        }
    }

    //显示参数
    switch(type)
    {
        case 1:     Show_int8();        break;
        case 2:     Show_uint8();       break;
        case 3:     Show_int16();       break;
        case 4:     Show_uint16();      break;
        case 5:     Show_int32();       break;
        case 6:     Show_float();       break;
        default:    break;
    }

    //显示页码
    ips200_showuint8(84, 9, Page + 1);
    ips200_show_string(116, 9, "/");
    ips200_showuint8(128, 9, PAGE_MAX);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第0页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------
static void Page0(void)
{
    uint8 r;
     //相隔线
     for(r = 0; r < 240; r++)
     {
         ips200_draw_point(r, 40, RGB565_RED);
     }
 
     ips200_show_string(0,  3, "MODE:");
 
     //参数加减
     Change_Para(2);
 
     //进入调参模式，显示调参精度等级
     if(Para_Mode == true)  ips200_showfloatB(150, 12, Precision[PrecisionSelect], 3, 3);  //3
 
     //显示参数名
     for(r = 0; r < MaxSelect[Page][0] + 1; r++)
     {
         //变量名非0才显示
         if(strcmp("0", Parameter[Page].ParaName[r]))
         {
             if(r == Channel)
                 ips200_show_stringB(48, (r + 3), Parameter[Page].ParaName[r]);
             else
                 ips200_show_string(48, (r + 3), Parameter[Page].ParaName[r]);
         }
     }
     //显示参数
     for(r = 0; r < MaxSelect[Page][0]; r++)
     {
         //参数地址为空指针则不显示，显示第一列参数
         if(Parameter[Page].ParaOne[r] != NULL)
         {
             if(r == Channel && Choose_List == 0)
                 ips200_showuint8B(208, r+3, (*(uint8*)(Parameter[Page].ParaOne[r])));
             else
                 ips200_showuint8(208, r+3, (*(uint8*)(Parameter[Page].ParaOne[r])));
         }
     }
 
     Choose_Mode = Channel;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第1页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------    
static void Page1(void)
{
    uint8_t r = 0, c = 0, i = 0 , a=0; 

    if(Choose_Mode == 1)
    {

    //    ips200_show_gray_image(0, 0, rgay_image, UVC_WIDTH, UVC_HEIGHT);
        ips200_displayimage032_zoomB((uint8 *)Bmp, CROP_W, CROP_H, 252, 75);
        //显示图像前瞻行
        for(c = 0; c < 240; c++)
        {
            ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1), RGB565_RED);//<< 1
        }

        //参数加减
        Change_Para(2);

        //进入调参模式，显示调参精度等级
        if(Para_Mode == true)  ips200_showfloatB(100, 12, Precision[PrecisionSelect], 3, 3); //5

        //显示参数名
        for(r = 0; r < MaxSelect[Page][0] + 1; r++)
        {
            //变量名非0才显示
            if(strcmp("0", Parameter[Page].ParaName[r]) && r <= 1)
            {
                if(r == Channel)
                    ips200_show_stringB(0, 7 + r, Parameter[Page].ParaName[r]);
                else
                    ips200_show_string(0, 7 + r, Parameter[Page].ParaName[r]);
            }
            else if(strcmp("0", Parameter[Page].ParaName[r]) && r == MaxSelect[Page][0])
            {
                if(r == Channel)
                    ips200_show_stringB(0, 9, Parameter[Page].ParaName[r]);
                else
                    ips200_show_string(0, 9, Parameter[Page].ParaName[r]);
            }
        }
        //显示参数
        for(r = 0; r < MaxSelect[Page][0]; r++)
        {
            //参数地址为空指针则不显示，显示第一列参数
            if(Parameter[Page].ParaOne[r] != NULL && r <= 1)
            {
                if(r == Channel && Choose_List == 0)
                    ips200_showuint8B(208, 7 + r, (*(uint8*)(Parameter[Page].ParaOne[r])));
                else
                    ips200_showuint8(208, 7 + r, (*(uint8*)(Parameter[Page].ParaOne[r])));
            }
            if(Parameter[Page].ParaOne[r] != NULL && r == MaxSelect[Page][0])
            {
               if(r == Channel && Choose_List == 0)
                   ips200_showuint8B(208, 9, (*(uint8*)(Parameter[Page].ParaOne[r])));
               else
                   ips200_showuint8(208, 9, (*(uint8*)(Parameter[Page].ParaOne[r])));
            }
        }
    }
    else//(非调参模式)
    {
        // ips200_show_gray_image(0, 0, binary_image, UVC_WIDTH, UVC_HEIGHT);                //这里放经过处理后的图像
        ips200_displayimage032_zoomB(Bmp[0], CROP_W, CROP_H, 252, 75);
        // //显示前瞻行
        for(c = 0; c < 252; c++)
        {
            ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1), RGB565_RED);//<< 1
        }

        // // 绘制左边界 (黄线，向左加粗)
        // draw_single_boundary(points_l, data_statics_l, RGB565_YELLOW, -1);
        // // 绘制中线 (红线，不横向加粗)
        // draw_single_boundary(Middle_line, data_statics_m, RGB565_RED, 2);
        // // 绘制右边界 (绿线，向右加粗)
        // draw_single_boundary(points_r, data_statics_r, RGB565_GREEN, 1);

        draw_row_boundary(L_boundary, RGB565_PURPLE, -1);
        draw_row_boundary(Row_Middle_line, RGB565_RED,2);
        draw_row_boundary(R_boundary, RGB565_PINK, 1);
        
        for(i = 0; i < 75; i ++)
            ips200_draw_point(126,i,RGB565_BLUE);
        //左边界
        // ips200_show_stringE(0,  8, "l:", RGB565_RED);
        // ips200_showuint16E(40, 8, data_statics_l, RGB565_RED);


        // // //右边界
        // ips200_show_stringE(160,  8, "m:", RGB565_RED);
        // ips200_showuint16E(208, 8, data_statics_m, RGB565_RED);    
 

        // //前瞻行白点数
        // ips200_show_stringE(80,  8, "r:", RGB565_RED);
        // ips200_showuint16E(128, 8, data_statics_r, RGB565_RED);


        // ips200_show_stringE(0,  9, "l_stop:", RGB565_RED);
        // ips200_showuint8E(48, 9, l_stop, RGB565_RED);
       
        // ips200_show_stringE(160,  9, "r_stop:", RGB565_RED);
        // ips200_showuint8E(208, 9, r_stop, RGB565_RED);
        
        // //右边界
        // ips200_show_stringE(160,  8, "m:", RGB565_RED);
        // ips200_showuint8E(208, 8, data_statics_m, RGB565_RED);

        // //左边界
        // ips200_show_stringE(0,  9, "r:", RGB565_RED);
        //  ips200_showuint8E(48, 9, data_statics_r, RGB565_RED);
        
    //     //左边界
    //     ips200_show_stringE(0,  8, "Left:", RGB565_RED);
    //     ips200_showuint8E(40, 8, Left, RGB565_RED);

    //     //前瞻行白点数
    //     ips200_show_stringE(80,  8, "White:", RGB565_RED);
    //    ips200_showuint8E(128, 8, WhiteSum, RGB565_RED);

    //     //右边界
    //     ips200_show_stringE(160,  8, "Right:", RGB565_RED);
    //     ips200_showuint8E(208, 8, Right, RGB565_RED);

    //     //左边界
    //     ips200_show_stringE(0,  9, "LongL:", RGB565_RED);
    //      ips200_showuint8E(48, 9, LongLeft, RGB565_RED);

       ips200_showint8E(123, 9, (int8)CarInfo.dirOffset,RGB565_BLUE);
//         //ips200_showint8E(75, 9, turn_PWM,RGB565_PURPLE);
//         // ips200_showint8E(75, 9, L_PWM,RGB565_PURPLE);
//         // ips200_showuint8E(83, 7, R_PWM, RGB565_PURPLE);

//         //ips200_showint8E(75, 9, WhiteRow[22],RGB565_BLUE);
// //        ips200_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
// //        ips200_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

//        ips200_show_stringE(160,  9, "LongR:", RGB565_RED);
//        ips200_showuint8E(208, 9, LongRight, RGB565_RED);

      //  ips200_showuint8E(0, 10, RoadFlag.crossDis, RGB565_PURPLE);
      //  ips200_showuint8E(50, 10, RoadFlag.cross, RGB565_PURPLE);

        //ips200_showuint8E(0, 11, Cross_WhiteCol[60], RGB565_RED);
        //ips200_showuint8E(50, 11, RoadFlag.cross, RGB565_RED);
// // 在屏幕右下角或指定位置显示 320x240 缩放后的全图 (缩放到 80x60)
// // 参数含义: 数据源, 源宽, 源高, 目标宽, 目标高

// //ips200_displayimage032_zoomB(image[0], 320, 240, 80, 60);

// // 给这个全图画个边框，方便对齐
// //ips200_draw_rectangle(0, 120, 80, 60, RGB565_GREEN);
    }
}

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      显示第1页（在显示函数中调用）
// //-------------------------------------------------------------------------------------------------------------------
// static void Page1(void)
// {
//     uint8_t left = 0, middle = 0, right = 0;
//     uint8_t r = 0, c = 0, i = 0; 

//     if(Choose_Mode == 1)
//     {

//     // //    ips200_show_gray_image(0, 0, rgay_image, UVC_WIDTH, UVC_HEIGHT);
//        ips200_displayimage032_zoomB(rgay[0], CROP_W, CROP_H, 252, 90);
//         //显示图像前瞻行
//         for(c = 0; c < 240; c++)
//         {
//             ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1.8), RGB565_RED);//<< 1
//         }

//         //参数加减
//         Change_Para(2);

//         //进入调参模式，显示调参精度等级
//         if(Para_Mode == true)  ips200_showfloatB(100, 12, Precision[PrecisionSelect], 3, 3); //5

//         //显示参数名
//         for(r = 0; r < MaxSelect[Page][0] + 1; r++)
//         {
//             //变量名非0才显示
//             if(strcmp("0", Parameter[Page].ParaName[r]) && r <= 1)
//             {
//                 if(r == Channel)
//                     ips200_show_stringB(0, 7 + r, Parameter[Page].ParaName[r]);
//                 else
//                     ips200_show_string(0, 7 + r, Parameter[Page].ParaName[r]);
//             }
//             else if(strcmp("0", Parameter[Page].ParaName[r]) && r == MaxSelect[Page][0])
//             {
//                 if(r == Channel)
//                     ips200_show_stringB(0, 9, Parameter[Page].ParaName[r]);
//                 else
//                     ips200_show_string(0, 9, Parameter[Page].ParaName[r]);
//             }
//         }
//         //显示参数
//         for(r = 0; r < MaxSelect[Page][0]; r++)
//         {
//             //参数地址为空指针则不显示，显示第一列参数
//             if(Parameter[Page].ParaOne[r] != NULL && r <= 1)
//             {
//                 if(r == Channel && Choose_List == 0)
//                     ips200_showuint8B(208, 7 + r, (*(uint8*)(Parameter[Page].ParaOne[r])));
//                 else
//                     ips200_showuint8(208, 7 + r, (*(uint8*)(Parameter[Page].ParaOne[r])));
//             }
//             if(Parameter[Page].ParaOne[r] != NULL && r == MaxSelect[Page][0])
//             {
//                if(r == Channel && Choose_List == 0)
//                    ips200_showuint8B(208, 9, (*(uint8*)(Parameter[Page].ParaOne[r])));
//                else
//                    ips200_showuint8(208, 9, (*(uint8*)(Parameter[Page].ParaOne[r])));
//             }
//         }
//     }
//     else
//     {
//         // ips200_show_gray_image(0, 0, binary_image, UVC_WIDTH, UVC_HEIGHT);                //这里放经过处理后的图像
//         // ips200_displayimage032_zoomB(Bmp[0], CROP_W, CROP_H, 252, 90);
//         //显示前瞻行
//         // for(c = 0; c < 252; c++)
//         // {
//         //     ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1.8), RGB565_RED);//<< 1
//         // }

//         // //显示左边界、中线、右边界
//         //迷宫法图像显示
//         ips200_displayimage032_zoomB(Bmp[0], CROP_W, CROP_H, 248, 88);
//         //显示前瞻行
//         for(c = 0; c < 248; c++)
//         {
//             ips200_draw_point(c, ((ROW - DataInfo.forwardLook) << 1), RGB565_RED);
//         }

//         //显示左边界、中线、右边界
//             for(r = 0 , i = 0; r < ROW; r++)
//         {
//                     c = ROW - 1 - r;

//                     left = (COL - LeftBlack[c]) << 1;
//                    middle = (COL - MiddleLine[c]) << 1;
//                     right = (COL - RightBlack[c]) << 1;

//             //第一行边界显示
//             ips200_draw_point(left, r+i, RGB565_RED);
//             ips200_draw_point(left - 1, r+i, RGB565_RED);

//             // ips200_draw_point(middle, r+i, RGB565_RED);

//             ips200_draw_point(right, r+i, RGB565_RED);
//             ips200_draw_point(right + 1, r+i, RGB565_RED);

//             i++;   

//             //第二行边界显示
//             ips200_draw_point(left, r+i, RGB565_PURPLE);
//             ips200_draw_point(left - 1, r+i, RGB565_PURPLE);

//             // ips200_draw_point(middle, r+i, RGB565_RED);

//             ips200_draw_point(right, r+i, RGB565_BLUE);
//             ips200_draw_point(right + 1, r+i, RGB565_BLUE);
//         }
//         for(i = 0; i < 84; i ++)
//             ips200_draw_point(123,i,RGB565_BLUE);

// //         //左边界
// //         ips200_show_stringE(0,  6, "Left:", RGB565_RED);
// //         ips200_showuint8E(40, 6, Left, RGB565_RED);

// //         //前瞻行白点数
// //         ips200_show_stringE(80,  6, "White:", RGB565_RED);
// //        ips200_showuint8E(128, 6, WhiteSum, RGB565_RED);

// //         //右边界
// //         ips200_show_stringE(160,  6, "Right:", RGB565_RED);
// //         ips200_showuint8E(208, 6, Right, RGB565_RED);

// //         //左边界
// //         ips200_show_stringE(0,  7, "LongL:", RGB565_RED);
// //          ips200_showuint8E(48, 7, LongLeft, RGB565_RED);

// // //        ips200_show_string(188, 1, "off");
// //         ips200_showint8E(123, 7, (int8)CarInfo.dirOffset,RGB565_BLUE);
// //         ips200_showint8E(75, 7, WhiteRow[22],RGB565_BLUE);
// // //        ips200_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
// // //        ips200_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

// //         ips200_show_stringE(160,  7, "LongR:", RGB565_RED);
// //         ips200_showuint8E(208, 7, LongRight, RGB565_RED);

// //         ips200_showuint8E(0, 8, RoadFlag.crossDis, RGB565_RED);
// //         ips200_showuint8E(50, 8, RoadFlag.cross, RGB565_RED);

// //         ips200_showuint8E(0, 9, Cross_WhiteCol[60], RGB565_RED);
// //         ips200_showuint8E(50, 9, RoadFlag.cross, RGB565_RED);


//         //左边界
//         ips200_show_stringE(0,  8, "Left:", RGB565_RED);
//         ips200_showuint8E(40, 8, Left, RGB565_RED);

//         //前瞻行白点数
//         ips200_show_stringE(80,  8, "White:", RGB565_RED);
//        ips200_showuint8E(128, 8, WhiteSum, RGB565_RED);

//         //右边界
//         ips200_show_stringE(160,  8, "Right:", RGB565_RED);
//         ips200_showuint8E(208, 8, Right, RGB565_RED);

//         //左边界
//         ips200_show_stringE(0,  9, "LongL:", RGB565_RED);
//          ips200_showuint8E(48, 9, LongLeft, RGB565_RED);

// //        ips200_show_string(188, 1, "off");
//         ips200_showint8E(123, 9, (int8)CarInfo.dirOffset,RGB565_BLUE);
//         ips200_showint8E(75, 9, WhiteRow[22],RGB565_BLUE);
// //        ips200_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
// //        ips200_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

//         ips200_show_stringE(160,  9, "LongR:", RGB565_RED);
//         // ips200_showuint8E(208, 9, LongRight, RGB565_RED);

//         // ips200_showuint8E(0, 10, RoadFlag.crossDis, RGB565_RED);
//         // ips200_showuint8E(50, 10, RoadFlag.cross, RGB565_RED);

//         // ips200_showuint8E(0, 11, Cross_WhiteCol[60], RGB565_RED);
//         // ips200_showuint8E(50, 11, RoadFlag.cross, RGB565_RED);
//     }
// }//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示第1页（在显示函数中调用）
//-------------------------------------------------------------------------------------------------------------------    

static void Page2()
{
    //保护标志位
    ips200_show_string(0,  0, "Protect:");
    ips200_showuint8(64, 0, RoadFlag.protect);

    //丢线标志位
    ips200_show_string(100,  0, "Blind:");
    ips200_showuint8(148, 0, RoadFlag.blind);

    ips200_show_string(0,  2, "Str:");
    ips200_showuint8(35, 2, RoadFlag.straight);

    //斑马线标志位
    ips200_show_string(100,  2, "zebra");
    ips200_showuint8(148, 2, RoadFlag.zebra);

    //斑马线标志位
    ips200_show_string(0,  1, "Dis:");
    ips200_show_float(35, 1, RoadFlag.zebradis, 2, 2);
//
//    ips200_show_string(120,  1, "zeAngle:");
//    ips200_show_float(180, 1, RoadFlag.zebraAngle, 3, 1);
    //十字标志位
    ips200_show_string(0,  3, "Cross:");
    ips200_showuint8(48, 3, RoadFlag.cross);

    ips200_show_string(100,  3, "Dis:");
    ips200_show_float(148, 3, RoadFlag.crossDis, 2, 2);

    //俯仰角偏航角
    ips200_show_string(0,  4, "Pitch:");
    ips200_show_float(48, 4, CarInfo.Pitch, 2, 2);

    ips200_show_string(100,  4, "Yaw:");
    ips200_show_float(148, 4, CarInfo.Yaw, 3, 2);

    //坡道标志位
    ips200_show_string(0,  5, "Ramp:");
    ips200_showuint8(40, 5, RoadFlag.ramp);  //
    ips200_show_string(100,  5, "Dis:");
    ips200_show_float(148, 5, RoadFlag.rampDis, 2, 2);

    //圆环标志位
    ips200_show_string(0,  6, "Circle");
    ips200_showuint8(56, 6, RoadFlag.circle);

    ips200_show_string(95,  6, "Angle");
    ips200_show_float(135, 6, RoadFlag.circleAngle, 3, 1);

    ips200_show_string(170,  6, "CDis");
    ips200_show_float(207, 6, RoadFlag.circleDis,3,2);  //RoadFlag.circleDis

    //避障标志位
    ips200_show_string(100,  1, "obs:");
    ips200_showuint8(148, 1, RoadFlag.AI);
    ips200_show_float(183, 1, RoadFlag.AIDis, 1, 1);
//    ips200_showuint8(172, 1, RoadFlag.obstDis);
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示函数（在主函数中调用）
//-------------------------------------------------------------------------------------------------------------------
//  @explain    若显示页面仅用于调参，可直接调用Para_Page("XXX")函数
//              若显示其他页面，则需自己编写函数，并在相应的case条件中调用
//-------------------------------------------------------------------------------------------------------------------
void IPS200_display(void)
{
    switch(Page)
    {
        case 0:     Page0();                break;
        case 1:     Page1();                break;
        case 2:     Page2();                break;
        case 3:     Para_Page("float");     break;
        case 4:     Para_Page("float");     break;
        case 5:     Para_Page("uint8");     break;
        case 6:     Para_Page("float");     break;
        case 7:     Para_Page("float");     break;
        default:    break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      按键测试函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    未知按键引脚号时可调用此函数判断按键引脚号
//-------------------------------------------------------------------------------------------------------------------
void Key_Text(void)
{
    if(!UP)
    {
        while(!UP);
        ips200_clear();
        ips200_show_string(120,0,"UP");
        BuzzerMs(100);
    }
    else if(!DOWN)
    {
        while(!DOWN);
        ips200_clear();
        ips200_show_string(120,1,"DOWN");
        BuzzerMs(100);
    }
    else if(!LEFT)
    {
        while(!LEFT);
        ips200_clear();
        ips200_show_string(120,2,"LEFT");
        BuzzerMs(100);
    }
    else if(!RIGHT)
    {
        while(!RIGHT);
        ips200_clear();
        ips200_show_string(120,3,"RIGHT");
        BuzzerMs(100);
    }
    else if(!SET)
    {
        while(!SET);
        ips200_clear();
        ips200_show_string(120,4,"SET");
        BuzzerMs(100);
    }
}
void key_test(void)
{
    if(!SET)
    {
        ips200_show_string(0, 0, "SET");
    }
    if(!RIGHT)
    {
        ips200_show_string(0, 1, "RIGHT");
    }
    if(!LEFT)
    {
        ips200_show_string(0, 2, "LEFT");
    }
    if(!UP)
    {
        ips200_show_string(0, 3, "UP");
    }
    if(!DOWN)
    {
        ips200_show_string(0, 4, "DOWN");
    }
}