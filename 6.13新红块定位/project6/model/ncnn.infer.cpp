#include "ncnn_infer.h"
#include "ncnn.h" // 包含你的 LQ_NCNN 类

static LQ_NCNN g_ncnn;
static bool g_initialized = false;

// 类别标签
const std::vector<std::string> class_labels = {
    "0_gun", "0_shoulei", "1_first_aid_kit", "1_yanjing",
    "2_ambulance"
};
// 用于减速检测的无参版本：只判断赛道内是否存在红块（黑色区域）
bool detect_red_slow_down(const cv::Mat& bgr_image) {
    static cv::Mat hsv, mask, m1, m2;
    cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++) {
        if (cv::contourArea(contours[i]) > 15)   // 极低门槛，远处红块也触发
            return true;
    }
    return false;
}
// static bool detect_red_and_get_roi(const cv::Mat& bgr_image, cv::Rect& roi) {
//     static cv::Mat hsv, mask, m1, m2;
//     cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
//     cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
//     cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
//     mask = m1 | m2;

//     // === 1. 找最下方有效红色块 ===
//     int bottom_y = -1, top_y = -1;
//     int left_x = 9999, right_x = -1;

//     for (int y = mask.rows - 1; y >= 0; --y) {
//         const uchar* row = mask.ptr<uchar>(y);
//         int l = -1, r = -1;
//         for (int x = 0; x < mask.cols; ++x) {
//             if (row[x]) {
//                 if (l < 0) l = x;
//                 r = x;
//             }
//         }
//         if (l < 0) continue;

//         int continuous = 1;
//         for (int yy = y - 1; yy >= std::max(0, y - 2); --yy) {
//             const uchar* rr = mask.ptr<uchar>(yy);
//             bool ok = false;
//             for (int x = l; x <= r; ++x) {
//                 if (rr[x]) { ok = true; break; }
//             }
//             if (ok) ++continuous;
//         }
//         if (continuous < 2) continue;

//         bottom_y = y;
//         left_x = l;
//         right_x = r;
//         top_y = y;
//         int prev_width = r - l + 1;

//         for (int yy = y - 1; yy >= 0; --yy) {
//             const uchar* rr = mask.ptr<uchar>(yy);
//             int ll = -1, rr_ = -1;
//             for (int x = 0; x < mask.cols; ++x) {
//                 if (rr[x]) {
//                     if (ll < 0) ll = x;
//                     rr_ = x;
//                 }
//             }
//             if (ll < 0) break;
//             int width = rr_ - ll + 1;
//             if (width > prev_width * 5.0 && width > 20) break;
//             left_x = std::min(left_x, ll);
//             right_x = std::max(right_x, rr_);
//             top_y = yy;
//             prev_width = width;
//         }
//         break;
//     }

//     if (bottom_y < 0 || top_y < 0) return false;

//     int red_h = bottom_y - top_y + 1;
//     int red_w = right_x - left_x + 1;
//     if (red_h < 5 || red_h > 60) return false;
//     if (red_w < 2 || red_w > 40) return false;

//     // === 2. 触发条件：只要红块连续就触发，不限距离 ===
//     if (red_h < 6 && red_w < 4) return false;

//     // === 3. 动态扩展：底部越低（越近），向上扩越多 ===
//     int cx = (left_x + right_x) / 2;
//     int by = bottom_y;

//     // 核心：根据红块底部位置计算所需扩展
//     // 底部低（近）：需要多扩，因为目标板大
//     // 底部高（远）：少扩，因为目标板小
//     int up_expand;
//    if (by >= 120) {
//     up_expand = 75;   // 原来 90，砍 15
//    } else if (by >= 100) {
//     up_expand = 60;   // 原来 75，砍 15
//    } else if (by >= 80) {
//     up_expand = 50;   // 原来 60，砍 10
//   } else {
//     up_expand = 40;   // 原来 50，砍 10
//    }
//     const int SIDE_EXPAND = 28;
//     const int DOWN_MARGIN = 5;

//     int x = std::max(0, cx - SIDE_EXPAND);
//     int y = std::max(0, by - up_expand);
//     int w = std::min(SIDE_EXPAND * 2, bgr_image.cols - x);
//     int h = std::min(up_expand + DOWN_MARGIN, bgr_image.rows - y);

//     // 边界保护：如果 y 被 clamp 到 0，确保 h 足够大
//     if (y == 0) {
//         h = std::min(by + DOWN_MARGIN, bgr_image.rows);
//     }

//     roi = cv::Rect(x, y, w, h);

//     bool contains_red = (x <= left_x && y <= top_y &&
//                           x + w >= right_x && y + h >= bottom_y);
    
//     // 最终校验：ROI 高度必须足够包到目标板顶部
//     // 近距离 ROI 高度至少 80，中距离 60，远距离 50
//     int min_h = (by >= 120) ? 80 : (by >= 100) ? 65 : 50;
    
//     return contains_red && h >= min_h && h <= 130;
// }
static bool detect_red_and_get_roi(const cv::Mat& bgr_image, cv::Rect& roi) {
    static cv::Mat hsv, mask, m1, m2;
    cv::cvtColor(bgr_image, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, cv::Scalar(0, 80, 80), cv::Scalar(10, 255, 255), m1);
    cv::inRange(hsv, cv::Scalar(160, 80, 80), cv::Scalar(180, 255, 255), m2);
    mask = m1 | m2;
    static cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

    // === 1. 找最下方有效红色块 ===
    int bottom_y = -1, top_y = -1;
    int left_x = 9999, right_x = -1;

    for (int y = mask.rows - 1; y >= 0; --y) {
        const uchar* row = mask.ptr<uchar>(y);
        int l = -1, r = -1;
        for (int x = 0; x < mask.cols; ++x) {
            if (row[x]) { if (l < 0) l = x; r = x; }
        }
        if (l < 0) continue;

        // 远距离放宽连续性
        int continuous = 1;
        for (int yy = y - 1; yy >= std::max(0, y - 3); --yy) {
            const uchar* rr = mask.ptr<uchar>(yy);
            bool ok = false;
            int overlap_l = std::max(l - 5, 0);
            int overlap_r = std::min(r + 5, mask.cols - 1);
            for (int x = overlap_l; x <= overlap_r; ++x) {
                if (rr[x]) { ok = true; break; }
            }
            if (ok) ++continuous;
        }
        int min_continuous = (y < 50) ? 1 : (y < 90) ? 2 : 3;
        if (continuous < min_continuous) continue;

        bottom_y = y; left_x = l; right_x = r; top_y = y;
        int prev_width = r - l + 1;

        // 向上爬，修复倾斜和断裂
        for (int yy = y - 1; yy >= 0; --yy) {
            const uchar* rr = mask.ptr<uchar>(yy);
            int ll = -1, rr_ = -1;
            for (int x = 0; x < mask.cols; ++x) {
                if (rr[x]) { if (ll < 0) ll = x; rr_ = x; }
            }
            if (ll < 0) break;
            int width = rr_ - ll + 1;

            float width_ratio = (y < 60) ? 8.0f : (y < 100) ? 6.0f : 5.0f;
            int min_width_for_break = (y < 60) ? 10 : 20;
            if (width > prev_width * width_ratio && width > min_width_for_break) break;

            bool has_overlap = !(rr_ < left_x || ll > right_x);
            if (!has_overlap && y >= 80) break;

            left_x = std::min(left_x, ll);
            right_x = std::max(right_x, rr_);
            top_y = yy;
            prev_width = width;
        }
        break;
    }

    if (bottom_y < 0 || top_y < 0) return false;

    int red_h = bottom_y - top_y + 1;
    int red_w = right_x - left_x + 1;

    // 分层尺寸过滤（微调远距离下限）
    if (bottom_y < 50) {
        if (red_h < 3 || red_w < 2) return false;   // 原来 red_h < 2
    } else if (bottom_y < 80) {
        if (red_h < 3 || red_w < 2) return false;
    } else if (bottom_y < 110) {
        if (red_h < 4 || red_h > 80) return false;
        if (red_w < 2 || red_w > 50) return false;
    } else {
        if (red_h < 5 || red_h > 60) return false;
        if (red_w < 2 || red_w > 40) return false;
        if (red_h < 6 && red_w < 4) return false;
    }

    // === 3. 动态扩展：比例 + 保底像素 ===
    int cx = (left_x + right_x) / 2;
    int by = bottom_y;

    float up_scale, side_scale, down_scale;
    int up_min, side_min, down_min;

    if (by >= 130) {
        up_scale = 2.5f; side_scale = 1.0f; down_scale = 0.5f;
        up_min = 60; side_min = 20; down_min = 5;
    } else if (by >= 110) {
        up_scale = 3.0f; side_scale = 1.2f; down_scale = 0.8f;
        up_min = 55; side_min = 22; down_min = 5;
    } else if (by >= 90) {
        up_scale = 3.5f; side_scale = 1.5f; down_scale = 1.0f;
        up_min = 50; side_min = 25; down_min = 8;
    } else if (by >= 70) {
        up_scale = 4.0f; side_scale = 2.0f; down_scale = 1.2f;
        up_min = 45; side_min = 28; down_min = 10;
    } else if (by >= 50) {
        up_scale = 4.5f; side_scale = 2.5f; down_scale = 1.5f;
        up_min = 40; side_min = 30; down_min = 12;
    } else {
        up_scale = 5.0f; side_scale = 3.0f; down_scale = 2.0f;
        up_min = 35; side_min = 32; down_min = 15;
    }

    int up_expand = std::max(static_cast<int>(red_h * up_scale), up_min);
    int side_expand = std::max(static_cast<int>(red_w * side_scale), side_min);
    int down_expand = std::max(static_cast<int>(red_h * down_scale), down_min);

    int x = std::max(0, cx - side_expand);
    int y = std::max(0, by - up_expand);
    int w = std::min(cx + side_expand, bgr_image.cols) - x;
    int h = std::min(by + down_expand, bgr_image.rows) - y;

    if (y == 0) {
        h = std::min(by + down_expand, bgr_image.rows);
    }

    roi = cv::Rect(x, y, w, h);

    bool contains_red = (x <= left_x && y <= top_y &&
                         x + w >= right_x && y + h >= bottom_y);

    // 分层 min_h（极远端放宽到 20）
    int min_h;
    if (by >= 130) min_h = 75;
    else if (by >= 110) min_h = 65;
    else if (by >= 90) min_h = 55;
    else if (by >= 70) min_h = 45;
    else if (by >= 50) min_h = 35;
    else min_h = 20;   // 原来 25

    return contains_red && h >= min_h && h <= 150;
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
//     cv::Rect search_area(105, 90, 140, 140);
//     search_area &= cv::Rect(0, 0, flipped.cols, flipped.rows); 
//     cv::Mat search_region = flipped(search_area);

//     // 3. 在搜索区域内找红块
//     cv::Rect red_roi_in_search;
//     bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

//     if (found_red) {
//         // 4. 坐标转换
//         cv::Rect final_roi = red_roi_in_search;
//         final_roi.x += search_area.x;
//         final_roi.y += search_area.y;
//         final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
        
//         if (final_roi.width <= 0 || final_roi.height <= 0) {
//             return false;
//         }
        
//         cv::Mat cropped = flipped(final_roi);

//         // --- 存图调试逻辑：每推理 5 帧存一张，不覆盖 ---
//         static int frame_counter = 0;      // 推理帧计数
//         static int save_index = 0;         // 保存序号，不覆盖
        
//         if (++frame_counter >= 5) {
//             frame_counter = 0;
//             save_index++;
            
//             // 保存 ROI 原图（模型输入）
//             char roi_path[64];
//             sprintf(roi_path, "/tmp/roi_%03d.jpg", save_index);
//             cv::imwrite(roi_path, cropped);
            
//             // 保存带框的调试图（在原图上画框）
//             cv::Mat debug_img = flipped.clone();
//             cv::rectangle(debug_img, search_area, cv::Scalar(0, 255, 0), 1);  // 绿色搜索区
//             cv::rectangle(debug_img, final_roi, cv::Scalar(0, 0, 255), 2);      // 红色 ROI
            
//             char debug_path[64];
//             sprintf(debug_path, "/tmp/debug_%03d.jpg", save_index);
//             cv::imwrite(debug_path, debug_img);
            
//             printf("[Save#%d] ROI=%dx%d at (%d,%d) | files: roi_%03d.jpg debug_%03d.jpg\n",
//                    save_index, final_roi.width, final_roi.height,
//                    final_roi.x, final_roi.y, save_index, save_index);
//         }

//         // 送去推理
//         return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
//     } 

//     return false;
// }
// //调试1 ：26.5.13： 38cm识别
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty()) return false;

    // 1. 立即翻转！摄像头是倒装的，所以第一时间把图像转正
    cv::Mat flipped; 
    cv::flip(bgr_image, flipped, -1); 

    // 2. 在“正”的画面里定义搜索区域
    cv::Rect search_area(95, 80, 150, 140);
    search_area &= cv::Rect(0, 0, flipped.cols, flipped.rows); 
    cv::Mat search_region = flipped(search_area);

    // 3. 在正向画面里找红块
    cv::Rect red_roi_in_search;
    bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

    // //序列化存图逻辑
    // static int photo_index = 1;       // 照片编号：1, 2, 3...
    // static bool has_saved_this_block = false; // 存图锁：防止对同一个红块狂刷照片
    // static int red_lost_count = 0;    // 丢失计数：用于判定是否已经开过红块
    // 持久化图片序号：从文件读取，避免覆盖
    // static int photo_index = -1;
    // static bool has_saved_this_block = false;
    // static int red_lost_count = 0;

    // 首次调用时，从文件读取上次的序号
    // if (photo_index < 0) {
    //     FILE* fp = fopen("photo_index.txt", "r");
    //     if (fp) {
    //         fscanf(fp, "%d", &photo_index);
    //         fclose(fp);
    //     } else {
    //         photo_index = 1;  // 文件不存在，从1开始
    //     }
    // }

    if (found_red) {
    // 坐标转换：映射回 flipped 的坐标系
    cv::Rect final_roi = red_roi_in_search;
    final_roi.x += search_area.x;
    final_roi.y += search_area.y;
    // 【关键】先与图像边界求交集，并检查是否完全包含有效区域（避免边缘裁剪）
    // final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
    // if (final_roi.width <= 0 || final_roi.height <= 0) {
    //     return false;  // ROI 无效，跳过本帧
    // }
    cv::Mat cropped = flipped(final_roi);
    cv::Mat enhanced;
    cv::addWeighted(cropped, 1.2, sharp, -0.2, 0, enhanced);
    cropped = enhanced;  

//    if (!has_saved_this_block) {
//             char filename[64];
//             sprintf(filename, "block_%d.jpg", photo_index);
//             cv::imwrite(filename, cropped);
//             printf("[已保存截图: %s]\n", filename);
            
//             // 更新序号文件
//             FILE* fp = fopen("photo_index.txt", "w");
//             if (fp) {
//                 fprintf(fp, "%d", photo_index + 1);
//                 fclose(fp);
//             }
//             photo_index++;
//             has_saved_this_block = true;
//         }
//         red_lost_count = 0;
        return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
    } 
    // else {
    //     // 连续 20 帧完全没看到红色，认为已经开过这个红块了，解锁下一次保存
    //     if (++red_lost_count > 30) {
    //         has_saved_this_block = false; 
    //     }
    // }

    return false;
}

int Get_AIid(const cv::Mat& frame_rgb) {
    static int stable_cnt = 0;
    static int last_id = -1;
    static float conf_peak = 0;
    static int peak_frame = 0;

    int id = -1;
    float conf = 0.0f;

    if (!ncnn_process_frame(frame_rgb, id, conf)) {
        // 推理失败，检查丢帧兜底
        if (stable_cnt >= 2 && conf_peak > 0.85f) {
            int final_id = last_id;
            printf(">>> 锁定目标(丢帧兜底): %s (peak=%.2f) <<<\n", 
                   class_labels[final_id].c_str(), conf_peak);
            // 重置状态
            stable_cnt = 0;
            last_id = -1;
            conf_peak = 0;
            peak_frame = 0;
            return final_id;
        }
        stable_cnt = 0;
        last_id = -1;
        conf_peak = 0;
        peak_frame = 0;
        return -1;
    }

    // 推理成功，打印当前帧信息
  //  printf("[AI] id=%d(%s) conf=%.2f\n", id, class_labels[id].c_str(), conf);
    // 更新稳定计数
    if (id == last_id) {
        stable_cnt++;
        if (conf > conf_peak) {
            conf_peak = conf;
            peak_frame = stable_cnt;
        }
    } else {
        // 类别变了，检查上一类别是否满足换类别兜底
        if (stable_cnt >= 2 && conf_peak > 0.88f) {
            int final_id = last_id;
            printf(">>> 锁定目标(换类别兜底): %s (peak=%.2f) <<<\n", 
                   class_labels[final_id].c_str(), conf_peak);
            // 重置状态
            stable_cnt = 0;
            last_id = -1;
            conf_peak = 0;
            peak_frame = 0;
            return final_id;
        } else {
            stable_cnt = 1;
            last_id = id;
            conf_peak = conf;
            peak_frame = 1;
        }
    }

    // 正常触发条件
    bool trigger = false;
    if (stable_cnt >= 2 && conf_peak >= 0.90f && conf >= 0.85f) {
        trigger = true;
    } else if (stable_cnt >= 3 && conf_peak >= 0.85f && conf >= 0.80f) {
        trigger = true;
    } else if (stable_cnt >= 1 && conf >= 0.95f) {
        trigger = true;
    }

    if (trigger) {
        int final_id = id;
        printf(">>> 锁定目标: %s (peak=%.2f @%d, last=%.2f, cnt=%d) <<<\n", 
               class_labels[final_id].c_str(), conf_peak, peak_frame, conf, stable_cnt);
        // 重置状态
        stable_cnt = 0;
        last_id = -1;
        conf_peak = 0;
        peak_frame = 0;
        return final_id;
    }

    return -1;   // 尚未锁定
}