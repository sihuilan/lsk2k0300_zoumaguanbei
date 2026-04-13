#ifndef _zf_driver_uvc_h
#define _zf_driver_uvc_h


#include "zf_common_typedef.h"
#include <opencv2/opencv.hpp>

#define UVC_WIDTH   160
#define UVC_HEIGHT  120
#define UVC_FPS     120
#define CROP_H      50
#define CROP_W      140  //祖传120 迷宫法94

// #define CROP_H      40
// #define CROP_W      120         //祖传

#define ADAPTIVE_BLOCK_SIZE 31  // 局部邻域大小（必须为奇数）
#define ADAPTIVE_C        10     // 从均值中减去的常数


int8 uvc_camera_init(const char *path);
int8 wait_image_refresh();


extern cv::Mat frame_rgay;      // 构建opencv对象 灰色
extern uint8_t *rgay_image;
extern uint8_t *binary_image;


#endif
