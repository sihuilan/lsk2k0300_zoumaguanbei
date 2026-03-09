/*********************************************************************************************************************
* LS2K0300 Opensourec Library 即（LS2K0300 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是LS2K0300 开源库的一部分
*
* LS2K0300 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          main
* 公司名称          成都逐飞科技有限公司
* 适用平台          LS2K0300
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者           备注
* 2025-02-27        大W            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"

// *************************** 例程硬件连接说明 ***************************
//  久久派与主板使用54pin排线连接，再将久久派插到主板上面，确保插到底核心板与主板插座间没有缝隙即可
//  久久派与主板使用54pin排线连接，再将久久派插到主板上面，确保插到底核心板与主板插座间没有缝隙即可
//  久久派与主板使用54pin排线连接，再将久久派插到主板上面，确保插到底核心板与主板插座间没有缝隙即可
//  使用本历程，就需要使用我们逐飞科技提供的内核。
// 
//  目前仅支持两寸SPI屏幕
//  SPI 两寸屏 硬件引脚
//  SCL         查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO60
//  SDA         查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO62
//  RST         查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO74
//  DC          查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO26
//  CS          查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO62
//  BL          查看 seekfree_smart_cat_pai_99 文件中 st7789v 节点定义 默认 GPIO63
//  GND         核心板电源地 GND
//  3V3         核心板 3V3 电源
//
// 使用USB插入UVC摄像头
// 
// *************************** 例程测试说明 ***************************
// 1.核心板烧录本例程 插在主板上 2寸IPS 显示模块插在主板的屏幕接口排座上 请注意引脚对应 不要插错
// 
// 2.电池供电 上电后 2寸IPS 屏幕亮起 显示数字等信息
// 
// 3.判断屏幕为SPI屏幕或者并口屏幕: 查看屏幕背面的PCB丝印，如果带有SPI字样就为SPI屏幕，否则为并口屏幕
//   例程文件夹下有不同类型2寸屏幕的区别示意图 "SPI和并口2寸屏幕区别.png"
// 
// 4.运行例程，既可看到灰度图像显示到屏幕上
//
// 如果发现现象与说明严重不符 请参照本文件最下方 例程常见问题说明 进行排查
//
// **************************** 代码区域 ****************************


#define COL     CROP_W
#define ROW     CROP_H
#define MID     (COL >> 1)

#define border_min      1
#define border_max      CROP_W-2
#define USE_num	        1000      //400个提取点，看情况修改
// #define MAX_TRACK_STEPS 1000     //最大跟踪步数

#define ENDROW 20

// 另外一端的IP地址
#define SERVER_IP "192.168.33.220"
// 端口号
#define PORT 8086

//0：不包含边界信息  
//1：包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点
//2：包含三条边线信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点，一般来说很少有这样的使用需求
//3：包含三条边线信息，边界信息含有横纵轴坐标，意味着你可以指定每个点的横纵坐标，边线的数量也可以大于或者小于图像的高度，通常来说边线数量大于图像的高度，一般是搜线算法能找出回弯的情况
//4：没有图像信息，仅包含三条边线信息，边线信息只包含横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点，这样的方式可以极大的降低传输的数据量
#define INCLUDE_BOUNDARY_TYPE   1

// 边界的点数量远大于图像高度，便于保存回弯的情况
#define BOUNDARY_NUM            (CROP_H * 4 / 2)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

uint8 Bmp[CROP_H][CROP_W];
uint8 rgay[CROP_H][CROP_W];
uint8 image[UVC_HEIGHT][UVC_WIDTH];
uint8 transfer[CROP_H][CROP_W];

cv::Mat frame_binary;

uint8 LeftBlack[ROW], RightBlack[ROW], MiddleLine[ROW]; //左右边界、中线
uint8 LeftBlack1[ROW], RightBlack1[ROW]; //左右边界、中线
uint8  Left_compound, Right_compound;                   //左右边界中线合成参数
uint8 LongLeft, LongRight;                              //左右有边界行数

uint8 WhiteRow[ROW], WhiteCol[COL];                     //存储图像横、纵方向白点数

uint8 forwardLook[COL];                                 //前瞻行像素点反转
uint8 WhiteSum;                                         //前瞻行白点数
uint8 LastRight, LastLeft;                              //历史左右边界
uint8 Right = COL - 1, Left = 0;                        //当前左右边界
uint8 Width[ROW];
uint8 Cross_WhiteCol[COL]={0};                          //十字列白点数
uint8 obstacle_L_WhiteRow[ROW]={0};                     //左出弯时行白点数
uint8 obstacle_R_WhiteRow[ROW]={0};                     //右出弯时行白点数
uint8 Longest_White_Column_Left[2]={0};                 //左侧最长白列【0】是白列长度，【1】是下标，第j列
uint8 Longest_White_Column_Right[2]={0};                //右侧最长白列【0】是白列长度，【1】是下标，第j列
uint8 Search_Stop_Line=0;                               //搜索截止行


const uint8 Standard_Road_Wide[CROP_H]=                 //标准赛宽这个数组每次调整摄像头和图像都要重新测量  //用与坡道识别
{ 108,106,104,101,99, 97, 95, 93, 91, 89,              
   88, 86, 84, 82, 79, 77, 76, 74, 73, 68,
   67, 66, 65, 61, 58, 57, 54, 53, 50, 47,
   45, 42, 41, 38, 36, 34, 32, 30, 27, 25,
   23, 20, 18, 16, 14, 13, 11,  8, 0, 0};



/************************************************************二值化部分************************************************************/
/*********************************************************************************************************************************/


//迭代阈值法
uint8 Iterative_Threshold(const cv::Mat& image, bool minMaxDel) {
    // 计算直方图
    cv::Mat hist;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange[] = {range};    
    double predict_Threshold = 0;
    double threshold = 0;
    static double s_threshold = 0;
    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, histRange);

    int validCount = image.total();

    // 删除最大最小灰度值
    if (minMaxDel) {
        // 清除低灰度区域
        for (int i = 0; i < 5; ++i) {
            validCount -= static_cast<int>(hist.at<float>(i));
            hist.at<float>(i) = 0;
        }
        // 清除中间区域的所有孤岛阈值
        for (int i = 5; i <= 250; ++i) {
            if (hist.at<float>(i + 4) == 0 &&
                hist.at<float>(i + 3) == 0 &&
                hist.at<float>(i + 2) == 0 &&
                hist.at<float>(i + 1) == 0 &&
                hist.at<float>(i) != 0 &&
                hist.at<float>(i - 1) == 0 &&
                hist.at<float>(i - 2) == 0 &&
                hist.at<float>(i - 3) == 0 &&
                hist.at<float>(i - 4) == 0) {
                validCount -= static_cast<int>(hist.at<float>(i));
                hist.at<float>(i) = 0;
            }
        }
        // 清除高灰度区域
        for (int i = 251; i < 256; ++i) {
            validCount -= static_cast<int>(hist.at<float>(i));
            hist.at<float>(i) = 0;
        }
    }

    // 寻找最大最小值
    int maxGray = 0, minGray = 255;
    for (int i = 0; i < 255; ++i) {
        if (hist.at<float>(i) != 0) {
            minGray = i;
            break;
        }
    }
    for (int i = 255; i > 0; --i) {
        if (hist.at<float>(i) != 0) {
            maxGray = i;
            break;
        }
    }

    // 预测阈值
    predict_Threshold = (threshold == 0 || s_threshold >= maxGray || s_threshold <= minGray) ?
        (static_cast<uint8>((maxGray + minGray) / 2)) : s_threshold;        //如果满足a条件里的任意一个，则输出表达式1否则输出2

    // 求总灰度值
    double sum = 0;
    for (int i = minGray; i <= maxGray; ++i) {
        sum += hist.at<float>(i) * i;
    }

    // 迭代求阈值
    for (int t = 0; t < 50; ++t) {
        int w0 = 0;
        double sum0 = 0;
        // 背景部分
        for (int i = minGray; i < predict_Threshold; ++i) {
            w0 += static_cast<int>(hist.at<float>(i));
            sum0 += hist.at<float>(i) * i;
        }

        // 前景部分
        int w1 = validCount - w0;
        double sum1 = sum - sum0;

        double u0 = w0 ? (sum0 / w0) : 0;
        double u1 = w1 ? (sum1 / w1) : 0;

        predict_Threshold = static_cast<uint8>((u1 + u0) / 2);

        if (std::fabs(predict_Threshold - threshold) <= 1) {
            break;
        } else if (std::fabs(predict_Threshold - s_threshold) >= 6) {
            threshold = s_threshold;
        } else if (predict_Threshold < 45 || s_threshold < 45) {
            threshold = 45;
        } else {
            threshold = predict_Threshold;
        }
    }

    s_threshold = predict_Threshold;
    return threshold;
}

void crop_image(uint8 src[UVC_HEIGHT][UVC_WIDTH],
                uint8 dest[CROP_H][CROP_W],
                int startRow, int startCol, int cropH, int cropW) 
{
// 预先进行边界检查（调用前确保合法性更好）
for (int i = 0; i < cropH; i++) 
{
// 指向原图当前行的起始位置
uint8 *srcPtr = &src[startRow + i][startCol];
// 指向目标图当前行的起始位置
uint8 *destPtr = &dest[i][0];
// 使用 memcpy 复制一整行（如果内存是连续的）
memcpy(destPtr, srcPtr, cropW * sizeof(uint8));
}
}




//最长白列法
void Prospect_Border1(void)             //后面的边界提取，能够取到比较具体的每行的边界在哪一列
{
    uint8 i=0,j=0;
    //uint8_t leftLine = 0, rightLine = 0;
    //uint8_t leftRecord = 0, rightRecord = 0;
   // uint8 Lose_Left=0,Lose_Right=0;
    // 左右边界、中线归边
    std::memset(MiddleLine, 0, sizeof(MiddleLine));
    std::memset(LeftBlack1, COL - 1, sizeof(LeftBlack));
    std::memset(RightBlack1, 0, sizeof(RightBlack));
    std::memset(Longest_White_Column_Left,0,sizeof(Longest_White_Column_Left));        
    std::memset(Longest_White_Column_Right,0,sizeof(Longest_White_Column_Right));    
   //从左到右找左边最长白列
    Longest_White_Column_Left[0] =0;
    for(i=COL-41;i>=40;i--)
    {
        if (Longest_White_Column_Left[0] < WhiteCol[i])//找最长的那一列
        {
            Longest_White_Column_Left[0] = WhiteCol[i];//【0】是白列长度
            Longest_White_Column_Left[1] = i;              //【1】是下标，第j列
        }
    }
    //从右到左找右左边最长白列
    Longest_White_Column_Right[0] = 0;//【0】是白列长度
    for(i=40;i<=COL-41;i++)//从右往左，注意条件，找到左边最长白列位置就可以停了
    {
        if (Longest_White_Column_Right[0] < WhiteCol[i])//找最长的那一列
        {
            Longest_White_Column_Right[0] = WhiteCol[i];//【0】是白列长度
            Longest_White_Column_Right[1] = i;              //【1】是下标，第j列
        }
    } 
    if(Longest_White_Column_Left[0]>Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止行
    }
    else if(Longest_White_Column_Left[0]<Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
    }
    else if(Longest_White_Column_Left[0]==Longest_White_Column_Right[0])
    {
            Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
    }
    // printf("Longest_White_Column_Left[1]- Longest_White_Column_Right[1]= %d\n",Longest_White_Column_Left[1]- Longest_White_Column_Right[1]);
    
        for (i = 0; i <Search_Stop_Line; i++)//常规巡线
    {
        //从最下面一行，访问到我的有效是视野行
        for (j = Longest_White_Column_Right[1]; j >= 2; j--)
        {
            if (Bmp[i][j] ==255 && Bmp[i][j - 1] == 0 && Bmp[i][j - 2] == 0)//白黑黑，找到右边界
            {
                RightBlack1[i]  = j;
                // if(!Lose_Right)
                // {
                //     LongRight=i; 
                // }
                break;
            }
            else if(j<=2)//没找到右边界，把屏幕最右赋值给右边界
            {
                RightBlack1[i]  = j-2;
                // if(!Lose_Right)
                // {
                //     LongRight=i;
                //     Lose_Right=1;
                // }
                break;
            }

        }
        for (j =Longest_White_Column_Left[1] ; j <=COL-3; j++)//往左边扫描
        {//从最下面一行，访问到我的有效是视野行
            if (Bmp[i][j] ==255 && Bmp[i][j + 1] == 0 && Bmp[i][j + 2] == 0)//黑黑白认为到达左边界
            {
                LeftBlack1[i] = j;
                // if(!Lose_Left)
                // {
                //     LongLeft=i; 
                // }
                break;
            }
            else if(j>=COL-3)
            {
                LeftBlack1[i] = j+2;//找到头都没找到边，就把屏幕最左右当做边界
                // if(!Lose_Left)
                // {
                //     LongLeft=i; 
                //     Lose_Left=1;
                // }
                break;
            }
        }
    }


    
    
}

// //-------------------------------------------------------------------------------------------------------------------
// //  @brief      边缘提取函数
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
// //-------------------------------------------------------------------------------------------------------------------

void Prospect_Border(void) {

    std::atomic<bool> leftFlag(false);
    std::atomic<bool> rightFlag(false);
    uint8_t pLeft, pRight;
    uint8_t leftLine = 0, rightLine = 0;
    uint8_t leftRecord = 0, rightRecord = 0;

    LongLeft = 0;
    LongRight = 0;

    // 左右边界、中线归边
    std::memset(MiddleLine, 0, sizeof(MiddleLine));
    std::memset(LeftBlack, COL - 1, sizeof(LeftBlack));
    std::memset(RightBlack, 0, sizeof(RightBlack));
    // std::memset(Longest_White_Column_Left,0,sizeof(Longest_White_Column_Left));        
    // std::memset(Longest_White_Column_Right,0,sizeof(Longest_White_Column_Right));  

    // 设置赛道宽度
    std::memset(Width, COL - 1, sizeof(Width));


//    //从左到右找左边最长白列
//     Longest_White_Column_Left[0] =0;
//     for(int i=COL-21;i>=20;i--)
//     {
//         if (Longest_White_Column_Left[0] < WhiteCol[i])//找最长的那一列
//         {
//             Longest_White_Column_Left[0] = WhiteCol[i];//【0】是白列长度
//             Longest_White_Column_Left[1] = i;              //【1】是下标，第j列
//         }
//     }
//     //从右到左找右左边最长白列
//     Longest_White_Column_Right[0] = 0;//【0】是白列长度
//     for(int i=20;i<=COL-21;i++)//从右往左，注意条件，找到左边最长白列位置就可以停了
//     {
//         if (Longest_White_Column_Right[0] < WhiteCol[i])//找最长的那一列
//         {
//             Longest_White_Column_Right[0] = WhiteCol[i];//【0】是白列长度
//             Longest_White_Column_Right[1] = i;              //【1】是下标，第j列
//         }
//     } 
//     if(Longest_White_Column_Left[0]>Longest_White_Column_Right[0])
//     {
//             Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止行
//     }
//     else if(Longest_White_Column_Left[0]<Longest_White_Column_Right[0])
//     {
//             Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
//     }
//     else if(Longest_White_Column_Left[0]==Longest_White_Column_Right[0])
//     {
//             Search_Stop_Line = Longest_White_Column_Right[0];//搜索截止行
//     }
    Prospect_Border1();
    // 前瞻行之前寻边界
    for (int r = 0; r < ENDROW; r++) {
        // 轮寻左边界
        if (r > 0 && LeftBlack[r - 1] > 0 && LeftBlack[r - 1] < COL - 1) {
       //     RightBlack[r - 1]
            pLeft = LeftBlack[r - 1];
            if (pLeft > 109) pLeft = 109;
            else if (pLeft < 30) pLeft = 30;

            for (int c = pLeft - 30; c < pLeft + 30; c++) {
                if (Bmp[r][c] == 255 && Bmp[r][c+1]  == 0) {
                    LeftBlack[r] = c + 1;
                    leftFlag = true;
                    break;
                } else if (c == pLeft + 29) {         //当第二行的左边界相距上一行的左边界大于29个点时，就给他归边
                    LeftBlack[r] = COL - 1;
                    leftFlag = false;
                    break;
                }
            }
        }

        // if (RoadFlag.ramp) {
        //     if ((r == 0 || !leftFlag) && r < ENDROW) {
        //         for (int c = 80; c < COL - 2; c++) {
        //             if (Bmp[r][c]  == 255 && Bmp[r][c+1]  == 0) {
        //                 LeftBlack[r] = c + 1;
        //                 break;
        //             } else if (c == 1) {
        //                 LeftBlack[r] = COL - 1;
        //                 break;
        //             }
        //         }
        //     }
        // } else {
        //     if ((r == 0 || !leftFlag) && r < ENDROW) {
        //         for (int c = COL - 2; c > 0; c--) {
        //             if (Bmp[r][c]  == 0 && Bmp[r][c-1]  == 255) {
        //                 LeftBlack[r] = c;
        //                 break;
        //             } else if (c == 1) {
        //                 LeftBlack[r] = COL - 1;
        //                 break;
        //             }
        //         }
        //     }
        // }
            if ((r == 0 || !leftFlag) && r < ENDROW) {              //不能正常寻到左边界时的处理，从最左边一直往最右边边界一直寻
                for (int c = COL - 2; c > 0; c--) {
                    if (Bmp[r][c]  == 0 && Bmp[r][c-1]  == 255) {
                        LeftBlack[r] = c;
                        break;
                    } else if (c == 1) {                //如果还是找不到就直接给他归边了
                        LeftBlack[r] = COL - 1;
                        break;
                    }
                }
            }
        


        // 轮寻右边界
        if (r > 0 && RightBlack[r - 1] > 0 && RightBlack[r - 1] < COL - 1) {
            pRight = RightBlack[r - 1];
            if (pRight > 109) pRight = 109;
            else if (pRight < 30) pRight = 30;

            for (int c = pRight + 30; c > pRight - 30; c--) {
                if (Bmp[r][c]  == 255 && Bmp[r][c-1]  == 0) {
                    RightBlack[r] = c - 1;
                    rightFlag = true;
                    break;
                } else if (c == pRight - 29) {
                    RightBlack[r] = 0;
                    rightFlag = false;
                    break;
                }
            }
        }


        // if (RoadFlag.ramp) {
        //     if ((r == 0 || !rightFlag) && r < ENDROW) {
        //         for (int c = 80; c > 1; c--) {
        //             if (Bmp[r][c]  == 255 && Bmp[r][c-1]  == 0) {
        //                 LeftBlack[r] = c + 1;
        //                 break;
        //             } else if (c == 1) {
        //                 LeftBlack[r] = COL - 1;
        //                 break;
        //             }
        //         }
        //     }
        // } else {
        //         if ((r == 0 || !rightFlag) && r < ENDROW) {
        //         for (int c = 0; c < COL - 1; c++) {
        //             if (Bmp[r][c] == 0 && Bmp[r][c+1] == 255) {
        //                 RightBlack[r] = c;
        //                 break;
        //             } else if (c == COL - 2) {
        //                 RightBlack[r] = 0;
        //                 break;
        //             }
        //         }
        //     }
        // }


                if ((r == 0 || !rightFlag) && r < ENDROW) {
                for (int c = 0; c < COL - 1; c++) {
                    if (Bmp[r][c] == 0 && Bmp[r][c+1] == 255) {
                        RightBlack[r] = c;
                        break;
                    } else if (c == COL - 2) {
                        RightBlack[r] = 0;
                        break;
                    }
                }
            }
    
        // 边界矫正
        if (r > 0) {
            if (leftLine == 0 && LeftBlack[r - 1] != COL - 1) {
                leftLine = LeftBlack[r - 1];
            }                                       //首行的找边界
            if (rightLine == 0 && RightBlack[r - 1] != 0) {
                rightLine = RightBlack[r - 1];
            }

            if (LeftBlack[r] != COL - 1 && leftLine != 0) {
                if (std::fabs(LeftBlack[r] - leftLine) > 40) {
                    LeftBlack[r] = COL - 1;                 //如果当前行的边界与上一行不归边的边界相差太大则归边
                } else {
                    leftLine = LeftBlack[r];
                }
            }

            if (RightBlack[r] != 0 && rightLine != 0) {
                if (std::fabs(RightBlack[r] - rightLine) > 40) {
                    RightBlack[r] = 0;
                } else {
                    rightLine = RightBlack[r];
                }
            }
        }

        // 记录非归边左右边界行数
        if (LeftBlack[r] > 1 && LeftBlack[r] < COL - 2) leftRecord = r;
        if (RightBlack[r] > 1 && RightBlack[r] < COL - 2) rightRecord = r;

        // 边界突变查错纠正
        if (RightBlack[r] >= LeftBlack[r]) {
            if (r > 0) {
                if (std::fabs(LeftBlack[r] - LeftBlack[r - 1]) < std::fabs(RightBlack[r] - RightBlack[r - 1])) {
                    RightBlack[r] = 0;
                } else {
                    LeftBlack[r] = COL - 1;
                }
            } else {
                if (std::fabs(80 - LeftBlack[r]) > std::fabs(80 - RightBlack[r])) {
                    LeftBlack[r] = COL - 1;
                } else {
                    RightBlack[r] = 0;
                }
            }
        }

        // 前一行边界归边以及本行另一边界归边处理               用摄像头实测之后再细看
        if (r > 0) {
            if (RightBlack[r - 1] == 0 && RightBlack[r] != 0 && LeftBlack[r] == COL - 1) {
                if (RightBlack[r] >= LeftBlack[leftRecord]) {
                    RightBlack[r] = 0;
                }
            } else if (LeftBlack[r - 1] == COL - 1 && LeftBlack[r] != COL - 1 && RightBlack[r] == 0) {
                if (LeftBlack[r] <= RightBlack[rightRecord]) {
                    LeftBlack[r] = COL - 1;
                }
            }
        }

        // 记录有边界的左右边界行数、赛道宽度
        if (LeftBlack[r] != COL - 1) LongLeft = r + 1;
        if (RightBlack[r] != 0) LongRight = r + 1;

        Width[r] = LeftBlack[r] - RightBlack[r];
    }

    // 前瞻行之后寻边界
    if (LeftBlack[ENDROW - 1] != COL - 1 || RightBlack[ENDROW - 1] != 0) {
         //检测左边界是否存在
         if(LeftBlack[ENDROW - 1] == COL - 1)
            leftFlag = true;
        else
            leftFlag = false;

        //检测右边界是否存在
        if(RightBlack[ENDROW - 1] == 0)
            rightFlag = true;
         else
            rightFlag = false;

        for (int r = ENDROW; r < 40; r++) {
            // 搜寻左边界
            if (!leftFlag) {
                pLeft = LeftBlack[r - 1];
                if (pLeft > 104) pLeft = 104;
                else if (pLeft < 35) pLeft = 35;

                for (int c = pLeft - 35; c < pLeft + 35; c++) {
                    if (Bmp[r][c] == 255 && Bmp[r][c+1] == 0) {
                        LeftBlack[r] = c + 1;
                        break;
                    } else if (c == pLeft + 34) {
                        LeftBlack[r] = COL - 1;
                        break;
                    }
                }

                if (LeftBlack[r] == COL - 1 || LeftBlack[r] == 0) {
                    LongLeft = r;
                    leftFlag = true;
                }
            }

            // 搜寻右边界
            if (!rightFlag) {
                pRight = RightBlack[r - 1];
                if (pRight > 104) pRight = 104;
                else if (pRight < 35) pRight = 35;

                for (int c = pRight + 35; c > pRight - 35; c--) {
                    if (Bmp[r][c] == 255 && Bmp[r][c-1] == 0) {
                        RightBlack[r] = c - 1;
                        break;
                    } else if (c == pRight - 34) {
                        RightBlack[r] = 0;
                        break;
                    }
                }

                if (RightBlack[r] == COL - 1 || RightBlack[r] == 0) {
                    LongRight = r;
                    rightFlag = true;
                }
            }

            // 边界矫正
            if (LeftBlack[r] != COL - 1 && leftLine != 0) {
                if (std::fabs(LeftBlack[r] - leftLine) > 35) {
                    LeftBlack[r] = COL - 1;
                } else {
                    leftLine = LeftBlack[r];
                }
            }

            if (RightBlack[r] != 0 && rightLine != 0) {
                if (std::fabs(RightBlack[r] - rightLine) > 35) {
                    RightBlack[r] = 0;
                } else {
                    rightLine = RightBlack[r];
                }
            }

            // 记录非归边左右边界行数
            if (LeftBlack[r] > 1 && LeftBlack[r] < COL - 2) leftRecord = r;
            if (RightBlack[r] > 1 && RightBlack[r] < COL - 2) rightRecord = r;

            // // 边界突变查错纠正
            // if (RightBlack[r] >= LeftBlack[r]) {
            //     if (std::fabs(LeftBlack[r] - LeftBlack[r - 1]) < std::fabs(RightBlack[r] - RightBlack[r - 1])) {
            //         RightBlack[r] = 0;
            //     } else {
            //         LeftBlack[r] = COL - 1;
            //     }
            // }

            // 边界突变查错纠正
        if (RightBlack[r] >= LeftBlack[r]) {
            if (r > 0) {
                if (std::fabs(LeftBlack[r] - LeftBlack[r - 1]) < std::fabs(RightBlack[r] - RightBlack[r - 1])) {
                    RightBlack[r] = 0;
                } else {
                    LeftBlack[r] = COL - 1;
                }
            } else {
                if (std::fabs(80 - LeftBlack[r]) > std::fabs(80 - RightBlack[r])) {
                    LeftBlack[r] = COL - 1;
                } else {
                    RightBlack[r] = 0;
                }
            }
        }

            // 前一行边界归边以及本行另一边界归边处理
            if (RightBlack[r - 1] == 0 && RightBlack[r] != 0 && LeftBlack[r] == COL - 1) {
                if (RightBlack[r] >= LeftBlack[leftRecord]) {
                    RightBlack[r] = 0;
                }
            } else if (LeftBlack[r - 1] == COL - 1 && LeftBlack[r] != COL - 1 && RightBlack[r] == 0) {
                if (LeftBlack[r] <= RightBlack[rightRecord]) {
                    LeftBlack[r] = COL - 1;
                }
            }

            // 记录有边界的左右边界行数、赛道宽度
            if (LeftBlack[r] != COL - 1) LongLeft = r + 1;
            if (RightBlack[r] != 0) LongRight = r + 1;

            if (!leftFlag || !rightFlag) {
                Width[r] = LeftBlack[r] - RightBlack[r];
            } else {
                break;
            }
        }


        for (int r = 40; r < ROW; r++) {
            // 搜寻左边界
            if (!leftFlag) {
                pLeft = LeftBlack[r - 1];
                if (pLeft > 119) pLeft = 119;
                else if (pLeft < 20) pLeft = 20;

                for (int c = pLeft - 20; c < pLeft + 20; c++) {
                    if (Bmp[r][c] == 255 && Bmp[r][c+1] == 0) {
                        LeftBlack[r] = c + 1;
                        break;
                    } else if (c == pLeft + 19) {
                        LeftBlack[r] = COL - 1;
                        break;
                    }
                }

                if (LeftBlack[r] == COL - 1 || LeftBlack[r] == 0) {
                    LongLeft = r;
                    leftFlag = true;
                }
            }

            // 搜寻右边界
            if (!rightFlag) {
                pRight = RightBlack[r - 1];
                if (pRight > 119) pRight = 119;
                else if (pRight < 20) pRight = 20;

                for (int c = pRight + 20; c > pRight - 20; c--) {
                    if (Bmp[r][c] == 255 && Bmp[r][c-1] == 0) {
                        RightBlack[r] = c - 1;
                        break;
                    } else if (c == pRight - 19) {
                        RightBlack[r] = 0;
                        break;
                    }
                }

                if (RightBlack[r] == COL - 1 || RightBlack[r] == 0) {
                    LongRight = r;
                    rightFlag = true;
                }
            }

            // 边界矫正
            if (LeftBlack[r] != COL - 1 && leftLine != 0) {
                if (std::fabs(LeftBlack[r] - leftLine) > 20) {
                    LeftBlack[r] = COL - 1;
                } else {
                    leftLine = LeftBlack[r];
                }
            }

            if (RightBlack[r] != 0 && rightLine != 0) {
                if (std::fabs(RightBlack[r] - rightLine) > 20) {
                    RightBlack[r] = 0;
                } else {
                    rightLine = RightBlack[r];
                }
            }

            // 记录非归边左右边界行数
            if (LeftBlack[r] > 1 && LeftBlack[r] < COL - 2) leftRecord = r;
            if (RightBlack[r] > 1 && RightBlack[r] < COL - 2) rightRecord = r;

            // // 边界突变查错纠正
            // if (RightBlack[r] >= LeftBlack[r]) {
            //     if (std::fabs(LeftBlack[r] - LeftBlack[r - 1]) < std::fabs(RightBlack[r] - RightBlack[r - 1])) {
            //         RightBlack[r] = 0;
            //     } else {
            //         LeftBlack[r] = COL - 1;
            //     }
            // }

            // 边界突变查错纠正
        if (RightBlack[r] >= LeftBlack[r]) {
            if (r > 0) {
                if (std::fabs(LeftBlack[r] - LeftBlack[r - 1]) < std::fabs(RightBlack[r] - RightBlack[r - 1])) {
                    RightBlack[r] = 0;
                } else {
                    LeftBlack[r] = COL - 1;
                }
            } else {
                if (std::fabs(80 - LeftBlack[r]) > std::fabs(80 - RightBlack[r])) {
                    LeftBlack[r] = COL - 1;
                } else {
                    RightBlack[r] = 0;
                }
            }
        }

            // 前一行边界归边以及本行另一边界归边处理
            if (RightBlack[r - 1] == 0 && RightBlack[r] != 0 && LeftBlack[r] == COL - 1) {
                if (RightBlack[r] >= LeftBlack[leftRecord]) {
                    RightBlack[r] = 0;
                }
            } else if (LeftBlack[r - 1] == COL - 1 && LeftBlack[r] != COL - 1 && RightBlack[r] == 0) {
                if (LeftBlack[r] <= RightBlack[rightRecord]) {
                    LeftBlack[r] = COL - 1;
                }
            }

            // 记录有边界的左右边界行数、赛道宽度
            if (LeftBlack[r] != COL - 1) LongLeft = r + 1;
            if (RightBlack[r] != 0) LongRight = r + 1;

            if (!leftFlag || !rightFlag) {
                Width[r] = LeftBlack[r] - RightBlack[r];
            } else {
                break;
            }
        }
    }

} 

//-------------------------------------------------------------------------------------------------------------------
//  @brief      合成中线函数
//-------------------------------------------------------------------------------------------------------------------
void Fitting_MiddleLine(void)
{
    uint8 r;
    //左边界长度大于右边界长度
    if(Longest_White_Column_Left[0]>Longest_White_Column_Right[0])
    {
        for(r = 0; r < Longest_White_Column_Right[0]; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
        for(r = Longest_White_Column_Right[0]; r < Longest_White_Column_Left[0];r++)
        {
            MiddleLine[r] = LeftBlack[r] >> 1;
        }
    }
    //右边界长度大于左边界长度
    else if(Longest_White_Column_Left[0] < Longest_White_Column_Right[0])
    {
        for(r = 0; r < Longest_White_Column_Left[0]; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
        for(r = Longest_White_Column_Left[0]; r < Longest_White_Column_Right[0]; r++)
        {
            MiddleLine[r] = (RightBlack[r] + COL - 1) >> 1;
        }
    }
    //左右边界长度相等
    else
    {
        for(r = 0; r < Longest_White_Column_Left[0]; r++)
        {
            MiddleLine[r] = (LeftBlack[r] + RightBlack[r]) >> 1;
        }
    }

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计列白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按列扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountColWhite(void)
{
    uint8 r, c;
    //列白点数组清0

    memset(WhiteCol, 0, sizeof(WhiteCol));

    for(c = 0; c < COL; c++)
    {
        for(r = 0; r < ROW; r++)
        {
            if(Bmp[r][c])       WhiteCol[c]++;
            else                break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      统计行白点数函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    按行扫描图像，统计连续白点数
//-------------------------------------------------------------------------------------------------------------------
void CountRowWhite(void)
{
    uint8 r, c;
    uint8 *p = NULL;

    //行白点数组清0
    memset(WhiteRow, 0, sizeof(WhiteRow));
    // memset(Cross_L_WhiteRow, 0, sizeof(Cross_L_WhiteCol));
    // memset(Cross_R_WhiteRow, 0, sizeof(Cross_R_WhiteCol));

    for(r = 0; r < ROW; r++)
    {
        p = &Bmp[r][0];
        for(c = MID; c > 0; c--)
        {
            if(*(p + c) == 255)       
            {
                WhiteRow[r]++;
                // Cross_L_WhiteRow[r]++;
            }
                else                
                break;
        }
        for(c = MID + 1; c < COL; c++)
        {
            if(*(p + c) == 255)
            {
                WhiteRow[r]++;
                // Cross_R_WhiteRow[r]++;
            }
            else 
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      十字统计列白点数函数
//-------------------------------------------------------------------------------------------------------------------
void Cross_CountColWhite(void)
{
    uint8 c,r;

    memset(Cross_WhiteCol, 0, sizeof(Cross_WhiteCol));

    for(c=0;c<139;c++)
    {
        for(r=25;r<ROW-3;r++)
        {
            if(Bmp[r][c]==255)
            {
                Cross_WhiteCol[c]++;
                if(Bmp[r][c]==255 &&Bmp[r+1][c]==0 && Bmp[r+2][c]==0 && Bmp[r+3][c]==0)         //白黑黑黑
                {
                    break;
                }
            }
        }
        for(r=24;r>3;r--)
        {
            if(Bmp[r][c]==255)// && Bmp[r+1][c]==0 && Bmp[r+2][c]==0 && Bmp[r+3][c]==0
            {
                Cross_WhiteCol[c]++;
                if(Bmp[r][c]==255 &&Bmp[r-1][c]==0 && Bmp[r-2][c]==0 && Bmp[r-3][c]==0)
                {
                    break;
                }
            }
        }
    }  
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      出弯时统计行白点数函数
//-------------------------------------------------------------------------------------------------------------------
void obstacle_CountRowWhite(void)
{
    uint8 r, c;
    uint8 *p = NULL;

    memset(obstacle_L_WhiteRow, 0, sizeof(obstacle_L_WhiteRow));
    memset(obstacle_R_WhiteRow, 0, sizeof(obstacle_R_WhiteRow));

    for(r=0;r<ROW;r++)
    {
        p = &Bmp[r][0];
        for(c=90;c<COL;c++)
        {
            if(*(p + c) == 255)       
            obstacle_L_WhiteRow[r]++;
            else                
            break;
        }
        for(c=89;c>0;c--)
        {
            if(*(p+c) == 255)
            obstacle_L_WhiteRow[r]++;
            else
            break;
        }
        for(c=30;c<COL;c++)
        {
            if(*(p + c) == 255)
            obstacle_R_WhiteRow[r]++;
            else
            break;
        }
        for(c=29;c>0;c--)
        {
            if(*(p + c) == 255)
            obstacle_R_WhiteRow[r]++;
            else
            break;
        }

    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      前瞻行左右调转函数
//-------------------------------------------------------------------------------------------------------------------
static void forwardMove(void)
{
    uint8 c = 0;

    WhiteSum = 0;

    for(c = 0; c < COL; c++)
    {
        forwardLook[c] = Bmp[DataInfo.forwardLook][COL - 1 - c];

        //统计前瞻行白点数
        if(forwardLook[c])
            WhiteSum++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      前瞻边缘提取函数
//-------------------------------------------------------------------------------------------------------------------
//  @explain    由于forwardLook_move() 函数将前瞻行调转了，此函数中前瞻的左边界是实际的左边界
//-------------------------------------------------------------------------------------------------------------------
void Prospect_forwardLook(void)
{
    uint8 i = 0,j = 0;
    LastLeft = Left;
    LastRight = Right;

    //前瞻行左右调转
    forwardMove();

    //前瞻行边界大小调转
    Left = 139-LeftBlack[DataInfo.forwardLook];
    Right = 139- RightBlack[DataInfo.forwardLook];

   
        //////检查纠正
        if(Left > Right) //两个判断并非程序写错，可以屏蔽掉其中一个跑一下看现象
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }

        if(fabs(Left - LastLeft) > 60)
        {
            Left = LastLeft;
        }
        if(fabs(Right - LastRight) > 60)
        {
            Right = LastRight;
        }
#if 0
        if(Left > Right) //两个判断并非程序写错，可以屏蔽掉其中一个跑一下看现象
        {
            if(fabs(Left - LastLeft) > fabs(Right - LastRight))
            {
                Left = 0;
            }
            else
            {
                Right = COL - 1;
            }
        }
#endif
}

//迷宫法巡线参数
uint8 start_point_l[2] = {0};//左边起点的x，y值
uint8 start_point_r[2] = {0};//右边起点的x，y值
uint16 points_l[(uint16)USE_num][2] = {{0}};//左线
uint16 points_r[(uint16)USE_num][2] = {{0}};//右线
uint16 dir_r[(uint16)USE_num] = {0};//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = {0};//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint8 Road_HighestPoint = 0;//最高点

//-------------------------------------------------------------------------------------------------------------------
// //  @brief      功能说明：寻找两个边界的边界点作为八邻域循环的起始点
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------
uint8 get_start_point(uint8 start_row)
{
	uint8 i = 0,l_found = 0,r_found = 0;
	//清零
	start_point_l[0] = 0;//x
	start_point_l[1] = 0;//y

	start_point_r[0] = 0;//x
	start_point_r[1] = 0;//y

		//从中间往右边，先找起点  //因为摄像头倒置，所以找的是左边的边界点
	for (i = COL / 2; i < border_max; i++)
	{
		start_point_l[0] = i;//x
		start_point_l[1] = start_row;//y
		if (Bmp[start_row][i]==255&&Bmp[start_row][i+1]==0)
		{
			//printf("找到左边起点image[%d][%d]\n", start_row,i);
			l_found = 1;
			break;
		}
	}

	for (i = COL / 2; i > border_min; i--)
	{
		start_point_r[0] = i;//x
		start_point_r[1] = start_row;//y
		if (Bmp[start_row][i]==255&&Bmp[start_row][i-1]==0)
		{
			//printf("找到右边起点image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}

	if(l_found&&r_found)
    return 1;
	else {
		// printf("未找到起点\n");
		return 0;
	} 
}
//-------------------------------------------------------------------------------------------------------------------
// //  @brief      边缘提取函数（迷宫法） 
// //-------------------------------------------------------------------------------------------------------------------
// //  @explain    1、由于摄像头倒装，以下函数的左边界为实际中的左边界，即倒装后图像的右边界
// //              2、此函数注释中叙述的左边界均为倒装图像的右边界
//-------------------------------------------------------------------------------------------------------------------

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
							uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
example：
	search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,start_point_l[0],
				start_point_l[1], start_point_r[0], start_point_r[1],&hightest);
*/
void search_l_r(uint16 break_flag, uint8(*image)[COL], 
                uint16 *l_stastic, uint16 *r_stastic, 
                uint8 l_start_x, uint8 l_start_y, 
                uint8 r_start_x, uint8 r_start_y, 
                uint8*hightest)
{
    uint8 i=0, j=0;
    //左边变量
    uint8 search_filds_l[8][2] = {{0}};
    uint16 index_l = 0;
    uint8 temp_l[8][2] = {{0}};
    uint8 center_point_l[2] = {0};//中心坐标点 
    uint16 l_data_statics;//统计左边
	//定义八个邻域
	static int8 seeds_l[8][2] = { {0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1},};
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针
    //注意一点，找到的第一个生长方向是1，不是0，最后一个8才是0;

	//右边变量
    uint8 search_filds_r[8][2] = {{0}};
    uint16 index_r = 0;
    uint8 temp_r[8][2] = {{0}};
    uint8 center_point_r[2] = {0};//中心坐标点
    uint16 r_data_statics;//统计右边
	//定义八个邻域
	static int8 seeds_r[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1}, {-1,0},{-1,1},};
    //{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是逆时针

    l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
    r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

    //第一次更新坐标点  将找到的起点值传进来
    center_point_l[0] = l_start_x;//x
    center_point_l[1] = l_start_y;//y
    center_point_r[0] = r_start_x;//x
    center_point_r[1] = r_start_y;//y

    while (break_flag--)
    {   
        //将左边界寻到的点存进数组
        for(i=0; i<8; i++)
        {           
            search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];//x
            search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_l[l_data_statics][0] = center_point_l[0];//x
        points_l[l_data_statics][1] = center_point_l[1];//y



        //将右边界寻到的点存进数组
        for (i = 0; i < 8; i++)//传递8F坐标
        {
            search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];//x
            search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];//y
        }
        //中心坐标点填充到已经找到的点内
        points_r[r_data_statics][0] = center_point_r[0];//x
        points_r[r_data_statics][1] = center_point_r[1];//y

        l_data_statics++;//索引加一

        index_l = 0;//先清零，后使用
        for (i = 0; i < 8; i++)
        {
            temp_l[i][0] = 0;//先清零，后使用
            temp_l[i][1] = 0;//先清零，后使用
        }

        //左边判断
       for (i = 0; i < 8; i++)
       {
           if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0
               && image[search_filds_l[(i + 1) & 7][1]][search_filds_l[(i + 1) & 7][0]] == 255)
           {
               temp_l[index_l][0] = search_filds_l[(i)][0];
               temp_l[index_l][1] = search_filds_l[(i)][1];
               index_l++;
               dir_l[l_data_statics - 1] = (i);//记录生长方向
           }

           if (index_l)
           {
               //更新坐标点
               center_point_l[0] = temp_l[0][0];//x
               center_point_l[1] = temp_l[0][1];//y
               for (j = 0; j < index_l; j++)
               {
                   if (center_point_l[1] < temp_l[j][1])
                   {
                       center_point_l[0] = temp_l[j][0];//x
                       center_point_l[1] = temp_l[j][1];//y
                   }
               }
           }

       }
       if ((points_r[r_data_statics][0]== points_r[r_data_statics-1][0]&& points_r[r_data_statics][0] == points_r[r_data_statics - 2][0]
           && points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] && points_r[r_data_statics][1] == points_r[r_data_statics - 2][1])
           ||(points_l[l_data_statics-1][0] == points_l[l_data_statics - 2][0] && points_l[l_data_statics-1][0] == points_l[l_data_statics - 3][0]
               && points_l[l_data_statics-1][1] == points_l[l_data_statics - 2][1] && points_l[l_data_statics-1][1] == points_l[l_data_statics - 3][1]))
       {
           //printf("三次进入同一个点，退出\n");
           break;
       }
       if (std::abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) < 2
           && std::abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1])< 2
           )
       {
           //printf("\n左右相遇退出\n");	
           *hightest = (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;//取出最高点
           //printf("\n在y=%d处退出\n",*hightest);
           break;
       }
       if ((dir_l[l_data_statics - 1] == 7 &&points_r[r_data_statics][1] > points_l[l_data_statics - 1][1]))//右边比左边高了，且左边在寻直线，右边等待左边 dir_l[l_data_statics - 1] == 7 &&
       {
        //    printf("\n如果右边比左边高了，右边等待左边\n");	
           continue;//此时右边比左边高了，右边等待左边，让左边多寻一次线
       }
       if ((dir_l[l_data_statics - 1] == 7 &&points_r[r_data_statics][1] < points_l[l_data_statics - 1][1]))
       {
           //此时左边比右边高了，左边等待右边，让右边多寻一次线
           //printf("\n左边开始向下了，等待右边，等待中... \n");
           center_point_l[0] = points_l[l_data_statics - 1][0];//x
           center_point_l[1] = points_l[l_data_statics - 1][1];//y
           l_data_statics--;
       }


       r_data_statics++;//索引加一
       index_r = 0;//先清零，后使用
       for (i = 0; i < 8; i++)
       {
           temp_r[i][0] = 0;//先清零，后使用
           temp_r[i][1] = 0;//先清零，后使用
       }

        //右边判断
        for (i = 0; i < 8; i++)
        {
            if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0
                && image[search_filds_r[(i + 1) & 7][1]][search_filds_r[(i + 1) & 7][0]] == 255)
            {
                temp_r[index_r][0] = search_filds_r[(i)][0];
                temp_r[index_r][1] = search_filds_r[(i)][1];
                index_r++;//索引加一
                dir_r[r_data_statics - 1] = (i);//记录生长方向
                //printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
            }
            if (index_r)
            {
 
                //更新坐标点
                center_point_r[0] = temp_r[0][0];//x
                center_point_r[1] = temp_r[0][1];//y
                for (j = 0; j < index_r; j++)
                {
                    if (center_point_r[1] > temp_r[j][1])
                    {
                        center_point_r[0] = temp_r[j][0];//x
                        center_point_r[1] = temp_r[j][1];//y
                    }
                }
 
            }
        }

    }
   //取出循环次数
   *l_stastic = l_data_statics;
   *r_stastic = r_data_statics;
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      提取有效左右边界函数
//-------------------------------------------------------------------------------------------------------------------

/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L	：找到的点的总数
example： get_left(data_stastics_l);
 */
void get_left(uint16 total_L)
{
	uint16 j = 0;
	uint8 h = 0;
    uint8 Long_Left=0;
    //初始化数组
    std::memset(LeftBlack, COL - 1, sizeof(LeftBlack));
    h=2;
    for(j=0;j<total_L;j++)
    {
        if((points_l[j][0]!=COL-2||points_l[j][0]!=1) && Long_Left == 0 )
        {
            LongLeft=points_l[j][1];
        }
        if((points_l[j][0]==COL-2||points_l[j][0]==1) && Long_Left == 0 && LongLeft < points_l[j][1])
        {
            LongLeft=points_l[j][1];
            Long_Left=1;
        }
        if(points_l[j][1]==h)
        LeftBlack[h]=points_l[j][0];
        else
        continue;
        h++;
        if(h>=ROW-3) {
        if(Long_Left == 0)
        {
            LongLeft=h;
            Long_Left=1;
        }
        break;
        }
    }
}
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
	uint16 j = 0;
	uint8 h = 0;
    uint8 Long_Right=0;
	//初化
    std::memset(RightBlack, 0, sizeof(RightBlack));
	h = 2;
    if((points_r[j][0]!=COL-2||points_r[j][0]!=1) && Long_Right == 0 )
    {
        LongRight=points_r[j][1];
    }
    if((points_r[j][0]==COL-2||points_r[j][0]==1) && Long_Right == 0 && LongRight < points_r[j][1])
    {
        LongRight=points_r[j][1];
        Long_Right=1;
    }
	//右边
	for (j = 0; j < total_R; j++)
	{
		if (points_r[j][1] == h)
			RightBlack[h] = points_r[j][0];
		else 
        continue;//每行只取一个点，没到下一行就不记录
        h++;
		if (h == ROW - 3) {
        if(Long_Right == 0)
        {
            LongRight=h;
            Long_Right=1;
        }    
		break;//到最后一行退出始
        }
	}
}

/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w]	图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[COL])
{

	uint8 i = 0;
	for (i = 0; i < ROW; i++)
	{
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][COL - 1] = 0;
		image[i][COL - 2] = 0;

	}
	for (i = 0; i < COL; i++)
	{
		image[0][i] = 0;
		image[1][i] = 0;
        image[COL-1][i] = 0;
		image[COL-2][i] = 0;

	}
}

//边线处理代码
void image_process(void)
{
//uint16 i;
uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
image_draw_rectan(Bmp);
//清零
data_stastics_l = 0;
data_stastics_r = 0;
if(get_start_point(2))
{ //迷宫法巡线
 printf("正在开始八领域\n");
 search_l_r((uint16)USE_num, Bmp, &data_stastics_l, &data_stastics_r,start_point_l[0],start_point_l[1],start_point_r[0],start_point_r[1],&hightest);
 printf("八邻域已结束\n");
 printf("左边点的个数为：%d\n,右边点的个数为：%d\n", data_stastics_l,data_stastics_r);
 //提取有效边线
 get_left(data_stastics_l);
 get_right(data_stastics_r);  
 Road_HighestPoint = hightest;
        // // 拟合中线
        Fitting_MiddleLine();
        // 统计列白点
        CountColWhite();
        // 统计行白点
        CountRowWhite();
        //统计十字白点
        Cross_CountColWhite();
        // 前瞻行处理
        Prospect_forwardLook();
        //十字补线有问题得再看看
        // cross_fill(Bmp, LeftBlack, RightBlack, data_statastics_l, data_statastics_r,
		// 								 dir_l, dir_r, points_l, points_r);

}
}



void sigint_handler(int signum) 
{
    printf("收到Ctrl+C，程序即将退出\n");
    exit(0);
}

void cleanup()
{
    printf("程序异常退出，执行清理操作\n");
    // 处理程序退出！！！
    // 这里需要关闭电机，关闭电调等。
}

/******************************************************逐飞助手*****************************************************/
/******************************************************************************************************************/

// 只有X边界
uint8 xy_x1_boundary[BOUNDARY_NUM], xy_x2_boundary[BOUNDARY_NUM], xy_x3_boundary[BOUNDARY_NUM];

// 只有Y边界
uint8 xy_y1_boundary[BOUNDARY_NUM], xy_y2_boundary[BOUNDARY_NUM], xy_y3_boundary[BOUNDARY_NUM];

// X Y边界都是单独指定的
uint8 x1_boundary[CROP_H], x2_boundary[CROP_H], x3_boundary[CROP_H];
uint8 y1_boundary[CROP_W], y2_boundary[CROP_W], y3_boundary[CROP_W];


uint8 image_copy[CROP_H][CROP_W];

void transfer_image(void)
{

#if(1 == INCLUDE_BOUNDARY_TYPE || 2 == INCLUDE_BOUNDARY_TYPE || 4 == INCLUDE_BOUNDARY_TYPE)
    int32 i = 0;
#elif(3 == INCLUDE_BOUNDARY_TYPE)
int32 i = 0;
    int32 j = 0;
#endif

    // // 逐飞助手初始化 设置回调函数
    // seekfree_assistant_interface_init(tcp_client_send_data, tcp_client_read_data);

#if(0 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(仅包含原始图像信息)
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);


#elif(1 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < CROP_H; i++)
    {
        // // x1_boundary[i] = 50 - (50 - 20) * i / CROP_H;
        // // x3_boundary[i] = 70 + (148 - 70) * i / CROP_H;


        x2_boundary[i] = COL-1-MiddleLine[49-i];
        x1_boundary[i] = COL-1-LeftBlack[49-i];
        x3_boundary[i] = COL-1-RightBlack[49-i];

                
        // x2_boundary[i] = MiddleLine[i];
        // x1_boundary[i] = LeftBlack[i];
        // x3_boundary[i] = RightBlack[i];
    }
    
    //     for(i = 0; i < CROP_W; i++)
    // {

    //     y1_boundary[i] = ROW-1-(DataInfo.forwardLook-2);
    //     y2_boundary[i] = ROW-1-(DataInfo.forwardLook+4);
    //     y3_boundary[i] = ROW-1-(DataInfo.forwardLook+10);
        
    // }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    // seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, Bmp[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, CROP_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);
    // seekfree_assistant_camera_boundary_config(Y_BOUNDARY, CROP_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);



#elif(2 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有纵轴坐标，横轴坐标由图像宽度得到，意味着每个边界在一列中只会有一个点)
    // 通常很少有这样的使用需求
    // 对边界数组写入数据
    for(i = 0; i < CROP_W; i++)
    {
        // y1_boundary[i] = 50 - (50 - 20) * i / CROP_H;
        // y2_boundary[i] = CROP_W / 2;
        // y3_boundary[i] = 78 + (78 - 58) * i / CROP_H;
        
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(Y_BOUNDARY, CROP_W, NULL, NULL ,NULL, y1_boundary, y2_boundary, y3_boundary);


#elif(3 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息含有横纵轴坐标)
    // 这样的方式可以实现对于有回弯的边界显示
    j = 0;
    for(i = CROP_H - 1; i >= CROP_H / 2; i--)
    {
        // 直线部分
        xy_x1_boundary[j] = 34;
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = 47;
        xy_y2_boundary[j] = i;
        
        xy_x3_boundary[j] = 60;
        xy_y3_boundary[j] = i;
        j++;
    }

    for(i = CROP_H / 2 - 1; i >= 0; i--)
    {
        // 直线连接弯道部分
        xy_x1_boundary[j] = 34 + (CROP_H / 2 - i) * (CROP_W / 2 - 34) / (CROP_H / 2);
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = 47 + (CROP_H / 2 - i) * (CROP_W / 2 - 47) / (CROP_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;
        
        xy_x3_boundary[j] = 60 + (CROP_H / 2 - i) * (CROP_W / 2 - 60) / (CROP_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }

    for(i = 0; i < CROP_H / 2; i++)
    {
        // 回弯部分
        xy_x1_boundary[j] = CROP_W / 2 + i * (138 - CROP_W / 2) / (CROP_H / 2);
        xy_y1_boundary[j] = i;
        
        xy_x2_boundary[j] = CROP_W / 2 + i * (133 - CROP_W / 2) / (CROP_H / 2);
        xy_y2_boundary[j] = 15 + i * 3 / 4;
        
        xy_x3_boundary[j] = CROP_W / 2 + i * (128 - CROP_W / 2) / (CROP_H / 2);
        xy_y3_boundary[j] = 30 + i / 2;
        j++;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, transfer[0], CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(XY_BOUNDARY, BOUNDARY_NUM, xy_x1_boundary, xy_x2_boundary, xy_x3_boundary, xy_y1_boundary, xy_y2_boundary, xy_y3_boundary);


#elif(4 == INCLUDE_BOUNDARY_TYPE)
    // 发送摄像头图像图像信息(并且包含三条边界信息，边界信息只含有横轴坐标，纵轴坐标由图像高度得到，意味着每个边界在一行中只会有一个点)
    // 对边界数组写入数据
    for(i = 0; i < CROP_H; i++)
    {
        x1_boundary[i] = 70 - (70 - 20) * i / CROP_H;
        x2_boundary[i] = CROP_W / 2;
        x3_boundary[i] = 80 + (159 - 80) * i / CROP_H;
    }
    seekfree_assistant_camera_information_config(SEEKFREE_ASSISTANT_MT9V03X, NULL, CROP_W, CROP_H);
    seekfree_assistant_camera_boundary_config(X_BOUNDARY, CROP_H, x1_boundary, x2_boundary, x3_boundary, NULL, NULL ,NULL);


#endif


        // // 阻塞式等待，图像刷新
        // if(wait_image_refresh() < 0)
        // {
        //     // 摄像头未采集到图像，这里需要关闭电机，关闭电调等。
        //     exit(0);
        // }


        seekfree_assistant_camera_send();



}

/******************************************************main控制函数*************************************************/
/******************************************************************************************************************/
int main(int, char**) 
{

    // 注册清理函数
    atexit(cleanup);

    // 注册SIGINT信号的处理函数
    signal(SIGINT, sigint_handler);

    // 初始化屏幕
    ips200_init("/dev/fb0");

    // 初始化UVC摄像头
    if(uvc_camera_init("/dev/video0") < 0)
    {
        return -1;
    }

    while(1)
    {
 /*      // 阻塞式等待，图像刷新
        if(wait_image_refresh() < 0)
        {
            // 摄像头未采集到图像，这里需要关闭电机，关闭电调等。
            exit(0);
        }
*/
        pthread_mutex_lock(&mutex); 

        // printf("RoadFlag.cross=%d\n",RoadFlag.cross);
        // printf("Cross_WhiteCol[60]=%d\n",Cross_WhiteCol[60]);
        //边缘提取
        Prospect_Border();        
        // 拟合中线
        Fitting_MiddleLine();
        // 统计列白点
        CountColWhite();
        // 统计行白点
        CountRowWhite();
        //统计十字白点
        Cross_CountColWhite();
        // //统计出弯白点
        // obstacle_CountRowWhite();        
        // //迷宫法巡线图像处理
        // image_process();   


        // 前瞻行处理
        Prospect_forwardLook();
    //    Protect(Bmp[0], 5);  //原本5， 但是出现好几次跑着跑着突然在赛道上进保护，然后停车的情况，所以改成6，看看效果怎么样
    //    LoseLineProtect();
        pthread_mutex_unlock(&mutex); 

        transfer_image();
        // 显示图像到屏幕上
        //ips200_show_gray_image(0, 0, rgay_image, UVC_WIDTH, UVC_HEIGHT);
    }
}