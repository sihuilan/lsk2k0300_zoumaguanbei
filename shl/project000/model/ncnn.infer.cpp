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
    
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    
    // 快速形态学处理
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //寻找最大面积
    double max_area = 0;
    int max_idx = -1;
    for(int i = 0; i < contours.size(); i++)
    {
        double a = cv::contourArea(contours[i]);
        if(a > max_area) { max_area = a; max_idx = i; };
    }
    if (max_idx == -1 || max_area < 70) 

    return false; // 稍微降低面积门槛，防止远距离丢帧 150
//     int dynamic_area_min = 60; // 默认门槛
// if (red_rect.y < 50) {      // 如果在图像上方（很远的地方）
//     dynamic_area_min = 35;  // 远距离允许小面积，实现“远见”
// } else {
//     dynamic_area_min = 80;  // 近距离必须是大面积，防止干扰
// }

if (max_area < dynamic_area_min) return false;
    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);

    // 1. 宽度扩展：向左右各扩展红块宽度的 20%，防止目标板边缘被切掉
    int expand_w = static_cast<int>(red_rect.width * 0.30f);
    int start_x = std::max(0, red_rect.x - expand_w);
    int final_w = red_rect.width + 2 * expand_w;
    if (start_x + final_w > bgr_image.cols) final_w = bgr_image.cols - start_x;

    // 2. 高度扩展：将 3.5 降为 2.5，让目标板处于截取图的中心偏上位置
    int expand_h = static_cast<int>(red_rect.height * 2.4f); 
    int start_y = std::max(0, red_rect.y - expand_h);
    int final_h = (red_rect.y + red_rect.height) - start_y;
    
    roi = cv::Rect(start_x, start_y, final_w, final_h);
    
    return true;
}

bool ncnn_init(const std::string& param_path, const std::string& bin_path) {
    if (g_initialized) return true;
    
    g_ncnn.SetModelPath(param_path, bin_path);
    g_ncnn.SetInputSize(96, 96);
    
    // 关键：对应你之前的训练参数
    // float mean_vals[3] = {123.675f, 116.28f, 103.53f};
    // float norm_vals[3] = {1.0f/58.395f, 1.0f/57.12f, 1.0f/57.375f};
    float mean_vals[3] = {123.675f, 116.28f, 103.53f};
    float norm_vals[3] = {0.01712475f, 0.017507f, 0.01742919f};
    g_ncnn.SetNormalize(mean_vals, norm_vals);
    
    if (!g_ncnn.Init()) return false;
    g_initialized = true;
    return true;
}

// bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
//     if (!g_initialized || bgr_image.empty()) return false;

//     // 1. 立即翻转
//     cv::Mat flipped; 
//     cv::flip(bgr_image, flipped, -1); 

//     // 2. 定义搜索区域
//     // cv::Rect search_area(10, 90, 300, 140); 
//     cv::Rect search_area(80, 30, 160, 190);
//     search_area &= cv::Rect(0, 0, flipped.cols, flipped.rows); 
//     cv::Mat search_region = flipped(search_area);

//     // 3. 在搜索区域内找红块
//     cv::Rect red_roi_in_search;
//     bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

//     if (found_red) {
//         // 4. 坐标转换：将局部坐标 red_roi_in_search 转为全局坐标 final_roi
//         cv::Rect final_roi = red_roi_in_search;
//         final_roi.x += search_area.x;
//         final_roi.y += search_area.y;
//         final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
        
//         cv::Mat cropped = flipped(final_roi);

//         // --- 存图调试逻辑开始 ---
//         static int save_cnt = 0;
//         if (++save_cnt >= 10) {
//             save_cnt = 0;
//             // 使用 clone 防止在原图上画框影响后续可能的处理
//             cv::Mat debug_img = flipped.clone(); 
            
//             // 绘制搜索区域（绿色矩形）
//             cv::rectangle(debug_img, search_area, cv::Scalar(0, 255, 0), 2);
            
//             // 绘制目标红块（红色矩形） - 这里必须用 final_roi
//             cv::rectangle(debug_img, final_roi, cv::Scalar(0, 0, 255), 2);
            
//             cv::imwrite("/tmp/debug_roi.jpg", debug_img);
//             printf("Saved debug image: SearchArea at y=%d, Target at x=%d,y=%d\n", 
//                     search_area.y, final_roi.x, final_roi.y);
//         }


//         // 送去推理
//         return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
//     } 

    

//     return false;
// }
//调试1 ：26.5.13： 38cm识别
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty()) return false;

    // 1. 立即翻转！摄像头是倒装的，所以第一时间把图像转正
    cv::Mat flipped; 
    cv::flip(bgr_image, flipped, -1); 

    // 2. 在“正”的画面里定义搜索区域
    cv::Rect search_area(80, 30, 150, 170);
    search_area &= cv::Rect(0, 0, flipped.cols, flipped.rows); 
    cv::Mat search_region = flipped(search_area);

    // 3. 在正向画面里找红块
    cv::Rect red_roi_in_search;
    bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

    //序列化存图逻辑
    static int photo_index = 1;       // 照片编号：1, 2, 3...
    static bool has_saved_this_block = false; // 存图锁：防止对同一个红块狂刷照片
    static int red_lost_count = 0;    // 丢失计数：用于判定是否已经开过红块

    if (found_red) {
        // 坐标转换：映射回 flipped 的坐标系
        cv::Rect final_roi = red_roi_in_search;
        final_roi.x += search_area.x;
        final_roi.y += search_area.y;
        final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
        
        // 【关键】从翻转后的图像截取，保证存出来的图不再是倒着的
        cv::Mat cropped = flipped(final_roi);

        // 如果是新遇到的红块（还没存过图），则存图并锁定
        if (!has_saved_this_block && photo_index <= 50) {
            char filename[64];
            sprintf(filename, "block_%d.jpg", photo_index);
            cv::imwrite(filename, cropped); 
            
            printf("[已识别到红块\n", filename);
            
            photo_index++;            
            has_saved_this_block = true; // 锁定，直到这个红块离开视野
        }
        red_lost_count = 0; // 只要能看到红色，就重置消失计数

        // 送去推理
        return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
    } 
    else {
        // 连续 10 帧完全没看到红色，认为已经开过这个红块了，解锁下一次保存
        if (++red_lost_count > 10) {
            has_saved_this_block = false; 
        }
    }

    return false;
}