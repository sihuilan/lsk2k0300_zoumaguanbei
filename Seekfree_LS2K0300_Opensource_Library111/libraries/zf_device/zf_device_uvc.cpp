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
    // //祖传迭代阈值法
    // // rgb转灰度
    // cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);
    // // cv对象转指针
    // rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
    // pthread_mutex_lock(&mutex);    
    // // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    // memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    // crop_image(image, rgay, 40, 20, CROP_H, CROP_W);
    
    // pthread_mutex_unlock(&mutex);
    // // 计算图像阈值
    // const cv::Rect roi(10, 40, CROP_W, CROP_H);  // 声明为常量  //祖传切割
    // // const cv::Rect roi(25, 40, CROP_W, CROP_H);  // 声明为常量  //迷宫法切割
    // // //均值滤波
    // // cv::Mat filteredImage;
    // // cv::medianBlur(frame_rgay, filteredImage, 3); // 5是卷积核大小，可根据实际调整
    // // //直方图均衡化 
    // // cv::Mat enhancedImage;
    // // cv::equalizeHist(filteredImage, enhancedImage);



    // // cv::Mat gray_roi = filteredImage(roi);          // 直接截取灰度图ROI    
    // cv::Mat gray_roi = frame_rgay(roi);          // 直接截取灰度图ROI    
    // ImageInfo.threshold = Iterative_Threshold(gray_roi, 1);  // 仅对ROI区域计算阈值
        
    // // 直接处理ROI区域，避免全图二值化
    // cv::threshold(gray_roi, cropped_binary, ImageInfo.threshold, 255, cv::THRESH_BINARY);

    // pthread_mutex_lock(&mutex); 
    // // 直接使用Mat的data指针，省去中间变量
    // memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H); 
    // pthread_mutex_unlock(&mutex);
    // transfer_imagegroup();
    // return 0;
    

// //自适应二值化 
//     // rgb转灰度
//     cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

//     // cv对象转指针
//     rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
//     pthread_mutex_lock(&mutex);    
//     // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     crop_image(image, rgay, 30, 0, CROP_H, CROP_W);
//     pthread_mutex_unlock(&mutex);

//     // 计算图像阈值
//     const cv::Rect roi(0, 30, CROP_W, CROP_H);
//     cv::Mat gray_roi = frame_rgay(roi);
    
//     // 改用自适应二值化（高斯加权）
//     cv::adaptiveThreshold(gray_roi, cropped_binary, 255, 
//                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
//                          cv::THRESH_BINARY, 
//                          ADAPTIVE_BLOCK_SIZE,    // 建议设置为11-31的奇数
//                          ADAPTIVE_C);           // 建议设置3-10
//         // // 新增形态学操作（腐蚀）
//     cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
//     cv::erode(cropped_binary, cropped_binary, kernel); // 仅腐蚀
//     pthread_mutex_lock(&mutex);
//     memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
//     pthread_mutex_unlock(&mutex);
//     transfer_imagegroup();
//     return 0;

//大津法二值化 
    // rgb转灰度
    cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

    // cv对象转指针
    rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
    pthread_mutex_lock(&mutex);    
    // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    crop_image(image, rgay, 32, 0, CROP_H, CROP_W);
    pthread_mutex_unlock(&mutex);
    // //均值滤波
    // cv::Mat filteredImage;
    // cv::medianBlur(frame_rgay, filteredImage, 3); // 5是卷积核大小，可根据实际调整

    const cv::Rect roi(0, 32, CROP_W, CROP_H);
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
    // transfer_imagegroup();
    return 0;


//     // 大津法二值化强曝光情况下版本2（待测试）
// cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

// // 新增：光照自适应预处理
// cv::Mat preprocessed;
// double mean_brightness = cv::mean(frame_rgay)[0]; // 计算平均亮度
// double gamma = 1.0;

// // 根据光照条件动态调整预处理
// if (mean_brightness > 180) { // 强光场景
//     gamma = 3.0;  // 压缩高光
    
//     // 伽马校正
//     cv::Mat lut(1, 256, CV_8U);
//     uchar* p = lut.ptr();
//     for (int i = 0; i < 256; ++i) {
//         p[i] = cv::saturate_cast<uchar>(pow(i/255.0, 1.0/gamma) * 255.0);
//     }
//     cv::LUT(frame_rgay, lut, preprocessed);
// } else if (mean_brightness < 50) { // 弱光场景
//     // 直方图均衡化增强对比度
//     cv::equalizeHist(frame_rgay, preprocessed);
// } else { // 正常光照
//     // 仅使用中值滤波去噪
//     cv::medianBlur(frame_rgay, preprocessed, 3);
// }

// // 指针操作
// rgay_image = reinterpret_cast<uint8_t *>(preprocessed.ptr(0));
// pthread_mutex_lock(&mutex);    
// memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
// crop_image(image, rgay, 30, 0, CROP_H, CROP_W);
// pthread_mutex_unlock(&mutex);

// const cv::Rect roi(0, 30, CROP_W, CROP_H);
// cv::Mat gray_roi = preprocessed(roi);

// // 应用优化的大津法
// ImageInfo.threshold = cv::threshold(gray_roi, cropped_binary, 
//                                    0, 255, 
//                                    cv::THRESH_BINARY | cv::THRESH_OTSU);

// // 自适应形态学处理
// cv::Mat kernel;
// if (mean_brightness > 180) { // 强光场景
//     // 先膨胀连接断裂边缘
//     kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
//     cv::dilate(cropped_binary, cropped_binary, kernel);
    
//     // 后腐蚀恢复形状
//     kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(2,2));
//     cv::erode(cropped_binary, cropped_binary, kernel);
// } else {
//     // 常规处理：仅腐蚀去除小噪点
//     kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
//     cv::erode(cropped_binary, cropped_binary, kernel);
// }

// pthread_mutex_lock(&mutex);
// memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
// pthread_mutex_unlock(&mutex);
// // transfer_imagegroup();
// return 0;




// //迷宫法版 大津法二值化 
//     // rgb转灰度
//     cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

//     // cv对象转指针
//     rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
//     pthread_mutex_lock(&mutex);    
//     // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     crop_image(image, rgay, 40, 20, CROP_H, CROP_W);
//     pthread_mutex_unlock(&mutex);
//     const cv::Rect roi(18, 38, CROP_W, CROP_H);
//     cv::Mat gray_roi = frame_rgay(roi);
    
//     // 应用大津法二值化（自动计算最优阈值）
//     ImageInfo.threshold = cv::threshold(gray_roi, cropped_binary, 
//                                        0, 255, 
//                                        cv::THRESH_BINARY | cv::THRESH_OTSU);
    
//     pthread_mutex_lock(&mutex);
//     memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
//     pthread_mutex_unlock(&mutex);
//     transfer_imagegroup();
//     return 0;
// //自适应二值化 
//     // rgb转灰度
//     cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);

//     // cv对象转指针
//     rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
//     pthread_mutex_lock(&mutex);    
//     // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
//     crop_image(image, rgay, 40, 20, CROP_H, CROP_W);
//     pthread_mutex_unlock(&mutex);

//     // 计算图像阈值
//     const cv::Rect roi(18, 38, CROP_W, CROP_H);
//     cv::Mat gray_roi = frame_rgay(roi);
    
//     // 改用自适应二值化（高斯加权）
//     cv::adaptiveThreshold(gray_roi, cropped_binary, 255, 
//                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
//                          cv::THRESH_BINARY, 
//                          ADAPTIVE_BLOCK_SIZE,    // 建议设置为11-31的奇数
//                          ADAPTIVE_C);           // 建议设置3-10
    
//     pthread_mutex_lock(&mutex);
//     memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H);
//     pthread_mutex_unlock(&mutex);
//     transfer_imagegroup();
//     return 0;
    // //祖传迭代阈值法
    // // rgb转灰度
    // cv::cvtColor(frame_rgb, frame_rgay, cv::COLOR_BGR2GRAY);
    // // cv对象转指针
    // rgay_image = reinterpret_cast<uint8_t *>(frame_rgay.ptr(0));
    // pthread_mutex_lock(&mutex);    
    // // memcpy(rgay[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    // memcpy(image[0], rgay_image, UVC_WIDTH * UVC_HEIGHT); 
    // crop_image(image, rgay, 40, 20, CROP_H, CROP_W);
    // pthread_mutex_unlock(&mutex);
    // // 计算图像阈值
    // const cv::Rect roi(18, 38, CROP_W, CROP_H);  // 声明为常量
    // cv::Mat gray_roi = frame_rgay(roi);          // 直接截取灰度图ROI      
    // ImageInfo.threshold = Iterative_Threshold(gray_roi, 1);  // 仅对ROI区域计算阈值
        
    // // 直接处理ROI区域，避免全图二值化
    // cv::threshold(gray_roi, cropped_binary, ImageInfo.threshold, 255, cv::THRESH_BINARY);
        
    // pthread_mutex_lock(&mutex);
    // // 直接使用Mat的data指针，省去中间变量
    // memcpy(Bmp[0], cropped_binary.data, CROP_W * CROP_H); 
    // pthread_mutex_unlock(&mutex);
    //     transfer_imagegroup();
    // return 0;
}

