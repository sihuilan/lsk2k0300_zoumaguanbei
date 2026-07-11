#include "zf_common_headfile.h"
#include <chrono>

RoadFlagType RoadFlag;

std::atomic<bool> StopCar(false);
uint8 ZebraTime = 0;
uint16 change,last_dl1a,d;
float inrampdis = 0;
float outramp = 0;
uint8 crosstime=0, ramptime=0, curvetime=0, LongStraightTime=0, intermtime=0, breaktime=0;
uint8 crossnum=0 ;
uint8 sum1=0;
uint8 SpaceDisNum,zebraNum;
uint8 count3=0;
float len = 0;
uint8 New_Obs=1;
uint8 zebranum=0;

extern cv::Mat frame_rgb;
int AI_id = -1;
float AI_conf = 0.0f;
int stable_cnt = 0;
int last_id = -1;

uint8 l_up[2]={0};
uint8 l_up_1[2]={0};                       
uint8 r_up[2]={0};  
uint8 r_up_1[2]={0};                      
uint8 l_down[2]={0}; 
uint8 l_down_1[2]={0};                      
uint8 r_down[2]={0}; 
uint8 r_down_1[2]={0};
uint8 lesser_Left[2] = {0};
uint8 lesser_Right[2] = {0};


//-------------------------------------------------------------------------------------------------------------------
//  @brief      图像识别并绕行
//-------------------------------------------------------------------------------------------------------------------
void AI_check(void)
{
    int id = -1;
    float conf = 0.0f;  
//--- AI 识别部分 ---
    if (RoadFlag.AI == 0)
    {  
        if (ncnn_process_frame(frame_rgb, id, conf))
        {
         if (id == last_id) stable_cnt++;
         else stable_cnt = 1;
         last_id = id;
         AI_id = id;
         AI_conf = conf;

            // 只有连续识别稳定时才触发
            if (stable_cnt >= 2 && conf > 0.7f) 
            {  
                // printf(">>> 锁定目标: %s (%.2f) <<<\n", class_labels[id].c_str(), conf);
                // if(id == 0 || id == 1)  
                //{
                    RoadFlag.AI = 1;
                   // RoadFlag.AIDis = 0;
               // }    
                //     RoadFlag.AIAngle= 0 ;
                //     printf("识别到武器，开始左转\n");
                // } 
                BuzzerMs(200);
            }
        }
        else
        {
            stable_cnt = 0; // 只有在常规行驶且真没识别到时，才清零计数器
            AI_id = -1;
            AI_conf = 0.0f;
        }
    }
    
}


//-------------------------------------------------------------------------------------------------------------------******
//  @brief      停车保护
//-------------------------------------------------------------------------------------------------------------------
void Protect(uint8 *p, uint8 line)              
{
    uint8 i, j;
    uint8 count;

    if(!RoadFlag.protect&&!RoadFlag.ramp && !RoadFlag.breaker && !RoadFlag.AI && RoadFlag.circle!= 5) // &&CarInfo.Pitch < 3.2
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
    if(!RoadFlag.AI && !RoadFlag.zebra && RoadFlag.circle!=9 && RoadFlag.circle!=10 && RoadFlag.circle!= 5 && RoadFlag.circle!=6 && !RoadFlag.ramp && !RoadFlag.obstacle && !RoadFlag.breaker)
    {
        //左边边界长度大于右边边界长度：右转弯或右切入十字
        if(LongLeft > LongRight && LongLeft <= 66 && LongLeft >= MIN_LINE && LongRight <= MAX_LINE) //LongLeft <= 28  24
        {
            //判断是否有折角，屏蔽十字干扰
            for(i = 0; i < LongLeft - 1; i++)    //****到达十字时，因为找不到线，直接跳到图像的最边缘（即“归边”）。这种坐标的突然跳变，就是代码里说的“折角”。
            {
                //如果为十字，左边界会突然归边（归边结果为COL-1），否则正常右转弯情况，下一行左边界会小于本行
                if(L_boundary[i] < L_boundary[i + 1])       //****左最大139，右最大0
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

                    for(i = L_boundary[j] - 1; i > R_boundary[j] + 1; i--)
                    {
                        if(*(p + i) == 0 && *(p + i - 1) == 0)
                            break;
                    }

                    if(i != R_boundary[j] + 1)
                        break;
                }

                //左边界小于70，再次确认其处于右转弯状态
                if(j == estimate && L_boundary[LongLeft - 1] < 100)   //****????要不要修改
                {
                    RoadFlag.blind = 1;
                    // printf("123\n");
                //    BuzzerMs(200);
                }
            }
        }
        //右边边界长度大于左边边界长度：左转弯或左切入十字
        else if(LongRight > LongLeft && LongRight <= 66 && LongRight >= MIN_LINE && LongLeft <= MAX_LINE)   //LongRight <= 28 24 22
        {
            //判断是否有折角，屏蔽十字干扰
            for(i = 0; i < LongRight - 1; i++){
                //如果为十字，右边界会突然归边（归边结果为0），否则正常左转弯情况，下一行右边界会大于本行
                if(R_boundary[i] > R_boundary[i + 1])
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

                    for(i = R_boundary[j] + 1; i < L_boundary[j] - 1; i++){
                        if(*(p + i) == 0 && *(p + i + 1) == 0)
                            break;
                    }
                    
                    if(i != L_boundary[j] - 1)
                        break;
                }

                //右边界大于50，再次确认其处于左转弯状态
                if(j == estimate && R_boundary[LongRight - 1] > 70)
                {
                    RoadFlag.blind = 2;
                    // printf("003\n");
                //    BuzzerMs(200);
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
                for (i = 94; i >= 26; i--)//L_boundary[j] R_boundary[j]
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
//            for (i = L_boundary[j]; i > R_boundary[j]; i--)
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
        if(LongLeft > TOPLINE && LongRight > TOPLINE  && fabs(CarInfo.dirOffset)<15)
        {
            LongStraightTime++;

            if(LongStraightTime>3)
            // {
            RoadFlag.straight = 1;
            // RoadFlag.curve=0;
            BuzzerMs(200);                
            // }


        }
        // else if(fabs(CarInfo.dirOffset)>=27)
        // {
        //     RoadFlag.curve=1;
        //     RoadFlag.straight = 0;
        // }
    }
    else
    {
          RoadFlag.straight = 0;
        //   RoadFlag.curve=0;
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

 void Cross_check(void){
  //角点补线法
    uint8 r;
    uint8 l_up_flag=0;
    uint8 r_up_flag=0;
    uint8 l_down_flag=0;
    uint8 r_down_flag=0;    
    uint8 L_cross=0;
    uint8 R_cross=0;
    std::memset(l_up, 0, sizeof(l_up));
    std::memset(l_up_1, 0, sizeof(l_up_1));
    std::memset(r_up, 0, sizeof(r_up));
    std::memset(r_up_1, 0, sizeof(r_up_1));
    std::memset(l_down, 0, sizeof(l_down));
    std::memset(r_down, 0, sizeof(r_down));

if( !RoadFlag.ramp && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.blind)
{
    if (l_down_flag == 0 )
    {
        for(r = 1; r < 37; r++)
        {
            if(LeftBlack1[r]-LeftBlack1[r+2] < -5 && LeftBlack1[r]-LeftBlack1[r+1] < 0 && LeftBlack1[r]-LeftBlack1[r-1] <= 0 && LeftBlack1[r]-LeftBlack1[r-1] >= -3) //不出现归边扫不到   && LeftBlack[r+2] > 110 && LeftBlack[r+2] <= 119
            {
                l_down[0]=LeftBlack1[r];
                l_down[1]=r;
                if(r > 2)
                {
                l_down_1[0]=LeftBlack1[r-3];                
                l_down_1[1]=r-3;
                }
                l_down_flag=1;
                L_cross++;
                // std::cout << "左下角点已抓取" << std::endl;
                break;
            }
        }    

    }

    if (r_down_flag == 0 )
    {
        for(r = 1; r < 37; r++)
        {
             if(RightBlack1[r]-RightBlack1[r+2] > 5 && RightBlack1[r]-RightBlack1[r+1] > 0 && RightBlack1[r]-RightBlack1[r-1] >= 0 && RightBlack1[r]-RightBlack1[r-1] <= 3)
            {
                r_down[0]=RightBlack1[r];
                r_down[1]=r;
                if(r > 2)
                {
                r_down_1[0]=RightBlack1[r-3];
                r_down_1[1]=r-3;
                }
                r_down_flag=1;
                R_cross++;
                // std::cout << "右下角点已抓取" << std::endl;
                break;
            }
        }    

    }

    if (l_up_flag == 0)
    {
        if(l_down_flag==1)
        {
            for(r = l_down[1]+1; r < ROW-1; r++)
            {
                if(LeftBlack1[r]-LeftBlack1[r-2] <= -8 && LeftBlack1[r]-LeftBlack1[r+1] >= 0 && LeftBlack1[r]-LeftBlack1[r+1] <= 3)
                {
                    l_up[0]=LeftBlack1[r];
                    l_up[1]=r;
                    l_up_flag=1;
                    L_cross++;
                    // std::cout << "左上角点已抓取" << std::endl;
                    break;
                }
            }   
        }
        else if(l_down_flag==0)
        {
            for(r = 3; r < ROW-1; r++)
            {
                if(LeftBlack1[r]-LeftBlack1[r-2] <= -8 && LeftBlack1[r]-LeftBlack1[r+1] >= 0 && LeftBlack1[r]-LeftBlack1[r+1] <= 3)
                {
                    l_up_1[0]=LeftBlack1[r+3];
                    l_up_1[1]=r+3;
                    l_up[0]=LeftBlack1[r];
                    l_up[1]=r;
                    l_up_flag=1;
                    L_cross++;
                    // std::cout << "左上角点已抓取" << std::endl;
                    break;
                }
            }   
        }
    }

    if (r_up_flag == 0)
    {
        if(r_down_flag==1)
        {
            for(r = r_down[1]+1; r < ROW-1; r++)
            {
                 if(RightBlack1[r]-RightBlack1[r-2] >= 8 && RightBlack1[r]-RightBlack1[r+1] <= 0 && RightBlack1[r]-RightBlack1[r+1] >= -3)
                {
                    r_up[0]=RightBlack1[r];
                    r_up[1]=r;
                    r_up_flag=1;
                    R_cross++;
                    // std::cout << "右上角点已抓取" << std::endl;
                    break;
                }
            }   
        }
        else if(r_down_flag==0)
        {
            for(r = 3; r < ROW-1; r++)
            {
                if(RightBlack1[r]-RightBlack1[r-2] >= 8 && RightBlack1[r]-RightBlack1[r+1] <= 0 && RightBlack1[r]-RightBlack1[r+1] >= -3)
                {    
                    r_up_1[0]=RightBlack1[r+3];
                    r_up_1[1]=r+3;
                    r_up[0]=RightBlack1[r];
                    r_up[1]=r;
                    r_up_flag=1;
                    R_cross++;
                    // std::cout << "右上角点已抓取" << std::endl;
                    break;
                }
            }   
        }
    }
}
    //  // 角点坐标打印
    if (L_cross||R_cross) {
    //     printf("jiaodian = %d\n", L_cross+R_cross);

    //     printf("l_down:(%d, %d)\n",  l_down[0], l_down[1]);
    //     printf("l_down: (%d, %d)\n",  r_down[0], r_down[1]);
    //     printf("l_up: (%d, %d)\n",  l_up[0], l_up[1]);
    //     printf("r_up: (%d, %d)\n",  r_up[0], r_up[1]);
    // }
    // else {
    //     printf("未检测到角点\n");
    }
    //直入十字判定
if( !RoadFlag.ramp && !RoadFlag.circle && !RoadFlag.zebra && !RoadFlag.blind)
{
    if(L_cross==2 &&R_cross==2)    //十字前
    {
        RoadFlag.cross=1; 
        // BuzzerMs(50);
    }
    else if(L_cross==1 && R_cross==1)//&& l_up[0] && r_up[0] && l_up[1] && r_up[1] && l_up_1[0] && r_up_1[0] && l_up_1[1] && r_up_1[1]
    {
        if(l_up[0]&&r_up[0]&&l_up[1]&&r_up[1]&&l_up_1[0]&&r_up_1[0]&&l_up_1[1]&&r_up_1[1]&& std::abs(Longest_White_Column_Left[1]-Longest_White_Column_Right[1])>=10)
        {
        // if(RoadFlag.Last_cross)
        // {
        //     RoadFlag.cross=2;
        // }
       if(std::abs(L_boundary[LongLeft]-R_boundary[LongRight])>=30 && std::abs(LongLeft-LongRight)<=10 && std::abs(Longest_White_Column_Left[1]-Longest_White_Column_Right[1])>=10)
        {
            RoadFlag.cross=2;
        }

        }
        else if(l_down[0]&&r_down[0]&&l_down[1]&&r_down[1]&&l_down_1[0]&&r_down_1[0]&&l_down_1[1]&&r_down_1[1])
        {
        RoadFlag.cross=3;
        }
        else if(r_up[0]&&r_up[1]&&r_up_1[0]&&r_up_1[1]&&l_down[0]&&l_down[1]&&l_down_1[0]&&l_down_1[1])
        {
            RoadFlag.cross=10;
        }
        else if(l_up[0]&&l_up[1]&&l_up_1[0]&&l_up_1[1]&&r_down[0]&&r_down[1]&&r_down_1[0]&&r_down_1[1])
        {
            RoadFlag.cross=11;
        }
        else if(l_up[0]&&l_up[1]&&l_down[0]&&l_down[1])
        {
            RoadFlag.cross=8;
        }
        else if(r_up[0]&&r_up[1]&&r_down[0]&&r_down[1])
        {
            RoadFlag.cross=9;
        }
        // BuzzerMs(100);
    } 
    else if(L_cross==2 && R_cross==1)
    {   
        if(r_up[0]&&r_up[1]&&r_up_1[0]&&r_up_1[1])
        {
        RoadFlag.cross=4;
        }
        else if(r_down[0]&&r_down[1]&&r_down_1[0]&&r_down_1[1])
        {
            RoadFlag.cross=6;
        }
    }
    else if(L_cross==1 && R_cross==2)
    {
        if(l_up[0]&&l_up[1]&&l_up_1[0]&&l_up_1[1])
        {
        RoadFlag.cross=5;
        }
        else if(l_down[0]&&l_down[1]&&l_down_1[0]&&l_down_1[1])
        {
            RoadFlag.cross=7;
        }
    }
    // else if((L_cross==1 &&R_cross==2)||(L_cross==2 &&R_cross==1))
    // {
    //     RoadFlag.cross=3;
    // }
    // //斜入十字判定
    // if(l_up[1] >= 0 && r_up[1] >= 0 && l_down[1] >= 0 && !RoadFlag.cross)       //左侧三点
    // {
    //     RoadFlag.cross = 3;
    //     std::cout << "左侧三点斜入十字" << std::endl;
    // }
    // else if( l_up[1] >= 0 && r_up[1] >= 0 && r_down[1] >= 0 && !RoadFlag.cross) //右侧三点
    // {
    //     RoadFlag.cross = 4;
    //     std::cout << "右侧三点斜入十字" << std::endl;
    // }
    // else if(l_up[1] >= 0 && r_up[1] >= 0&&LongLeft<=12&&LongRight<=12 && !RoadFlag.cross)    //两点
    // { 
    //     RoadFlag.cross=2;
    //     std::cout << "只有两点入十字" << std::endl;
    // }
    else
    RoadFlag.cross=0;
}
if(RoadFlag.ramp || RoadFlag.circle || RoadFlag.zebra || RoadFlag.blind || RoadFlag.lcur || (LongLeft>=45 && LongRight>=45))
{
    RoadFlag.cross=0;
}
// if(!RoadFlag.cross&&RoadFlag.Last_cross)
// {
//     RoadFlag.Last_cross=0;
// }
    // if(RoadFlag.cross && !crosstime && crossnum == 1) 
    // {  
    //     crossnum++;
    //     crosstime++;
    // }
    // else if(RoadFlag.cross && !crosstime)
    // {
    //     crosstime++;
    // }
    // else if(crosstime == 1 && !RoadFlag.cross)
    // {
    //     crossnum++;
    //     crosstime=0;
    // }
    // else if(crossnum ==3)
    // {
    //     crossnum=0;
    // }

 }

