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


                // // 帧计数
                // frameCount++;
        
                // // 每100帧计算一次帧率
                // if (frameCount % 100 == 0) {
                //     double endTime = static_cast<double>(cv::getTickCount());
                //     double totalTime = (endTime - startTime) / cv::getTickFrequency();
                //     lastFps = static_cast<double>(frameCount) / totalTime;
                    
                //     printf("%f\n",lastFps);
                //     // // 输出帧率信息
                //     // std::cout << "帧率: " << lastFps << " FPS" << std::endl;
                    
                //     // 重置计时器
                //     frameCount = 0;
                // }
       
    } 
    catch (const cv::Exception& e) 
    {
        std::cerr << "OpenCV 异常: " << e.what() << std::endl;
        return -1;
    }
   
//   // rgb转灰度
//     cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

//     // cv对象转指针
//     rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
//     pthread_mutex_lock(&mutex);    
//     // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     crop_image(image, rgay, 32, 10, CROP_H, CROP_W);
//     pthread_mutex_unlock(&mutex);
//     // //均值滤波
//     // cv::Mat filteredImage;
//     // cv::medianBlur(frame_rgay, filteredImage, 3); // 5是卷积核大小，可根据实际调整

//     const cv::Rect roi(10, 32, CROP_W, CROP_H);
//     // cv::Mat gray_roi = filteredImage(roi);          // 直接截取灰度图ROI   
//     cv::Mat gray_roi = frame_rgay(roi);

    
//     //应用大津法二值化（自动计算最优阈值）
//     ImageInfo.threshold = cv::threshold(gray_roi, cropped_binary, 
//                                        0, 255, 
//                                        cv::THRESH_BINARY | cv::THRESH_OTSU);

//     // // 新增形态学操作（先腐蚀后膨胀）
//     cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
//     cv::erode(cropped_binary, cropped_binary, kernel); // 仅腐蚀


//     pthread_mutex_lock(&mutex);
//     memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
//     pthread_mutex_unlock(&mutex);
//     // transfer_imagegroup();
//     return 0;
     cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

    // 将全图数据存入 image 数组 (320x240)
    // 注意：确保 image 数组大小已定义为 [240][320]
    pthread_mutex_lock(&mutex);
    memcpy(image[0], frame_rgay.data, UVC_WIDTH * UVC_HEIGHT); 
    pthread_mutex_unlock(&mutex);

    // // 2. 局部巡线处理
    // // 原逻辑坐标：x=10, y=32, 宽度=CROP_W, 高度=CROP_H
    // const cv::Rect roi(90, 90, CROP_W, CROP_H);
    int start_x = 70; // 保证水平居中
    int start_y = 80;                // 关键：从 90 改为 40，大大提升前瞻距离
 
   const cv::Rect roi(start_x, start_y, CROP_W, CROP_H);
    
    // 确保 ROI 不会越界
    if((roi.x + roi.width) <= frame_rgay.cols && (roi.y + roi.height) <= frame_rgay.rows) {
        cv::Mat gray_roi = frame_rgay(roi);

        // 3. 原来的 crop_image 逻辑，将局部灰度存入 rgay[CROP_H][CROP_W]
        pthread_mutex_lock(&mutex);
        // 直接从 gray_roi 拷贝到 rgay 数组
        for (int i = 0; i < CROP_H; i++) {
            memcpy(rgay[i], gray_roi.ptr(i), CROP_W);
        }
        pthread_mutex_unlock(&mutex);
        // 4. 二值化处理
        // 应用大津法
        ImageInfo.threshold = cv::threshold(gray_roi, cropped_binary, 
                                           0, 255, 
                                           cv::THRESH_BINARY | cv::THRESH_OTSU);

        // 形态学腐蚀
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
        cv::erode(cropped_binary, cropped_binary, kernel);

        // 5. 将二值化结果拷贝到 Bmp 数组供巡线算法计算
        pthread_mutex_lock(&mutex);
  
memcpy((uint8 *)Bmp, cropped_binary.data, CROP_W * CROP_H);
pthread_mutex_unlock(&mutex);
        // memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
        // pthread_mutex_unlock(&mutex);
    }

    return 0;
}