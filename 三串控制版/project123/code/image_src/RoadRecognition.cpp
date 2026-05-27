#include "zf_common_headfile.h"


RoadFlagType RoadFlag;

std::atomic<bool> StopCar(false);
uint8 ZebraTime = 0;
uint16 change,last_dl1a,d;
float inrampdis = 0;
float outramp = 0;
uint8 crosstime=0, ramptime=0, curvetime=0, LongStraightTime=0, intermtime=0, breaktime=0;
float len = 0;


//-------------------------------------------------------------------------------------------------------------------******
//  @brief      停车保护
//-------------------------------------------------------------------------------------------------------------------
void Protect(uint8 *p, uint8 line)              
{
    uint8 i, j;
    uint8 count;

    if(!RoadFlag.protect&&!RoadFlag.ramp && !RoadFlag.breaker) // &&CarInfo.Pitch < 3.2
    {
        //停车保护
        for (i = 0; i < line; i++)
        {
            count = 0;

            for (j = 0; j < COL; j++)
            {
                if (p[i * COL + j] == 0)
                    count++;
            }

            if (count < COL - 10)        //如果在赛道上经常莫名停车，请检查这个地方
                return;
        }

        RoadFlag.protect = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      丢线保护
//-------------------------------------------------------------------------------------------------------------------*****
//  @explain    弯道和十字附近可能会出现丢线丢到家去的情况的处理方法
//-------------------------------------------------------------------------------------------------------------------
void LoseLineProtect(void)
{
    uint8 i, j;
    uint8 *p = 0;
    uint8 estimate = 0;

    RoadFlag.blind = 0;     //丢线标志位清零

    //-----丢线处理(弯道和十字附近会出现的情况)
    //入圆环时屏蔽丢线保护，防止干扰
    if(!RoadFlag.zebra && RoadFlag.circle!=9 && RoadFlag.circle!=10 && RoadFlag.circle!= 5 && RoadFlag.circle!=6 && !RoadFlag.ramp && !RoadFlag.obstacle && !RoadFlag.breaker)
    {
        //左边边界长度大于右边边界长度：右转弯或右切入十字
        if(LongLeft > LongRight && LongLeft <= 42 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE) //LongLeft <= 28  24
        {
            //判断是否有折角，屏蔽十字干扰
            for(i = 0; i < LongLeft - 1; i++)    //****到达十字时，因为找不到线，直接跳到图像的最边缘（即“归边”）。这种坐标的突然跳变，就是代码里说的“折角”。
            {
                //如果为十字，左边界会突然归边（归边结果为COL-1），否则正常右转弯情况，下一行左边界会小于本行
                if(LeftBlack[i] < LeftBlack[i + 1])       //****左最大139，右最大0
                    break;
            }

            //判断成功，无折角
            if(i == LongLeft - 1)
            {
                if(LongLeft > DataInfo.forwardLook - 4)
                    estimate = LongLeft - 4;
                else
                    estimate = LongLeft - 2;

                for(j = MIN_LINE; j < estimate; j++)
                {
                    p = &Bmp[j][0];

                    for(i = LeftBlack[j] - 1; i > RightBlack[j] + 1; i--)
                    {
                        if(*(p + i) == 0 && *(p + i - 1) == 0)
                            break;
                    }

                    if(i != RightBlack[j] + 1)
                        break;
                }

                //左边界小于70，再次确认其处于右转弯状态
                if(j == estimate && LeftBlack[LongLeft - 1] < 100)   //****????要不要修改
                {
                    RoadFlag.blind = 1;
                   BuzzerMs(200);
                }
            }
        }
        //右边边界长度大于左边边界长度：左转弯或左切入十字
        else if(LongRight > LongLeft && LongRight <= 42 && LongRight >= MIN_LINE && LongLeft <= MAX_LINE)   //LongRight <= 28 24 22
        {
            //判断是否有折角，屏蔽十字干扰
            for(i = 0; i < LongRight - 1; i++){
                //如果为十字，右边界会突然归边（归边结果为0），否则正常左转弯情况，下一行右边界会大于本行
                if(RightBlack[i] > RightBlack[i + 1])
                    break;
            }

            //判断成功，无折角
            if(i == LongRight - 1){
                if(LongRight > DataInfo.forwardLook - 4)
                    estimate = LongRight - 4;
                else
                    estimate = LongRight - 2;

                for(j = MIN_LINE; j < estimate; j++){
                    p = &Bmp[j][0];

                    for(i = RightBlack[j] + 1; i < LeftBlack[j] - 1; i++){
                        if(*(p + i) == 0 && *(p + i + 1) == 0)
                            break;
                    }
                    
                    if(i != LeftBlack[j] - 1)
                        break;
                }

                //右边界大于50，再次确认其处于左转弯状态
                if(j == estimate && RightBlack[LongRight - 1] > 70)
                {
                    RoadFlag.blind = 2;
                   BuzzerMs(200);
                }

            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      斑马线识别
//-------------------------------------------------------------------------------------------------------------------
void CheckZebra(void)
{
    int i, j;
    uint8 zebraLook=0;
    //BlackLine=0;
    uint8 zebraNum = 0, zebraLine = 0;
    //uint8 Black_ROW[15];
    uint8 *p = 0;
    zebraLook = RoadFlag.zebraLine;

    static uint8 zebrastop;
    //坡道摄像头抬头易误识别到斑马线，故遇坡道则不识别斑马线
    if(intermtime == 1 && RoadFlag.intermDis >= 5.0 && !RoadFlag.protect && !RoadFlag.cross && crosstime == 0 && !RoadFlag.obstacle && !RoadFlag.zebra && !RoadFlag.ramp && !RoadFlag.breaker && CarInfo.Pitch < 6 && fabs(CarInfo.dirOffset) < 30) //&& LongLeft >= DataInfo.forwardLook-5 && LongRight >= DataInfo.forwardLook-5  5  && !RoadFlag.circle
    {
        if((LongLeft >= 9 && LongRight >= 37) || (LongRight >= 9 && LongLeft >= 37))  //&& LongLeft <= 25
        {
            //如果坡道后面有斑马线应该考虑坡道的定距与斑马线识别问题
            for(j = DataInfo.forwardLook + zebraLook; j >= 9; j--)
            {
//                zebraNum = 0;
                p = &Bmp[j][0];     //使用比前瞻稍微远一些的一行作为判断起跑线的依据
                for (i = 94; i >= 26; i--)//LeftBlack[j] RightBlack[j]
                {
                    if ((*(p + i)== 0 && *(p + i + 1) == 1) || (*(p + i) == 1 && *(p + i + 1) == 0))
                    {
                        zebraNum++;
                    }
                }

                //连续几行斑马线点均超过目标值，则确认为斑马线
                if(zebraNum >= 4 && DataInfo.door==0)   //黑白跳变点大于5
                {
                    zebraLine++;

                    if(zebraLine >= 2)  //有上述黑白跳变点的行数  2
                    {
                        RoadFlag.zebra = 1;                     //斑马线标志位置1
                        RoadFlag.cross = 0;                     //清十字标志位
                        RoadFlag.circle= 0;                    //清圆环标志位
                        ZebraTime++;
                        BuzzerMs(200);
                    }
                }
                else if(zebraNum >= 3 && DataInfo.door==1)   //黑白跳变点大于5
                {
                    zebraLine++;

                    if(zebraLine >= 2)  //有上述黑白跳变点的行数  2
                    {
                        RoadFlag.zebra = 1;                     //斑马线标志位置1
                        RoadFlag.cross = 0;                     //清十字标志位
                        RoadFlag.circle= 0;                     //清圆环标志位
                        intermtime = 0;
                        ZebraTime++;
                        BuzzerMs(200);
                    }
                }
            }

        }

//        //库可能在右边，从左边循起
//        if(LongLeft > LongRight)
//        {
//            for (i = LeftBlack[j]; i > RightBlack[j]; i--)
//            {
//                if ((*(p + i)== 0 && *(p + i + 1) == 1) || (*(p + i) == 1 && *(p + i + 1) == 0))
//                {
//                    zebraNum++;
//                }
//            }
//        }
//        //连续几行斑马线点均超过目标值，则确认为斑马线
//        if(zebraNum >= 3)
//        {
//            zebraLine++;
//
//            if(zebraLine >= 2)
//            {
//                RoadFlag.zebra = 1;                     //斑马线标志位置1
//                RoadFlag.cross = 0;                     //清十字标志位
//                intermtime = 0;                      //清三岔路口标志位
//                RoadFlag.circle= 0;                    //清圆环标志位
//                ZebraTime++;
//                BuzzerMs(200);
//            }
//        }
    }

    else if(RoadFlag.zebra)
    {
        if(ZebraTime == 1 && DataInfo.GOtime == 2 && RoadFlag.zebradis > 0.50)    //跑两圈时加上
        {
            RoadFlag.zebra = 0;
            RoadFlag.zebradis = 0;
            RoadFlag.zebraAngle = 0;
        }

        else if(ZebraTime == 2)  //RoadFlag.zebraOne=0.1
        {
            if( RoadFlag.zebra == 1 && DataInfo.door == 0 && RoadFlag.zebradis >= DataInfo.zebragoL)// && RoadFlag.zebradis > 0.15
            {
                if(WhiteSum >=95)
                    RoadFlag.zebra = 0;
                else
                    RoadFlag.zebra = 2;
//                RoadFlag.zebraAngle = 0;
            }
            else if(RoadFlag.zebra == 1 &&DataInfo.door == 1 && RoadFlag.zebradis >= DataInfo.zebragoR)//0.2 && RoadFlag.zebradis > 0.15
            {
                RoadFlag.zebra = 2;
//                RoadFlag.zebradis = 0;
            }
            else if(RoadFlag.zebra == 2 )
            {
                 if(DataInfo.door == 0 && fabs(RoadFlag.zebraAngle)>=70)//60 0.4   && RoadFlag.zebradis > 0.28
                 {
                     RoadFlag.zebra=3;
                     RoadFlag.zebradis = 0;
                     BuzzerMs(200);
                 }
                 else if(DataInfo.door == 1 && fabs(RoadFlag.zebraAngle)>=70)//55 0.6   && RoadFlag.zebradis > 0.35
                 {
                     RoadFlag.zebra=3;
                     RoadFlag.zebradis = 0;
                     BuzzerMs(200);
                 }
             }
             else if(RoadFlag.zebra==3 && RoadFlag.zebradis >=0.05)
             {
//                StopCar = TRUE;
                 if(CarInfo.L_encoder <= 0 && CarInfo.R_encoder <= 0)
                     zebrastop ++;
                 if(zebrastop == 20)
                 {
                     RoadFlag.zebra=0;
                     RoadFlag.protect=1;
                 }
             }
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      长直道识别
//-------------------------------------------------------------------------------------------------------------------
void CheckStraight(void)
{
    if(!RoadFlag.cross && !RoadFlag.ramp && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle && !RoadFlag.breaker)
    {
        if(LongLeft > TOPLINE && LongRight > TOPLINE && !RoadFlag.straight && fabs(CarInfo.dirOffset)<27)
        {
            LongStraightTime++;

            if(LongStraightTime>3)
            RoadFlag.straight = 1;
            RoadFlag.curve=0;

        }
        else if(fabs(CarInfo.dirOffset)>=27)
        {
            RoadFlag.curve=1;
            RoadFlag.straight = 0;
        }
    }
    else
    {
          RoadFlag.straight = 0;
          RoadFlag.curve=0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      十字识别
//-------------------------------------------------------------------------------------------------------------------  ***************这一部分我认为他的左右边界与上面的丢线和保护俩个模块的边界反了过来
void Cross_check(void)
{
    uint8  r;
    if(!RoadFlag.cross && !RoadFlag.ramp && !RoadFlag.circle && !RoadFlag.zebra &&!RoadFlag.obstacle && !RoadFlag.breaker)
    {
        //单侧归边 + 前核查校验
        if(Right == COL - 1 && Left <= CROSS_LEFT && Left > 0 && WhiteSum < WHITE_MAX && WhiteSum > WHITE_MIN)  //LongLeft < 27 && LongRight < 27用来防止圆环除误判为十字
        {
            for(r = 1; r <= 5; r++)
            {
                if(!Bmp[DataInfo.forwardLook + r][Left])
                    return;
            }
            // BuzzerMs(200);
            RoadFlag.cross = 1;

            Left = 0;
            Right = COL - 1;
        }

        else if(Left == 0 && Right >= CROSS_RIGHT && Right < COL - 1 && WhiteSum <= WHITE_MAX && WhiteSum > WHITE_MIN)
        {
            for(r = 1 ; r <= 5 ; r++)
            {
                if(!Bmp[DataInfo.forwardLook + r][Right])
                    return;
            }
            // BuzzerMs(100);
            RoadFlag.cross = 1;
            Left = 0;
            Right = COL - 1;
        }
        //双侧全归边（“大白画面”）
        else if(Left == 0 && Right == COL - 1 && WhiteSum >= WHITE_MAX)
        {
            // BuzzerMs(100);
            RoadFlag.cross = 1;
            Left = 0;
            Right = COL - 1;
        }
        //短线判定
        else if(WhiteSum >= WHITE_MAX && (LongLeft <= DataInfo.forwardLook && LongRight <= DataInfo.forwardLook))
        {
            // BuzzerMs(200);
            RoadFlag.cross = 1;
            Left = 0;
            Right = COL - 1;
        }
    }
    else if(RoadFlag.cross ==1 )
    {
        if(RoadFlag.crossDis > 0.3 && (Right > Left && Left > 1 && Right < COL - 2 && WhiteSum < 70)) //0.4
        {
            RoadFlag.cross = 0;
            crosstime += 1;
            RoadFlag.crossDis = 0;
            BuzzerMs(200);
        }
        else if(RoadFlag.crossDis > 0.3 &&( LongLeft > DataInfo.forwardLook && LongRight > DataInfo.forwardLook))
        {
            RoadFlag.cross = 0;
            crosstime += 1;
            RoadFlag.crossDis = 0;
            BuzzerMs(200);

            Left = COL - 1 - LeftBlack[DataInfo.forwardLook];                           //************左右边界再矫正回来
            LastLeft = COL - 1 - LeftBlack[DataInfo.forwardLook];
            Right = COL - 1 - RightBlack[DataInfo.forwardLook];
            LastRight = COL - 1 - RightBlack[DataInfo.forwardLook];
        }
        if(crosstime == 2)
        {
                intermtime = 1;     //十字后紧连着某种元素         *****************暂时可以不做考虑
                crosstime = 0;          
        }
        if(RoadFlag.circle || RoadFlag.breaker || RoadFlag.obstacle)
        {
            intermtime = 0;
            BuzzerMs(100);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      坡道识别
//-------------------------------------------------------------------------------------------------------------------
void Ramp_check(void)
{
    uint8 i = 0;

    if(!RoadFlag.cross  && !RoadFlag.ramp && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.obstacle )
    {
        if(LongLeft == ROW || LongRight == ROW)
        {
            for(i = DataInfo.forwardLook - 5; i <= ROW - 1; i++)
            {
                if(WhiteRow[i] < (WhiteRow[i + 1] - 2))     //宽度在变大
                    break;
                if(i == ROW - 1)
                    break;
            }
            if(i == ROW - 1 && Left != 0 && Right != COL-1)
            {
                if(WhiteRow[i - 1] > RAMP_WIDTH && WhiteRow[i - 2] > RAMP_WIDTH && WhiteRow[i - 3] > RAMP_WIDTH && WhiteRow[i - 4] > RAMP_WIDTH)
                {
                    RoadFlag.ramp = 1;
                    BuzzerMs(200);
                }
            }
        }
        //强行坡道
        else if(!RoadFlag.ramp && fabs(CarInfo.Pitch) >= 11.0)//10.0 11.3
         {
             RoadFlag.ramp = 1;
             RoadFlag.cross = 0;
             RoadFlag.circle = 0;
             crosstime = 0;
             BuzzerMs(200);
         }
    }

    //坡顶
    else if(RoadFlag.ramp == 1 && RoadFlag.rampDis > 1.1)
    {
        RoadFlag.ramp = 2;
        crosstime = 0;
        BuzzerMs(200);
    }
    //坡道结束
    else if(RoadFlag.ramp == 2 && fabs(CarInfo.Pitch) < 5.5 && RoadFlag.rampDis >= DataInfo.rampline)//5  1.9 DataInfo.rampline
    {
        RoadFlag.ramp = 0;
        ramptime ++;
//        outramp = 1;
        crosstime = 0;
        BuzzerMs(200);
    }


    if(ramptime)
    {
        if(RoadFlag.outrampdis >= 0.40)
            ramptime = 0;
        else if(RoadFlag.breaker || RoadFlag.circle ||RoadFlag.cross || RoadFlag.obstacle || RoadFlag.straight || RoadFlag.zebra)
            ramptime = 0;
    }


}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      避障识别
//-------------------------------------------------------------------------------------------------------------------
void Obstacle_check(void)
{
    //uint8 i,j;  //road_Angular;
    //uint8 obstacle_vertical = 0;
    //uint8 Black_COL[COL] = {0};//确定行黑点数

    if(!RoadFlag.cross && !RoadFlag.circle && !RoadFlag.ramp && !crosstime && !RoadFlag.obstacle && !RoadFlag.zebra && !RoadFlag.breaker && !ramptime)//fabs(CarInfo.Pitch) < 5.0
    {

        if(Left >= 28 &&  Right <= 93 && LongLeft >= 23 && LongLeft <=37 && LongRight >= 23 && LongRight <=37 && fabs(LongLeft - LongRight) <= 3 && fabs(CarInfo.dirOffset)<=16 && WhiteSum >= 48 && WhiteSum <= 58)//fabs(LongLeft - LongRight) <= 4 && fabs(CarInfo.dirOffset)<7
        {
//            for(i = 28; i <= ROW-6 ; i++)           //从上往下，但不能用上面几行，因为可能是拐弯处上面黑色部分，要稍微往下点开始
//            {
//                if(Left > 40 && Right < 80)     break;  //排除断路
//
//                for(j = (COL / 2) - 16; j < (COL / 2) + 16; j++)    //从中间列往两侧的一定范围内数黑点 18
//                {
//                    if(Bmp[i][j] == 0)
//                    {
//                        Black_COL[i]++;             //储存行黑点数
//                        if(Black_COL[i] >= 28)      obstacle_vertical++;    //行黑点数大于32的行数33
//                    }
//                    else            break;
//                }
//            }
//            if(obstacle_vertical > 3 )// && road_Angular < 3
//            {
                if(DataInfo.OBST == 0)          //(DataInfo.door == 1)
                {
                    RoadFlag.obstacle = 1;      //左避障
                    RoadFlag.ramp = 0;
                    BuzzerMs(100);
                }
                else if(DataInfo.OBST == 1)    //(DataInfo.door == 0)
                {
                    RoadFlag.obstacle = 2;      //右避障
                    RoadFlag.ramp = 0;
                    BuzzerMs(100);
                }
//            }
        }
//        else if(dl1a_distance_mm <= 1030 && LongLeft > 34 && LongRight > 34 && Left != 0 && Right != COL-1 && fabs(LongLeft - LongRight) <= 4) //LongLeft < 35 && LongRight< 35 && Left != 0 && Right != COL-1
//        {
//            RoadFlag.obstacle = 1;
//            BuzzerMs(100);
//        }

//        last_dl1a = dl1a_distance_mm;
//        else if( fabs(LongLeft - LongRight) <=5 && WhiteSum < 70 && dl1a_distance_mm < 800)
//        {
//            RoadFlag.obstacle = 1;
//            BuzzerMs(100);
//        }
    }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      断路识别
//-------------------------------------------------------------------------------------------------------------------
//  @explain
//-------------------------------------------------------------------------------------------------------------------
void Breaker_check(void)
{
    uint8 i,j,m,n;
    uint8 BlackLine = 0, WhiteLine =0;
    uint8 Blackcount[60]={0}, Whitecount[60]={0}; //White_COL[COL]={0};

    //电磁停车保护


    if(CarInfo.Pitch < 5.0 && fabs(change) < 900 && !RoadFlag.obstacle && !RoadFlag.ramp && !RoadFlag.circle && ZebraTime<2 && !RoadFlag.breaker && !RoadFlag.protect && !RoadFlag.blind)
    {
        //从平直赛道进入
        if(LongLeft >= 8 && LongLeft <=23 && LongRight >= 8 && LongRight <= 23 && WhiteSum < 50)//60
        {
            for(i = 21; i <= 40; i++)
            {
                 for(j=(COL/2)-25; j<(COL/2)+25; j++)    //列 20
                 {
                     if(Bmp[i][j] == 0)
                     {
                         Blackcount[i]++;             //储存行黑点数
                         if(Blackcount[i] >= 38)      BlackLine++;    //黑点行数
                     }
                 }
             }
//            for(i = 28; i <= 35; i++)
//            {
//                for(j=40; j<80; j++)
//                {
//                    if(Bmp[i][j] == 1)
//                    {
//                        Whitecount[i]++;
//                        if(Whitecount[i] >= 5)      WhiteLine++;    //白点行数
//                    }
//                }
//
//            }
            if(BlackLine >= 17 )//&& WhiteLine < 2
            {
                RoadFlag.protect = 0;
                RoadFlag.breaker = 1;
                breaktime ++;
                WhiteLine = 0;
                BuzzerMs(200);
            }
        }
        //从弯道中进入
        else if(LongLeft <= 23 && LongRight <= 23 && fabs(LongLeft-LongRight)<2 && WhiteSum == 0 && (Left==0 || Right == 119))
        {
            for(i = 0, j = 0; i < LongRight+4; i++)
            {
              if(RightBlack[i] < RightBlack[i + 1])   j++;
            }
            for(m = 0, n = 0; m < LongLeft+4; m++)
            {
              if(LeftBlack[m] > LeftBlack[m + 1])     n++;
            }
            if(j>=4 || n>=4)
            {
                RoadFlag.protect = 0;
                RoadFlag.breaker = 2;
                // DataInfo.MODE = 1;
                breaktime ++;
                WhiteLine = 0;
                BuzzerMs(200);
            }
        }
    }

    else if(RoadFlag.breaker)
    {
//        if(ADInfo.ADC[0]<=3 && ADInfo.ADC[1]<=3 && ADInfo.ADC[2]<=3)     RoadFlag.protect = 1;

        if(LongLeft >=30 && LongRight >= 30 && Left !=0 && Right != 119 && WhiteSum >= 50) // && RoadFlag.breakDis >1.55 23
        {
            //
            for(m=0,n=0;m<=30;m++)
            {
                if(LeftBlack[m] > LeftBlack[m + 1] || RightBlack[m] < RightBlack[m + 1])
                    n++;
            }
            for(i=0; i<=DataInfo.forwardLook; i++)
            {
                for(j=(COL/2)-25; j<(COL/2)+25; j++)
                {
                    if(Bmp[i][j] == 1)
                    {
                        Whitecount[i]++;
                        if(Whitecount[i] >= 45)      WhiteLine++;    //白点行数
                    }
                }
            }
            if(n >= 2 && WhiteLine >=21)
            {
                RoadFlag.breakDis = 0;
//                 RoadFlag.breaker = 3;
                //  DataInfo.MODE = 0;
                 BuzzerMs(200);
            }
        }
        else if(LongLeft >= 28 && Left >= 35 && WhiteSum >= 65)// && RoadFlag.breakDis >= ADDir.breakline
        {
            RoadFlag.breaker = 0;
            // DataInfo.MODE = 0;
//            RoadFlag.breakDis = 0;
//            RoadFlag.breaker = 3;
            BuzzerMs(200);
        }
        else if(LongRight >= 28 && Right >= 70 && WhiteSum >= 65)
        {
            RoadFlag.breaker = 0;
            // DataInfo.MODE = 0;
//            RoadFlag.breakDis = 0;
//            RoadFlag.breaker = 3;
            BuzzerMs(200);
        }
//        else if(RoadFlag.breaker == 3)// && RoadFlag.breakDis >= 0.3
//        {
//            RoadFlag.breaker = 0;
//            DataInfo.MODE = 0;
//        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      弯道识别
//-------------------------------------------------------------------------------------------------------------------
//  @explain    弯道离库或者坡道很近 给另外的转向P
//-------------------------------------------------------------------------------------------------------------------
// void Curve_check(void)
// {
//     if(!RoadFlag.Curve && !RoadFlag.circle && !RoadFlag.obstDis && !RoadFlag.protect && !RoadFlag.blind && gotime==1 )
//     {
//         if(LongLeft <= 25 && LongRight >= 28 && CarInfo.dirOffset >= 30 && CarInfo.distance < 2.0 && CarInfo.distance >0.7)   //左弯
//         {
//             RoadFlag.Curve = 1;
// //            curvetime ++;
//             BuzzerMs(150);
//         }
//         else if(LongRight <= 25 && LongLeft >= 28 && CarInfo.dirOffset <= -30 && CarInfo.distance < 2.0 && CarInfo.distance >0.7)  //右弯
//         {
//             RoadFlag.Curve = 2;
// //            curvetime ++;
//             BuzzerMs(150);
//         }
//     }
//     else if(RoadFlag.Curve )
//     {
//         if(LongLeft >= 30 && LongRight >= 30 && WhiteSum<65) //Left <= CROSS_LEFT && Right >= CROSS_RIGHT && fabs(LongLeft-LongRight)<=10
//         {
//             BuzzerMs(100);
//             RoadFlag.Curve = 0;
//             gotime = 0;
//         }
//         else if(LongRight >= 30 && LongLeft >= 30 && WhiteSum<65) //Left <= CROSS_LEFT && Right >= CROSS_RIGHT && fabs(LongLeft-LongRight)<=10
//         {
//             BuzzerMs(100);
//             RoadFlag.Curve = 0;
//             gotime = 0;
//         }
//         else if(CarInfo.distance >= 3.5)
//         {
//             BuzzerMs(100);
//             RoadFlag.Curve = 0;
//             gotime = 0;
//         }
//     }
// }
