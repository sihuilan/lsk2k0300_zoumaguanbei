#include "ncnn_infer.h"
#include "ncnn.h" // 包含你的 LQ_NCNN 类

static LQ_NCNN g_ncnn;
static bool g_initialized = false;
int g_red_area = 0;
int g_red_bottom = 0;

// 类别标签
const std::vector<std::string> class_labels = {
    "0_gun", "0_shoulei", "1_first_aid_kit", "1_yanjing",
    "2_ambulance"
};

bool ncnn_init(const std::string& param_path, const std::string& bin_path) {
    if (g_initialized) return true;
    
    g_ncnn.SetModelPath(param_path, bin_path);
    g_ncnn.SetInputSize(68, 68);
    
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
bool detect_red_unified(const cv::Mat& bgr_image, cv::Rect& roi, bool& need_slow_down) {
    static cv::Mat hsv, mask, m1, m2;
    
    const int img_w = bgr_image.cols;
    const int img_h = bgr_image.rows;
    int roi_width = 140;
    int roi_height = 160;
    int x_orig = (img_w - roi_width) / 2;
    int y_orig = 20;
    cv::Rect search_area_orig(x_orig, y_orig, roi_width, roi_height);
    search_area_orig &= cv::Rect(0, 0, img_w, img_h - 20);
    
    cv::Mat search_region = bgr_image(search_area_orig);
    
    cv::cvtColor(search_region, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    double max_area = 0;
    int max_idx = -1;
    for (size_t i = 0; i < contours.size(); ++i) {
        double a = cv::contourArea(contours[i]);
        if (a > max_area) { max_area = a; max_idx = i; }
    }
    
    need_slow_down = false;
    if (max_idx == -1)
    {
        RoadFlag.Red = 0;
        return false; 
   }
   
    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);
    
    if (max_area < 43) 
    {
        RoadFlag.Red = 0;
        return false;
    }
  
    // ========== 固定扩展 ROI ==========
    const float DOWN_SCALE = 2.4f;
    const float UP_SCALE   = 1.3f;
    const float SIDE_SCALE = 0.9f;
    
    int down_expand = (int)(red_rect.height * DOWN_SCALE);
    int up_expand   = (int)(red_rect.height * UP_SCALE);
    int side_expand = (int)(red_rect.width * SIDE_SCALE);
    
    int start_x = red_rect.x - side_expand;
    int start_y = red_rect.y - up_expand;
    int width  = red_rect.width + 2 * side_expand;
    int height = red_rect.height + up_expand + down_expand;
    
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (start_x + width > search_region.cols) width = search_region.cols - start_x;
    if (start_y + height > search_region.rows) height = search_region.rows - start_y;
    
    cv::Rect roi_in_search(start_x, start_y, width, height);
    if (roi_in_search.width < 30 || roi_in_search.height < 30) return false;
    
    roi = roi_in_search;
    roi.x += search_area_orig.x;
    roi.y += search_area_orig.y;
    roi &= cv::Rect(0, 0, img_w, img_h);
    
    if (roi.width <= 0 || roi.height <= 0) return false;
    
    return true;
}
bool ncnn_infer_from_roi(const cv::Mat& bgr_image, const cv::Rect& roi, 
                         int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty() || roi.width <= 0 || roi.height <= 0)
        return false;

    // ========== 调试：在原图上画框 ==========
    // static int save_cnt = 0;
    // if (++save_cnt >= 5) {
    //     save_cnt = 0;
    //     cv::Mat debug_img = bgr_image.clone();
        
    //     // 画 ROI 框（红色）
    //     cv::rectangle(debug_img, roi, cv::Scalar(0, 0, 255), 2);
        
    //     // 如果知道搜索区域，也画出来（绿色）
    //     // 需要从外部传入，或者在这里重新定义
    //     int img_w = bgr_image.cols;
    //     int img_h = bgr_image.rows;
    //     int roi_width = 140;
    //     int roi_height = 160;
    //     int x_orig = (img_w - roi_width) / 2;
    //     int y_orig = 10;
    //     cv::Rect search_area(x_orig, y_orig, roi_width, roi_height);
    //     search_area &= cv::Rect(0, 0, img_w, img_h-20);
        
    
    //     cv::rectangle(debug_img, search_area, cv::Scalar(0, 255, 0), 2);
        
    //     cv::imwrite("/tmp/debug_roi.jpg", debug_img);
    //     printf("Saved debug_roi.jpg: roi=(%d,%d,%d,%d)\n", 
    //            roi.x, roi.y, roi.width, roi.height);
    // }

    // ========== 正常推理 ==========
    cv::Mat cropped_inverted = bgr_image(roi);
    cv::Mat cropped;
    cv::flip(cropped_inverted, cropped, -1);
    // //   每 10 帧成功推理保存一张截图（正立图）
    //     static int frame_count = 0;
    //     static int save_index = 1;
    //     frame_count++;
    //     if (frame_count >= 1) {
    //         frame_count = 0;
    //         char filename[64];
    //         sprintf(filename, "infer_%04d.jpg", save_index);
    //         cv::imwrite(filename, cropped);
    //         printf("Saved inference snapshot: %s\n", filename);
    //         save_index++;
    //     }

    return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
}