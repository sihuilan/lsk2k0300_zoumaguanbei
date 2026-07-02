#ifndef CODE_IMAGE_INC_BINARY_IMAGE_H_
#define CODE_IMAGE_INC_BINARY_IMAGE_H_

typedef struct
{
    float threshold;        //二值化阈值
    float lastThreshold;    //上一个二值化阈值
}ImageInfomationType;

#pragma once

extern ImageInfomationType ImageInfo;

// extern uint8 Bmp[UVC_HEIGHT][UVC_WIDTH];
// extern uint8 rgay[UVC_HEIGHT][UVC_WIDTH];

extern uint8 Bmp[CROP_H][CROP_W];
extern uint8 rgay[CROP_H][CROP_W];
extern uint8 image[UVC_HEIGHT][UVC_WIDTH];  //切割中转数组
extern uint8 transfer[CROP_H][CROP_W];       //图传转移数组
uint8 Iterative_Threshold(const cv::Mat& image, bool minMaxDel);
void crop_image(uint8 src[UVC_HEIGHT][UVC_WIDTH],
                uint8 dest[CROP_H][CROP_W],
                int startRow, int startCol, int cropH, int cropW); 
#endif