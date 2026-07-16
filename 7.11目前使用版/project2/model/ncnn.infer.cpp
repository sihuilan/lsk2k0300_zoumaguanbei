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
// 统一红块检测：在搜索区域内检测，返回原图坐标的 ROI，并判断是否需要减速
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
if (max_idx == -1) {
    RoadFlag.BrickDetected = 0;
    return false;
}
 // 计算红块在搜索区域内的矩形
    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);
    float w = (float)red_rect.width;
    float h = (float)red_rect.height;
    float aspect = (w > h) ? (w / h) : (h / w);
    int brick_cx = red_rect.x + red_rect.width / 2;
    int brick_cy = red_rect.y + red_rect.height / 2;
    int brick_cx_orig = brick_cx + search_area_orig.x;
    int brick_cy_orig = brick_cy + search_area_orig.y;
    
    // 全局减速和面积下限
    need_slow_down = (max_area > 18);
    if (max_area < 43) {
        RoadFlag.BrickDetected = 0;
        return false;
    }
    
    // 红砖过滤（仅在非十字区域执行）
  if (RoadFlag.inside_CrossRound == 0) {
    cv::Rect red_orig(red_rect.x + search_area_orig.x,
                      red_rect.y + search_area_orig.y,
                      red_rect.width, red_rect.height);

    int y = min(max(brick_cy_orig, 0), ROW - 1);
    int mid_col = Row_Middle_line[y];

    // 中线无效时取附近平均
    if (mid_col <= 0 || mid_col >= img_w) {
        int sum = 0, cnt = 0;
        for (int dy = -3; dy <= 3; dy++) {
            int yy = y + dy;
            if (yy < 0 || yy >= ROW) continue;
            int m = Row_Middle_line[yy];
            if (m > 0 && m < img_w) { sum += m; cnt++; }
        }
        if (cnt > 0) mid_col = sum / cnt;
        else mid_col = img_w / 2;
    }

    int dist_to_mid = abs(brick_cx_orig - mid_col);

    // 获取该行左右边界，计算 edge_dist
    int left  = L_boundary[y];
    int right = R_boundary[y];
    if (left > right) std::swap(left, right);
    int edge_dist = min(abs(brick_cx_orig - left), abs(right - brick_cx_orig));

    // 中线是否在红块内
    bool mid_in_red = (mid_col >= red_orig.x && mid_col <= red_orig.x + red_orig.width);

    // printf("DEBUG: red_orig=(%d,%d,%d,%d), mid_col=%d, mid_in_red=%d, edge=%d, area=%.0f, aspect=%.2f\n",
        //    red_orig.x, red_orig.y, red_orig.width, red_orig.height,
        //    mid_col, mid_in_red, edge_dist, max_area, aspect);

    // ===== 红砖判定 =====
    bool is_brick = false;

    // 1. 如果中线在红块内，必为急救包（放行）
    if (mid_in_red) {
        is_brick = false;
        RoadFlag.BrickDetected = 0;
    }
    // 2. 大面积红砖（area > 800，aspect 在 1.2~1.3 之间）
    else if (max_area > 800 && aspect > 1.2f && aspect < 1.3f) {
        is_brick = true;
    }
    // 3. 细长红砖（aspect > 2.5，面积 > 30）
    else if (aspect > 2.5f && max_area > 30) {
        is_brick = true;
    }
    // 4. 中等面积红砖（area > 150，aspect > 2.0，贴边）
    else if (max_area > 150 && aspect > 2.0f && edge_dist < 30) {
        is_brick = true;
    }
    // 5. 贴边且面积较大的红砖（area > 100，aspect < 1.5，edge_dist < 25）
    else if (max_area > 100 && aspect < 1.5f && edge_dist < 25) {
        is_brick = true;
    }
    // 否则放行（弯道中偏离中线的急救包）
    else {
        is_brick = false;
        RoadFlag.BrickDetected = 0;
    }

    if (is_brick) {
        RoadFlag.BrickDetected = 1;
        RoadFlag.BrickOffset = (brick_cx_orig > mid_col) ? 25 : -25;
        printf("RedBlock:\n");
        return false;
    }
}

// ========== 动态扩展 ROI（替代固定窗口） ==========
// 获取红块在搜索区域内的矩形（已计算）

    // 使用固定扩展系数
    const float DOWN_SCALE = 2.4f;
    const float UP_SCALE = 1.3f;
    const float SIDE_SCALE = 0.9f;

    int down_expand = (int)(red_rect.height * DOWN_SCALE);
    int up_expand   = (int)(red_rect.height * UP_SCALE);
    int side_expand = (int)(red_rect.width * SIDE_SCALE);

    int start_x = red_rect.x - side_expand;
    int start_y = red_rect.y - up_expand;
    int width  = red_rect.width + 2 * side_expand;
    int height = red_rect.height + up_expand + down_expand;

    // 边界裁剪（search_region内）
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (start_x + width > search_region.cols) width = search_region.cols - start_x;
    if (start_y + height > search_region.rows) height = search_region.rows - start_y;

    cv::Rect roi_in_search(start_x, start_y, width, height);
    if (roi_in_search.width < 30 || roi_in_search.height < 30) return false;

    // 映射到原图
    roi = roi_in_search;
    roi.x += search_area_orig.x;
    roi.y += search_area_orig.y;
    roi &= cv::Rect(0, 0, img_w, img_h);
    
    return true;
}
// static bool detect_red_and_get_roi(const cv::Mat& bgr_image, cv::Rect& roi) {
//     static cv::Mat hsv, mask, m1, m2;
//     cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
//     cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
//     cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
//     mask = m1 | m2;
//     static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
//     cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

//     std::vector<std::vector<cv::Point>> contours;
//     cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//     double max_area = 0;
//     int max_idx = -1;
//     for (size_t i = 0; i < contours.size(); ++i) {
//         double a = cv::contourArea(contours[i]);
//         if (a > max_area) { max_area = a; max_idx = i; }
//     }
//     if (max_idx == -1) return false;
//     cv::Rect red_rect = cv::boundingRect(contours[max_idx]);

//     // 动态面积阈值：基线40，确保手雷和眼镜能正常触发
//     // if (max_area > 350) return false;   // 面积过大，视为红砖，直接退出
//     int area_min = 43;
    
// //     // if (max_area > 150 && (red_rect.y + red_rect.height) < 110) {
// // //     // 调试打印：每10帧输出一次，避免刷屏
// //        static int dbg_cnt = 0;
// //        if (++dbg_cnt >= 2) {
// // //         //    dbg_cnt = 0;
// //            printf("First aid delayed: area=%.0f", max_area);
// //         }
// //     //    return false;  // 大面积红块但底部不够低，放弃本次推理
// // //    }
    
//     if (max_area < area_min) return false;

//     // 扩展系数（倒立图专用：红块在上，目标板在下）
//     const float DOWN_SCALE = 2.3f;   // 向下扩展减少
//     const float UP_SCALE   = 1.3f;   // 向上也略减
//     const float SIDE_SCALE = 0.8f;   // 左右略收窄

//     int down_expand = static_cast<int>(red_rect.height * DOWN_SCALE);
//     int up_expand   = static_cast<int>(red_rect.height * UP_SCALE);
//     int side_expand = static_cast<int>(red_rect.width * SIDE_SCALE); 

//     int start_x = std::max(0, red_rect.x - side_expand);
//     int start_y = std::max(0, red_rect.y - up_expand);          // 向上留一点
//     int width  = red_rect.width + 2 * side_expand;
//     int height = red_rect.height + up_expand + down_expand;     // 向下扩很多

//     // 边界裁剪
//     if (start_x + width > bgr_image.cols) width = bgr_image.cols - start_x;
//     if (start_y + height > bgr_image.rows) height = bgr_image.rows - start_y;
//     roi = cv::Rect(start_x, start_y, width, height);
//     return (roi.width > 0 && roi.height > 0);
// }
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

// bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
//     if (!g_initialized || bgr_image.empty()) return false;

//     // 原图是倒立的（摄像头倒装），直接在原图上找红块
//     const int img_w = bgr_image.cols;
//     const int img_h = bgr_image.rows;
//     int roi_width = 120;
//     int roi_height = 200;
//     int x_orig = (img_w - roi_width) / 2 ; // 居中
//     int y_orig = 0; // 从顶部开始
//     cv::Rect search_area_orig(x_orig, y_orig, roi_width, roi_height);
//     search_area_orig &= cv::Rect(0, 0, img_w, img_h-10);
//     cv::Mat search_region = bgr_image(search_area_orig);

//     // HSV 红块检测
//     cv::Rect red_roi_in_search;
//     bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);
//     if (found_red) {

//         // 坐标映射
//         cv::Rect final_roi = red_roi_in_search;
//         final_roi.x += search_area_orig.x;
//         final_roi.y += search_area_orig.y;
//         final_roi &= cv::Rect(0, 0, img_w, img_h);
//         if (final_roi.width <= 0 || final_roi.height <= 0) return false;
//         // // ===== 添加可视化绘制 =====
//         // cv::Mat display_img = bgr_image.clone();  // 复制一份避免影响原图
        
//         // // 用绿色框绘制搜索区域
//         // cv::rectangle(display_img, search_area_orig, cv::Scalar(0, 255, 0), 2);
//         // // 用红色框绘制最终ROI
//         // cv::rectangle(display_img, final_roi, cv::Scalar(0, 0, 255), 2);
        
//         // // 可选：显示图像（调试时）
//         // // cv::imshow("ROI Visualization", display_img);
//         // // cv::waitKey(1);
        
//         // // 可选：保存图片到文件（每10帧保存一张，类似您原有逻辑）
//         // static int frame_cnt = 0;
//         // static int save_idx = 1;
//         // frame_cnt++;
//         // if (frame_cnt >= 10) {
//         //     frame_cnt = 0;
//         //     char filename[64];
//         //     sprintf(filename, "roi_vis_%04d.jpg", save_idx++);
//         //     cv::imwrite(filename, display_img);
//         //     printf("Saved ROI visualization: %s\n", filename);
//         // }

//         // 裁剪 + 翻转
//         cv::Mat cropped_inverted = bgr_image(final_roi);
//         cv::Mat cropped;
//         cv::flip(cropped_inverted, cropped, -1);

//         // NCNN 推理
//         bool success = g_ncnn.InferWithConfidence(cropped, class_id, confidence);

    // //   每 10 帧成功推理保存一张截图（正立图）
    //     static int frame_count = 0;
    //     static int save_index = 1;
    //     frame_count++;
    //     if (frame_count >= 10) {
    //         frame_count = 0;
    //         char filename[64];
    //         sprintf(filename, "infer_%04d.jpg", save_index);
    //         cv::imwrite(filename, cropped);
    //         printf("Saved inference snapshot: %s\n", filename);
    //         save_index++;
    //     }

//         return success;
//     }
//     return false;   // 没找到红块
// }
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