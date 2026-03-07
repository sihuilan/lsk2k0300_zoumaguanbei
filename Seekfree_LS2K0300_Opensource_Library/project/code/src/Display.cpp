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
    {Page7Para_ROW, Page7Para_COL},
    {Page8Para_ROW, Page8Para_COL},
    {Page9Para_ROW, Page9Para_COL},
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
     {{"ImageEP_CurveP",       "ImageTP_LR",          "ADC_ET_P", "Ramp_P_line",        "Breline_SpeedZbr",    "obstleP_Speed",         "Save_Data", "Save_Data"},      //参数名
      {&DataInfo.image_dirEP, &DataInfo.image_dirTLP, NULL,   &DataInfo.rampP,     NULL,     &DataInfo.obstacleP,     NULL},           //第1列参数地址
      {&DataInfo.CurveP,      &DataInfo.image_dirTRP, NULL,   &DataInfo.rampline,  &DataInfo.zebraSpeed, &DataInfo.obstacleSpeed, NULL}},                         //第2列参数地址
     //第五页
     {{"R50_LR_P",           "Speed_R50_R100P",    "R60_LR_P",           "R80_ET_P",           "Speed_R60_R80",  "zebrago_LR",        "Save_Data", "Save_Data"},  //参数名
      {&Circle.CircleR50_LP, &Circle.Speed_R50,    &Circle.CircleR60_LP, &Circle.CircleR80_EP, &Circle.Speed_R60, NULL,  NULL},    //第1列参数地址
      {&Circle.CircleR50_RP, &Circle.CircleR100_P, &Circle.CircleR60_RP, &Circle.CircleR80_P,  &Circle.Speed_R80, NULL,  NULL}},    //第2列参数地址
      //第六页
      {{ "Image_str_cur",              "Imge_tra_ramp",               "AD_tra_cur",              "CircleSize_12",      "CircleSize_34",      "Speed_G0_Time",   "Save_Data", "Save_Data"},  //参数名
        {&DataInfo.image_straightSpeed,&DataInfo.image_traditionSpeed, NULL, &Circle.Circle_size1, &Circle.Circle_size3, &DataInfo.goSpeed, NULL},                                                //第1列参数地址
        {&DataInfo.image_curSpeed,     &DataInfo.rampSpeed,           NULL,       &Circle.Circle_size2, &Circle.Circle_size4, &DataInfo.GOtime,  NULL}},
      //第七页
       {{"R50_In_Out",         "R60_In_Out",         "R80_In_Out",         "R100_In_Out",         "Obsdis_LR_2",      "Obsdis_LR_3",      "Save_Data", "Save_Data"},
        {&Circle.Circle_R50In, &Circle.Circle_R60In, &Circle.Circle_R80In, &Circle.Circle_R100In, &DataInfo.obsdis2L, &DataInfo.obsdis3L, NULL},
        {&Circle.Circle_R50Out,&Circle.Circle_R60Out,&Circle.Circle_R80Out,&Circle.Circle_R100Out,&DataInfo.obsdis2R, &DataInfo.obsdis3R, NULL}},
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
       ips200_showuint8(0,10,Circle.CircleSize);
    //    ips200_showuint8(186,0,CircleTime);
    //    ips200_showuint8E(80, 0, WhiteRow[38], RGB565_YELLOW);
        ips200_show_string(90, 1, "Voltage:");
       ips200_show_float(170,1,CarInfo.Voltage,3,2); //显示电源电压
//#    ips200_showuint8(50,0,WhiteSum);
//    ips200_showuint8(50,0,Right);
//    ips200_showuint8(100,0,LongRight);
    ips200_showuint8(0,7,RightBlack[DataInfo.forwardLook]);
//#    ips200_showuint8(50,1,Cross_WhiteCol[60]);
//    ips200_showuint8(50,1,Left);
//    ips200_showuint8(100,1,LongLeft);
    ips200_showuint8(50,7,LeftBlack[DataInfo.forwardLook]);


//    ips200_showuint8(100,1,RoadFlag.crossDis);
    //    ips200_showint16(0,1,SL_output);  //显示左轮编码器数值
    //    ips200_showint16(50,1,SR_output);  //显示右轮编码器数值
//        ips200_show_float(120,0,CarInfo.Voltage,3,2); //显示电源电压
//   ips200_showint16(0,0,mpu6050_acc_x);  //显示左轮编码器数值
        //电池电压
    //    ips200_show_string(200, 0, "V:");

    //    ips200_showint16(0,0,CarInfo.L_encoder);  //显示左轮编码器数值
    //    ips200_showint16(0,1,CarInfo.R_encoder);  //显示右轮编码器数值
        //   ips200_showint8(0,8,LeftBlack[36]+RightBlack[36]-MIDDLE);
        //   ips200_showint8(50,8,LeftBlack[42]+RightBlack[42]-MIDDLE);
        ips200_showint8(0,8,LeftBlack[ROW-20]-RightBlack[ROW-20]);
        ips200_showint8(50,8,LeftBlack[ROW-21]-RightBlack[ROW-21]);
        ips200_showint8(0,9,LeftBlack[ROW-10]-RightBlack[ROW-10]);
        ips200_showint8(50,9,LeftBlack[ROW-11]-RightBlack[ROW-11]);



          ips200_showint8(50,10,LeftBlack[DataInfo.forwardLook]-RightBlack[DataInfo.forwardLook]);
        ips200_showint8(0,10,LeftBlack[40]-RightBlack[40]);
        //   ips200_showint8(50,8,WhiteRow[LongLeft+1] );
        //   ips200_showint8(0,9,WhiteRow[LongLeft+2]);
        //   ips200_showint8(50,9,WhiteRow[LongLeft+3]);
        //   ips200_showint8(0,10,WhiteRow[LongLeft+4] );

            // ips200_showint8(0,11,(LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1])  - (LeftBlack[LongLeft] - LeftBlack[LongLeft -1])*2 );
            // ips200_showint8(50,11,(LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1])  - (LeftBlack[LongLeft+1] - LeftBlack[LongLeft -1+1])*2 );
            // ips200_showint8(0,12,(LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1])  - (LeftBlack[LongLeft+2] - LeftBlack[LongLeft -1+2])*2 );
            // ips200_showint8(50,12,(LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1])  - (LeftBlack[LongLeft+3] - LeftBlack[LongLeft -1+3])*2 );
            // ips200_showint8(0,13,(LeftBlack[LongLeft - 1] - RightBlack[LongLeft - 1])  - (LeftBlack[LongLeft+4] - LeftBlack[LongLeft -1+4])*2 );
        //   ips200_show_float(100,0,RoadFlag.rampDis,3,2);
        //   ips200_showint8(0,8,Cross_WhiteCol[60]);
        //   ips200_showint8(0,9,LeftBlack[DataInfo.forwardLook]);
        //   ips200_showint8(50,9,RightBlack[DataInfo.forwardLook]);


        // ips200_showint8(0,7,WhiteCol[10]);
        // ips200_showint8(50,7,WhiteCol[15]);
        // ips200_showint8(0,8,WhiteCol[20]);
        // ips200_showint8(50,8,WhiteCol[25]);
        // ips200_showint8(0,9,WhiteCol[30]);
        // // ips200_showint8(50,9,WhiteCol[6]);

        // ips200_showint8(0,10,WhiteCol[35]);
        // ips200_showint8(50,10,WhiteCol[40]);
        // ips200_showint8(0,11,WhiteCol[45]);
        // ips200_showint8(50,11,WhiteCol[50]);



        // ips200_showint8(0,12,WhiteCol[38]);

        //   ips200_showint8(0,10,LeftBlack[26]);
        //   ips200_showint8(50,10,LeftBlack[22]);
        //   ips200_showint8(100,10,LeftBlack[18]);
        //   ips200_showint8(200,10,LeftBlack[14]);
        //   ips200_showint8(0,11,LeftBlack[10]);
        //   ips200_showint8(50,11,LeftBlack[6]);



        //   ips200_showint8(0,8,WhiteRow[16]);
        //   ips200_showint8(50,8,WhiteRow[10]);
//        ips200_showint16(150,0,WhiteRow[38]);  //39行白点数
//        ips200_show_float(0,0,CarInfo.Yaw,3,2);
//        ips200_show_float(64,0,CarInfo.Pitch,3,2);
//        ips200_show_float(120,0,P,3,2);
//        ips200_showuint8(170,0,Angular);
    //    ips200_showint8(100,0,LeftBlack[DataInfo.forwardLook]);
    //    ips200_showint8(140,0,RightBlack[DataInfo.forwardLook]);

//        ips200_show_float(120,0,ShowSlope,3,2);
//        ips200_showuint8(170, 0, ShowAngular);
//        ips200_showuint8(0, 1, showrRightBlack);

//        ips200_showint8(0, 1, WhiteRow[39]);
////        ips200_showint8(170, 1, WhiteCol[95]);
//        ips200_showuint8(120, 0, Slope);
//        ips200_showuint8(120, 0, SpaceDisNum);
//        ips200_showuint8(170, 0, zebraNum);
//        ips200_showuint8(60, 1, WhiteRow[DataInfo.forwardLook+3]);
//        ips200_showuint8(120, 1, WhiteRow[35]);
//        ips200_showuint8(170, 1, WhiteCol[60]);
//        ips200_showuint8(60, 1, count3);//endSummation
//        ips200_showuint8(120, 1, WhiteCol[20]);


//        ips200_showuint8(60, 1, LastSummation);//endSummation
//        ips200_showuint8(120, 1, endSummation)
//        ips200_showuint8(170, 1, endSummation1);

//          ips200_showint8(0, 1, summation);
//          ips200_showint16(50, 1, WhiteRow[DataInfo.forwardLook + 5]);
//          ips200_showint16(100, 1, WhiteRow[DataInfo.forwardLook ]);
//          ips200_showint16(150, 1, WhiteRow[20]);
//        ips200_showuint(100,0,time1,10);
//        ips200_showuint(100,1,time2,10);    


//        //摄像头偏差
//        ips200_show_string(188, 1, "off");
        // ips200_showint8(100, 0, (int8)CarInfo.dirOffset);
//        ips200_show_float(180, 1, CarInfo.expectCurvature,3,2);

    //阈值
//    ips200_show_string(125,  1, "T:"); //Threshold
//    ips200_showuint8(140, 1,ImageInfo.threshold); //Ramptime  ImageInfo.threshold

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
    uint8_t left = 0, middle = 0, right = 0;
    uint8_t r = 0, c = 0, i = 0; 

    if(Choose_Mode == 1)
    {

    // //    ips200_show_gray_image(0, 0, rgay_image, UVC_WIDTH, UVC_HEIGHT);
       ips200_displayimage032_zoomB(rgay[0], CROP_W, CROP_H, 252, 90);
        //显示图像前瞻行
        for(c = 0; c < 240; c++)
        {
            ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1.8), RGB565_RED);//<< 1
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
    else
    {
        // ips200_show_gray_image(0, 0, binary_image, UVC_WIDTH, UVC_HEIGHT);                //这里放经过处理后的图像
        ips200_displayimage032_zoomB(Bmp[0], CROP_W, CROP_H, 252, 90);
        //显示前瞻行
        for(c = 0; c < 252; c++)
        {
            ips200_draw_point(c, ((ROW - DataInfo.forwardLook) *1.8), RGB565_RED);//<< 1
        }

        //显示左边界、中线、右边界
            for(r = 0 , i = 0; r < 45; r++)//r = 0 , i = 0; r < ROW; r++
        {
                    c = ROW - 1 - r;

                    left = static_cast<uint8_t>(COL-1 - LeftBlack[c]) *1.8 ;//<< 1
                   middle = static_cast<uint8_t>(COL-1 - MiddleLine[c]) *1.8;//<< 1
                    right = static_cast<uint8_t>(COL-1 - RightBlack[c]) *1.8;//<< 1

            //第一行边界显示
            ips200_draw_point(left, r+i, RGB565_RED);
            ips200_draw_point(left - 1, r+i, RGB565_RED);

            ips200_draw_point(middle, r+i, RGB565_RED);

            ips200_draw_point(right, r+i, RGB565_RED);
            ips200_draw_point(right + 1, r+i, RGB565_RED);

            i++;   

            //第二行边界显示
            ips200_draw_point(left, r+i, RGB565_PURPLE);
            ips200_draw_point(left - 1, r+i, RGB565_PURPLE);

            ips200_draw_point(middle, r+i, RGB565_RED);

            ips200_draw_point(right, r+i, RGB565_BLUE);
            ips200_draw_point(right + 1, r+i, RGB565_BLUE);
        }
        for(i = 0; i < 90; i ++)
            ips200_draw_point(126,i,RGB565_BLUE);

        // //迷宫法图像显示
        // ips200_displayimage032_zoomB(Bmp[0], CROP_W, CROP_H, 248, 88);
        // //显示前瞻行
        // for(c = 0; c < 248; c++)
        // {
        //     ips200_draw_point(c, ((ROW - DataInfo.forwardLook) << 1), RGB565_RED);
        // }

        // //显示左边界、中线、右边界
        //     for(r = 0 , i = 0; r < ROW; r++)
        // {
        //             c = ROW - 1 - r;

        //             left = (COL - LeftBlack[c]) << 1;
        //            middle = (COL - MiddleLine[c]) << 1;
        //             right = (COL - RightBlack[c]) << 1;

        //     //第一行边界显示
        //     ips200_draw_point(left, r+i, RGB565_RED);
        //     ips200_draw_point(left - 1, r+i, RGB565_RED);

        //     ips200_draw_point(middle, r+i, RGB565_RED);

        //     ips200_draw_point(right, r+i, RGB565_RED);
        //     ips200_draw_point(right + 1, r+i, RGB565_RED);

        //     i++;   

        //     //第二行边界显示
        //     ips200_draw_point(left, r+i, RGB565_PURPLE);
        //     ips200_draw_point(left - 1, r+i, RGB565_PURPLE);

        //     ips200_draw_point(middle, r+i, RGB565_RED);

        //     ips200_draw_point(right, r+i, RGB565_BLUE);
        //     ips200_draw_point(right + 1, r+i, RGB565_BLUE);
        // }
        // for(i = 0; i < 84; i ++)
        //     ips200_draw_point(123,i,RGB565_BLUE);

//         //左边界
//         ips200_show_stringE(0,  6, "Left:", RGB565_RED);
//         ips200_showuint8E(40, 6, Left, RGB565_RED);

//         //前瞻行白点数
//         ips200_show_stringE(80,  6, "White:", RGB565_RED);
//        ips200_showuint8E(128, 6, WhiteSum, RGB565_RED);

//         //右边界
//         ips200_show_stringE(160,  6, "Right:", RGB565_RED);
//         ips200_showuint8E(208, 6, Right, RGB565_RED);

//         //左边界
//         ips200_show_stringE(0,  7, "LongL:", RGB565_RED);
//          ips200_showuint8E(48, 7, LongLeft, RGB565_RED);

// //        ips200_show_string(188, 1, "off");
//         ips200_showint8E(123, 7, (int8)CarInfo.dirOffset,RGB565_BLUE);
//         ips200_showint8E(75, 7, WhiteRow[22],RGB565_BLUE);
// //        ips200_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
// //        ips200_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

//         ips200_show_stringE(160,  7, "LongR:", RGB565_RED);
//         ips200_showuint8E(208, 7, LongRight, RGB565_RED);

//         ips200_showuint8E(0, 8, RoadFlag.crossDis, RGB565_RED);
//         ips200_showuint8E(50, 8, RoadFlag.cross, RGB565_RED);

//         ips200_showuint8E(0, 9, Cross_WhiteCol[60], RGB565_RED);
//         ips200_showuint8E(50, 9, RoadFlag.cross, RGB565_RED);


        //左边界
        ips200_show_stringE(0,  8, "Left:", RGB565_RED);
        ips200_showuint8E(40, 8, Left, RGB565_RED);

        //前瞻行白点数
        ips200_show_stringE(80,  8, "White:", RGB565_RED);
       ips200_showuint8E(128, 8, WhiteSum, RGB565_RED);

        //右边界
        ips200_show_stringE(160,  8, "Right:", RGB565_RED);
        ips200_showuint8E(208, 8, Right, RGB565_RED);

        //左边界
        ips200_show_stringE(0,  9, "LongL:", RGB565_RED);
         ips200_showuint8E(48, 9, LongLeft, RGB565_RED);

//        ips200_show_string(188, 1, "off");
        ips200_showint8E(123, 9, (int8)CarInfo.dirOffset,RGB565_BLUE);
        ips200_showint8E(75, 9, WhiteRow[22],RGB565_BLUE);
//        ips200_showuint8E(83, 7, RightBlack[23], RGB565_PURPLE);
//        ips200_showuint8E(123, 7, RightBlack[24], RGB565_BLUE);

        ips200_show_stringE(160,  9, "LongR:", RGB565_RED);
        ips200_showuint8E(208, 9, LongRight, RGB565_RED);

        ips200_showuint8E(0, 10, RoadFlag.crossDis, RGB565_RED);
        ips200_showuint8E(50, 10, RoadFlag.cross, RGB565_RED);

        ips200_showuint8E(0, 11, Cross_WhiteCol[60], RGB565_RED);
        ips200_showuint8E(50, 11, RoadFlag.cross, RGB565_RED);



    }
}

static void Page2()
{
    //保护标志位
    ips200_show_string(0,  0, "Protect:");
    ips200_showuint8(64, 0, RoadFlag.protect);

    //丢线标志位
    ips200_show_string(100,  0, "Blind:");
    ips200_showuint8(148, 0, RoadFlag.blind);

//#    ips200_showuint8(200, 0, ZebraTime);


    ips200_show_string(0,  2, "Str:");
    ips200_showuint8(35, 2, RoadFlag.straight);

    //斑马线标志位
    ips200_show_string(100,  2, "zebra");
    ips200_showuint8(148, 2, RoadFlag.zebra);

//    //斑马线标志位
//
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
    ips200_showuint8(148, 1, RoadFlag.obstacle);
    ips200_show_float(183, 1, RoadFlag.obstDis, 1, 1);
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