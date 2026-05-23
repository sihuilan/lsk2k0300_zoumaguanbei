#include "zf_common_headfile.h"

ImageInfomationType ImageInfo;

// uint8 Bmp[UVC_HEIGHT][UVC_WIDTH];
// uint8 rgay[UVC_HEIGHT][UVC_WIDTH];

uint8 Bmp[CROP_H][CROP_W];
uint8 rgay[CROP_H][CROP_W];
uint8 image[UVC_HEIGHT][UVC_WIDTH];
uint8 transfer[CROP_H][CROP_W];

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

