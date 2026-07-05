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
bool detect_red_slow_down(const cv::Mat& bgr_image) {
    static cv::Mat hsv, mask, m1, m2;
    
    // 原图是倒立的（摄像头倒装），直接在原图上找红块
    const int img_w = bgr_image.cols;
    const int img_h = bgr_image.rows;
    int x_orig = img_w - 105 - 120;
    int y_orig = img_h - 90 - 140;
    cv::Rect search_area_orig(x_orig, y_orig, 120, 140);
    search_area_orig &= cv::Rect(0, 0, img_w, img_h);
    cv::Mat search_region = bgr_image(search_area_orig);
    
    // 在搜索区域内做HSV检测
    cv::cvtColor(search_region, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 70, 70), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 70, 70), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > 10)   // 极低门槛，远处红块也触发
            return true;
    }
    return false;
}

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
//调试ROI代码
// bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
//     if (!g_initialized || bgr_image.empty()) return false;

//     // 1. 立即翻转
//     cv::Mat flipped; 
//     cv::flip(bgr_image, flipped, -1); 

//     // 2. 定义搜索区域
//     // cv::Rect search_area(x,y,width,height)
//     cv::Rect search_area(105, 90, 120, 140);
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
//         if (++save_cnt >= 5) {
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
static bool detect_red_and_get_roi(const cv::Mat& bgr_image, cv::Rect& roi) {
    static cv::Mat hsv, mask, m1, m2;
    cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    double max_area = 0;
    int max_idx = -1;
    for (size_t i = 0; i < contours.size(); ++i) {
        double a = cv::contourArea(contours[i]);
        if (a > max_area) { max_area = a; max_idx = i; }
    }
    if (max_idx == -1) return false;
    // cv::Rect red_rect = cv::boundingRect(contours[max_idx]);

    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);


    // int area_min = ((red_rect.y + red_rect.height) > 80) ? 60 : 30;
    // if (max_area < area_min) return false;
    // 动态面积阈值：基线40，确保手雷和眼镜能正常触发
    int area_min = 40;
    
    // 针对急救包等大面积红块：如果红块面积已经很大(>150)，说明目标板已经很近，
    // 但为了确保绕行距离足够，要求红块底部必须足够低(>110)才触发推理。
    // 这样可以延迟急救包的识别，让车更靠近，右转更充分。
    //if (max_area > 150 && (red_rect.y + red_rect.height) < 110) {
        // 调试打印：每10帧输出一次，避免刷屏
     //   static int dbg_cnt = 0;
       // if (++dbg_cnt >= 10) {
      //      dbg_cnt = 0;
      //      printf("First aid delayed: area=%.0f, bottom=%d\n", max_area, red_rect.y + red_rect.height);
        //}
     //   return false;  // 大面积红块但底部不够低，放弃本次推理
  //  }
    
    if (max_area < area_min) return false;

    // // ===== 新增：红砖过滤 =====
    // if (max_area > 350) return false;   // 面积过大，视为红砖，直接退出

    // 扩展系数（倒立图专用：红块在上，目标板在下）
    const float DOWN_SCALE = 3.8f;   // 向下扩展（原向上）—— 包住目标板
    const float UP_SCALE   = 1.5f;   // 向上扩展（原向下）—— 包住红块自身
    const float SIDE_SCALE = 0.6f;   // 左右扩展

    int down_expand = static_cast<int>(red_rect.height * DOWN_SCALE);
    int up_expand   = static_cast<int>(red_rect.height * UP_SCALE);
    int side_expand = static_cast<int>(red_rect.width * SIDE_SCALE);

    int start_x = std::max(0, red_rect.x - side_expand);
    int start_y = std::max(0, red_rect.y - up_expand);          // 向上留一点
    int width  = red_rect.width + 2 * side_expand;
    int height = red_rect.height + up_expand + down_expand;     // 向下扩很多

    // 边界裁剪
    if (start_x + width > bgr_image.cols) width = bgr_image.cols - start_x;
    if (start_y + height > bgr_image.rows) height = bgr_image.rows - start_y;
    roi = cv::Rect(start_x, start_y, width, height);
    return (roi.width > 0 && roi.height > 0);
}
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty()) return false;

    // 原图是倒立的（摄像头倒装），直接在原图上找红块
    const int img_w = bgr_image.cols;
    const int img_h = bgr_image.rows;
    int x_orig = img_w - 105 - 120;
    int y_orig = img_h - 90 - 140;
    cv::Rect search_area_orig(x_orig, y_orig, 120, 140);
    search_area_orig &= cv::Rect(0, 0, img_w, img_h);
    cv::Mat search_region = bgr_image(search_area_orig);

    // HSV 红块检测
    cv::Rect red_roi_in_search;
    bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

    if (found_red) {
        // 坐标映射
        cv::Rect final_roi = red_roi_in_search;
        final_roi.x += search_area_orig.x;
        final_roi.y += search_area_orig.y;
        final_roi &= cv::Rect(0, 0, img_w, img_h);
        if (final_roi.width <= 0 || final_roi.height <= 0) return false;

        // 裁剪 + 翻转
        cv::Mat cropped_inverted = bgr_image(final_roi);
        cv::Mat cropped;
        cv::flip(cropped_inverted, cropped, -1);

        // NCNN 推理
        bool success = g_ncnn.InferWithConfidence(cropped, class_id, confidence);

      // 每 10 帧成功推理保存一张截图（正立图）
        // static int frame_count = 0;
        // static int save_index = 1;
        // frame_count++;
        // if (frame_count >= 10) {
        //     frame_count = 0;
        //     char filename[64];
        //     sprintf(filename, "infer_%04d.jpg", save_index);
        //     cv::imwrite(filename, cropped);
        //     printf("Saved inference snapshot: %s\n", filename);
        //     save_index++;
        // }

        return success;
    }
    return false;   // 没找到红块
}