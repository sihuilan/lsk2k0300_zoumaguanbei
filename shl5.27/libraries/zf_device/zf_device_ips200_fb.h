#ifndef __zf_device_ips200_fb_h__
#define __zf_device_ips200_fb_h__

#include "zf_common_typedef.h"

#define IPS200_DEFAULT_PENCOLOR         (RGB565_WHITE    )                        // 默认的画笔颜色
#define IPS200_DEFAULT_BGCOLOR          (RGB565_BLACK  )                        // 默认的背景颜色
#define IPS200_PEN                      (RGB565_RED    )                        // 图像界面的画笔颜色



void    ips200_clear            (void);
void    ips200_full             (const uint16 color);
void    ips200_draw_point       (uint16 x, uint16 y, const uint16 color);
void    ips200_draw_line        (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips200_show_char        (uint16 x, uint16 y, const char dat);
void    ips200_show_string      (uint16 x, uint16 y, const char dat[]);
void    ips200_show_int         (uint16 x, uint16 y, const int32 dat, uint8 num);
void    ips200_show_uint        (uint16 x, uint16 y, const uint32 dat, uint8 num);
void    ips200_show_float       (uint16 x, uint16 y, const double dat, uint8 num, uint8 pointnum);

void    ips200_show_gray_image  (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height);

void    ips200_init             (const char *path);

//===================================================参数高光显示==================================================
void    ips200_show_charB           (uint16 x, uint16 y, const char dat);
void    ips200_show_charE           (uint16 x, uint16 y, const char dat, uint16 color);

void    ips200_show_stringB         (uint16 x, uint16 y, const char dat[]);
void    ips200_show_stringE         (uint16 x, uint16 y, const char dat[], uint16 color);

void    ips200_showint8             (uint16 x, uint16 y, int8 dat);
void    ips200_showint8B            (uint16 x, uint16 y, int8 dat);
void    ips200_showint8E            (uint16 x, uint16 y, int8 dat, uint16 color);
void    ips200_showuint8            (uint16 x, uint16 y, uint8 dat);
void    ips200_showuint8B           (uint16 x, uint16 y, uint8 dat);
void    ips200_showuint8E           (uint16 x, uint16 y, uint8 dat, uint16 color);

void    ips200_showint16            (uint16 x,uint16 y,int16 dat);
void    ips200_showint16B           (uint16 x,uint16 y,int16 dat);
void    ips200_showint16E           (uint16 x, uint16 y, int16 dat, uint16 color);
void    ips200_showuint16           (uint16 x,uint16 y,uint16 dat);
void    ips200_showuint16B          (uint16 x,uint16 y,uint16 dat);
void    ips200_showuint16E          (uint16 x, uint16 y, uint16 dat, uint16 color);

void    ips200_showint32B           (uint16 x,uint16 y,int32 dat,uint8 num);
void    ips200_showint32E           (uint16 x, uint16 y, int32 dat, uint8 num, uint16 color);
void    ips200_showfloatB           (uint16 x, uint16 y, float dat, uint8 num, uint8 pointnum);
void    ips200_showfloatE           (uint16 x, uint16 y, float dat, uint8 num, uint8 pointnum, uint16 color);

void    ips200_displayimage032_zoomB(const uint8_t* image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height); 
// void    ips200_displayimageB                (const uint8 *image, uint16 x, uint16 y, uint16 width, uint16 height);
// void ips200_show_color_roi(uint16 x_offset, uint16 y_offset, const cv::Mat& cropped_bgr, uint16 dis_width, uint16 dis_height) ;
#endif