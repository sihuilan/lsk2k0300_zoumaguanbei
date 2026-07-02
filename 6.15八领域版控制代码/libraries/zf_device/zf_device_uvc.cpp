#include "zf_device_uvc.h"
#include "zf_common_headfile.h"

using namespace cv;

cv::Mat frame_rgb;      // 构建opencv对象 彩色
cv::Mat frame_rgay;     // 构建opencv对象 灰度
cv::Mat cropped_binary;  // 储存切割后的二值化图像

uint8_t *rgay_image;    // 灰度图像数组指针
uint8_t *binary_image;   // 二值化图像数组指针

VideoCapture cap;

// // 帧率计算相关变量
// static int frameCount = 0;
// static double startTime = 0;
// static double lastFps = 0;

int8 uvc_camera_init(const char *path)
{
    cap.open(path);

    if(!cap.isOpened())
    {
        printf("find uvc camera error.\r\n");
        return -1;
    } 
    else 
    {
        printf("find uvc camera Successfully.\r\n");
    }


    cap.set(CAP_PROP_FRAME_WIDTH, UVC_WIDTH);     // 设置摄像头宽度
    cap.set(CAP_PROP_FRAME_HEIGHT, UVC_HEIGHT);    // 设置摄像头高度
    cap.set(CAP_PROP_FPS, UVC_FPS);              // 显示屏幕帧率

    return 0;
}

void transfer_imagegroup(void)
{
    for(int i = CROP_H-1, x=0; i >= 0; i--, x++)
    {
        const int reverse_j = CROP_W-1;
        for(int j = reverse_j; j >= 0; j--)
        {
            transfer[x][reverse_j - j] = Bmp[i][j];
        }
    }
}

int8 wait_image_refresh()
{
    try 
    {
        // // 开始计时
        // if (frameCount == 0) {
        //     startTime = static_cast<double>(cv::getTickCount());
        // }

        // 阻塞式等待图像刷新
        cap >> frame_rgb;
        // cap.read(frame_rgb);
        if (frame_rgb.empty()) 
        {
            std::cerr << "未获取到有效图像帧" << std::endl;
            return -1;
        }
    } 
    catch (const cv::Exception& e) 
    {
        std::cerr << "OpenCV 异常: " << e.what() << std::endl;
        return -1;
    }
  //大津法二值化 
    // rgb转灰度
    cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

    // cv对象转指针
    rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
    pthread_mutex_lock(&mutex);    
    // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    crop_image(image, rgay, 80, 70, CROP_H, CROP_W);
    pthread_mutex_unlock(&mutex);
    // //均值滤波
    // cv::Mat filteredImage;
    // cv::medianBlur(frame_rgay, filteredImage, 3); // 5是卷积核大小，可根据实际调整

    const cv::Rect roi(70, 80, CROP_W, CROP_H);
    // cv::Mat gray_roi = filteredImage(roi);          // 直接截取灰度图ROI   
    cv::Mat gray_roi = frame_rgay(roi);

    
    //应用大津法二值化（自动计算最优阈值）
    ImageInfo.threshold = cv::threshold(gray_roi, cropped_binary, 
                                       0, 255, 
                                       cv::THRESH_BINARY | cv::THRESH_OTSU);
    // // 新增形态学操作（先腐蚀后膨胀）
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    cv::erode(cropped_binary, cropped_binary, kernel); // 仅腐蚀

    pthread_mutex_lock(&mutex);
    memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
    pthread_mutex_unlock(&mutex);
    return 0;
}

