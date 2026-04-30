#include "ncnn_infer.h"
#include "ncnn.h" // 包含你的 LQ_NCNN 类

static LQ_NCNN g_ncnn;
static bool g_initialized = false;

// 类别标签
const std::vector<std::string> class_labels = {
    "0_gun", "0_shoulei", "1_first_aid_kit", "1_yanjing",
    "2_ambulance", "2_zhuangjiache"
};

// 内部函数：红色检测优化版
static bool detect_red_and_get_roi(const cv::Mat& bgr_image, cv::Rect& roi) {
    static cv::Mat hsv, mask, m1, m2; // 使用 static 减少内存反复申请
    cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
    
    cv::inRange(hsv, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 100, 100), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    
    // 快速形态学处理
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    if (contours.empty()) return false;
    
    // 找最大面积
    double max_area = 0;
    int max_idx = -1;
    for(int i = 0; i < contours.size(); i++) {
        double a = cv::contourArea(contours[i]);
        if(a > max_area) { max_area = a; max_idx = i; }
    }
    
    if (max_idx == -1 || max_area < 200) return false;
    
    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);
    // 向上扩展逻辑：根据实际目标板高度调整系数
    int expand_h = static_cast<int>(red_rect.height * 3.5f); 
    int start_y = std::max(0, red_rect.y - expand_h);
    roi = cv::Rect(red_rect.x, start_y, red_rect.width, (red_rect.y + red_rect.height) - start_y);
    
    return true;
}

bool ncnn_init(const std::string& param_path, const std::string& bin_path) {
    if (g_initialized) return true;
    
    g_ncnn.SetModelPath(param_path, bin_path);
    g_ncnn.SetInputSize(96, 96);
    
    // 关键：对应你之前的训练参数
    float mean_vals[3] = {123.675f, 116.28f, 103.53f};
    float norm_vals[3] = {1.0f/58.395f, 1.0f/57.12f, 1.0f/57.375f};
    g_ncnn.SetNormalize(mean_vals, norm_vals);
    
    if (!g_ncnn.Init()) return false;
    g_initialized = true;
    return true;
}

bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty()) return false;

   // 1. 自动处理翻转（如果安装是反的）
   // 如果摄像头是正的，直接用 bgr_image 即可，节省 flip 的时间
    cv::Mat flipped; cv::flip(bgr_image, flipped, -1); 
    
    cv::Rect roi;
    if (!detect_red_and_get_roi(bgr_image, roi)) return false;

    // 2. 截取并推理
    cv::Mat cropped = bgr_image(roi);
    static int save_count = 0;
 if(save_count < 5) {
    cv::imwrite("debug_roi.jpg", cropped);
    save_count++;
    printf("Debug: 已保存 ROI 图像到当前目录\n");
}
    return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
}