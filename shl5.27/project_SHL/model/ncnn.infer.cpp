// #include "ncnn_infer.h"
// #include "ncnn.h" 
// #include <atomic>
// #include <vector>
// #include <string>
// #include <algorithm>

// // Sequential image counter
// std::atomic<int> g_debug_img_counter(0);
// static LQ_NCNN g_ncnn;
// static bool g_initialized = false;

// // Global flag to ensure sequential ROI save per bypass event
// static bool g_roi_saved_for_this_turn = false;

// const std::vector<std::string> class_labels = {
//     "0_gun", "0_shoulei", "1_first_aid_kit", "1_yanjing",
//     "2_ambulance", "2_zhuangjiache"
// };

// // Extern mutex for Bmp access
// extern pthread_mutex_t mutex;
// bool is_red_present() {
//     extern uint8_t Bmp[CROP_H][CROP_W];
    
//     pthread_mutex_lock(&mutex);
//     cv::Mat bin(CROP_H, CROP_W, CV_8UC1, (void*)Bmp);
//     cv::Mat bin_upright;
//     cv::flip(bin, bin_upright, -1); // 物理正向：赛道是白(255)，红块和背景是黑(0)
//     pthread_mutex_unlock(&mutex);

//     // ----------------- ? 第一步：动态提取最大白色赛道 -----------------
//     std::vector<std::vector<cv::Point>> track_contours;
//     // 复制一份去抓赛道外轮廓
//     cv::Mat bin_for_track = bin_upright.clone();
//     cv::findContours(bin_for_track, track_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

//     if (track_contours.empty()) return false;

//     double max_track_area = 0;
//     int max_track_idx = -1;
//     for (size_t i = 0; i < track_contours.size(); ++i) {
//         double area = cv::contourArea(track_contours[i]);
//         if (area > max_track_area) {
//             max_track_area = area;
//             max_track_idx = i;
//         }
//     }

//     // 如果连赛道都找不着，直接退出
//     if (max_track_idx == -1 || max_track_area < 500) return false;

//     // ? 创建全黑掩膜，并把最大的白赛道内部填满白色（255）
//     cv::Mat track_mask = cv::Mat::zeros(bin_upright.size(), CV_8UC1);
//     cv::drawContours(track_mask, track_contours, max_track_idx, cv::Scalar(255), cv::FILLED);

//     // 物理隔离赛道外杂物
//     cv::Mat clean_track_only;
//     // 将原图和赛道掩膜求交集：赛道外的区域全部强制抹黑！
//     cv::bitwise_and(bin_upright, track_mask, clean_track_only);

//     // 只在赛道内部找黑孔洞 
//     std::vector<std::vector<cv::Point>> contours;
//     std::vector<cv::Vec4i> hierarchy;
//     // 使用 RETR_TREE 抠出洗净后的赛道内部的黑色红块
//     cv::findContours(clean_track_only, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    
//     if (!contours.empty() && RoadFlag.AI == 0 && RoadFlag.AIpass == 0) {
//         double max_area = 0;
//         int max_idx = -1;

//         for (size_t i = 0; i < contours.size(); ++i) {
//             // 必须是被包裹在白赛道内部的孔洞
//             if (hierarchy[i][3] == -1) continue; 

//             double area = cv::contourArea(contours[i]);
//             // 限制红块在图像中的合理像素大小
//             if (area > 20 && area < (CROP_W * CROP_H / 5)) {
//                 if (area > max_area) {
//                     max_area = area;
//                     max_idx = i;
//                 }
//             }
//         }

//         if (max_idx != -1) {
//             cv::Rect r_global = cv::boundingRect(contours[max_idx]);
            
//             // 宽高比精确过滤（12x5cm = 2.4），卡在 1.6 到 3.5 之间
//             float ratio = (float)r_global.width / r_global.height;
//             if (ratio < 1.6f || ratio > 3.6f) return false;

//             // 中心过滤，红块不可能贴在最左右两侧边框上
//             int cx = r_global.x + r_global.width / 2;
//             if (cx < (CROP_W * 0.12f) || cx > (CROP_W * 0.88f)) return false;

//             g_roi_saved_for_this_turn = false;

//             // 调试画框：存图看看过滤得有多干净
//             cv::Mat save_mat;
//             cv::cvtColor(clean_track_only, save_mat, cv::COLOR_GRAY2BGR); 
//             cv::rectangle(save_mat, r_global, cv::Scalar(0, 255, 0), 2); // 绿框圈出黑红块

//             char filename[64];
//             snprintf(filename, sizeof(filename), "/tmp/debug_%03d_preslow_binary.jpg", g_debug_img_counter.fetch_add(1));
//             cv::imwrite(filename, save_mat);
//            printf("红块保存至: %s\n", filename);
            
//             return true; 
//         }
//     }
//     return false; 
// }

// bool ncnn_init(const std::string& param_path, const std::string& bin_path) {
//     if (g_initialized) return true;
    
//     g_ncnn.SetModelPath(param_path, bin_path);
//     g_ncnn.SetInputSize(96, 96);
    
//     float mean_vals[3] = {123.675f, 116.28f, 103.53f};
//     float norm_vals[3] = {0.01712475f, 0.017507f, 0.01742919f};
//     g_ncnn.SetNormalize(mean_vals, norm_vals);
    
//     if (!g_ncnn.Init()) return false;
//     g_initialized = true;
//     return true;
// }

// bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
//     if (!g_initialized || bgr_image.empty()) return false;

//     extern uint8_t Bmp[CROP_H][CROP_W];
//     pthread_mutex_lock(&mutex);
//     cv::Mat bin(CROP_H, CROP_W, CV_8UC1, (void*)Bmp);
//     cv::Mat bin_upright;
//     cv::flip(bin, bin_upright, -1); 
//     pthread_mutex_unlock(&mutex);

//     std::vector<std::vector<cv::Point>> track_contours;
//     cv::Mat bin_for_track = bin_upright.clone();
//     cv::findContours(bin_for_track, track_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
//     if (track_contours.empty()) return false;

//     double max_track_area = 0;
//     int max_track_idx = -1;
//     for (size_t i = 0; i < track_contours.size(); ++i) {
//         double area = cv::contourArea(track_contours[i]);
//         if (area > max_track_area) {
//             max_track_area = area;
//             max_track_idx = i;
//         }
//     }
//     if (max_track_idx == -1 || max_track_area < 500) return false;

//     cv::Mat track_mask = cv::Mat::zeros(bin_upright.size(), CV_8UC1);
//     cv::drawContours(track_mask, track_contours, max_track_idx, cv::Scalar(255), cv::FILLED);

//     cv::Mat clean_track_only;
//     cv::bitwise_and(bin_upright, track_mask, clean_track_only);

//     // 树状抠孔洞
//     std::vector<std::vector<cv::Point>> contours;
//     std::vector<cv::Vec4i> hierarchy;
//     cv::findContours(clean_track_only, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
//     if (contours.empty()) return false;

//     double max_area = 0;
//     int max_idx = -1;
//     for (size_t i = 0; i < contours.size(); ++i) {
//         if (hierarchy[i][3] == -1) continue; 

//         double area = cv::contourArea(contours[i]);
//         if (area > 25 && area < (CROP_W * CROP_H / 5)) {
//             if (area > max_area) {
//                 max_area = area;
//                 max_idx = i;
//             }
//         }
//     }

//     if (max_idx == -1) return false;

//     // 完美的全局绝对重合坐标
//     cv::Rect red_box = cv::boundingRect(contours[max_idx]);
    
//     float ratio = (float)red_box.width / red_box.height;
//     if (ratio < 1.6f || ratio > 3.6f) return false;
    
//     int cx = red_box.x + red_box.width / 2;
//     if (cx < (CROP_W * 0.12) || cx > (CROP_W * 0.88)) return false;

//     int cy = red_box.y + red_box.height / 2;
    
//     // 向上前瞻拓展
//     float dynamic_coeff = 3.5f;
//     if (max_area < 80) dynamic_coeff = 4.5f; 
//     else if (max_area > 200) dynamic_coeff = 2.8f; 

//     int up_expand = (int)(red_box.height * dynamic_coeff);   
//     int side_expand = (int)(red_box.width * 0.55f);  
    
//     int x1 = std::max(0, cx - side_expand);
//     int y1 = std::max(0, cy - up_expand); 
//     int x2 = std::min(CROP_W - 1, cx + side_expand);
//     int y2 = std::min(CROP_H - 1, red_box.y + red_box.height); // 底边精准对齐
    
//     cv::Rect target_roi(x1, y1, x2 - x1, y2 - y1);
    
//     // (基于真实参数 start_x=70, start_y=80)
//     int start_x = 70;
//     int start_y = 80;
//     cv::Rect color_roi_rect(target_roi.x + start_x, target_roi.y + start_y, target_roi.width, target_roi.height);
    
//     color_roi_rect = color_roi_rect & cv::Rect(0, 0, bgr_image.cols, bgr_image.rows);
//     if (color_roi_rect.width <= 0 || color_roi_rect.height <= 0) return false;
    
//     cv::Mat target = bgr_image(color_roi_rect).clone();
//     cv::Mat resized;
//     cv::resize(target, resized, cv::Size(96, 96));

//     if (!g_roi_saved_for_this_turn && RoadFlag.AIpass == 0) {
//         g_roi_saved_for_this_turn = true; 

//         char filename[64];
//         snprintf(filename, sizeof(filename), "/tmp/debug_%03d_roi.jpg", g_debug_img_counter.fetch_add(1));
//         cv::imwrite(filename, target); 
//         printf("彩色推理图已保存: %s\n", filename);
//     }
    
//     return g_ncnn.InferWithConfidence(resized, class_id, confidence);
// }
#include "ncnn_infer.h"
#include "ncnn.h" 
#include <atomic>
#include <vector>
#include <string>
#include <algorithm>

// Sequential image counter
std::atomic<int> g_debug_img_counter(0);
static LQ_NCNN g_ncnn;
static bool g_initialized = false;

// Global flag to ensure sequential ROI save per bypass event
static bool g_roi_saved_for_this_turn = false;

const std::vector<std::string> class_labels = {
    "0_gun", "0_shoulei", "1_first_aid_kit", "1_yanjing",
    "2_ambulance", "2_zhuangjiache"
};

// Extern mutex for Bmp access
extern pthread_mutex_t mutex;
bool is_red_present(cv::Mat* debug_out_mat = nullptr) {
    // 哨兵状态拦截
    // if (RoadFlag.AIpass != 0 && RoadFlag.AIpass != 9) return false;
    if (RoadFlag.AI != 0) return false;

 extern uint8_t Bmp[CROP_H][CROP_W];
    
    pthread_mutex_lock(&mutex);
    cv::Mat bin(CROP_H, CROP_W, CV_8UC1, (void*)Bmp);
    cv::Mat bin_upright;
    cv::flip(bin, bin_upright, -1); // 物理正向：赛道是白(255)，红块和背景是黑(0)
    pthread_mutex_unlock(&mutex);

    // ----------------- ? 第一步：动态提取最大白色赛道 -----------------
    std::vector<std::vector<cv::Point>> track_contours;
    cv::Mat bin_for_track = bin_upright.clone();
    cv::findContours(bin_for_track, track_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (track_contours.empty()) return false;

    double max_track_area = 0;
    int max_track_idx = -1;
    for (size_t i = 0; i < track_contours.size(); ++i) {
        double area = cv::contourArea(track_contours[i]);
        if (area > max_track_area) {
            max_track_area = area;
            max_track_idx = i;
        }
    }

    if (max_track_idx == -1 || max_track_area < 500) return false;

    // 创建全黑掩膜，并把最大的白赛道内部填满白色（255）
    cv::Mat track_mask = cv::Mat::zeros(bin_upright.size(), CV_8UC1);
    cv::drawContours(track_mask, track_contours, max_track_idx, cv::Scalar(255), cv::FILLED);

    // ----------------- ? 第二步：物理隔离赛道外杂物 -----------------
    cv::Mat clean_track_only;
    cv::bitwise_and(bin_upright, track_mask, clean_track_only);

    // ----------------- ? 第三步：只在赛道内部找黑孔洞 -----------------
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(clean_track_only, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    
    // 本地静态存图锁：用于卡死疯狂刷图
    static bool has_saved_binary_this_turn = false;

    if (!contours.empty()) {
        double max_area = 0;
        int max_idx = -1;

        for (size_t i = 0; i < contours.size(); ++i) {
            if (hierarchy[i][3] == -1) continue; // 必须是被包裹在白赛道内部的孔洞

            double area = cv::contourArea(contours[i]);
            if (area > 20 && area < (CROP_W * CROP_H / 5)) {
                if (area > max_area) {
                    max_area = area;
                    max_idx = i;
                }
            }
        }
        if (max_idx != -1) {
            cv::Rect r_global = cv::boundingRect(contours[max_idx]);
            
            float ratio = (float)r_global.width / r_global.height;
            if (ratio < 1.6f || ratio > 3.6f) return false;

            int cx = r_global.x + r_global.width / 2;
            if (cx < (CROP_W * 0.12f) || cx > (CROP_W * 0.88f)) return false;

            // 重置推理端的存图标志
            g_roi_saved_for_this_turn = false;

            // ? 核心改动：如果这一轮已经存过图了，直接返回 true 触发减速，绝对不再重复写硬盘
            if (!has_saved_binary_this_turn) {
                has_saved_binary_this_turn = true; // 立即上锁

                // 调试画框并存图
                cv::Mat save_mat;
                cv::cvtColor(clean_track_only, save_mat, cv::COLOR_GRAY2BGR); 
                cv::rectangle(save_mat, r_global, cv::Scalar(0, 255, 0), 2); 

                char filename[64];
                snprintf(filename, sizeof(filename), "/tmp/debug_%03d_preslow_binary.jpg", g_debug_img_counter.fetch_add(1));
                cv::imwrite(filename, save_mat);
                printf("锁定新红块！: %s\n", filename);
            }
            
            return true; 
        }
    }


    if (RoadFlag.AI == 0) {
        has_saved_binary_this_turn = false;
    }

    return false; 
}
bool ncnn_init(const std::string& param_path, const std::string& bin_path) {
    if (g_initialized) return true;
    
    g_ncnn.SetModelPath(param_path, bin_path);
    g_ncnn.SetInputSize(96, 96);
    
    float mean_vals[3] = {123.675f, 116.28f, 103.53f};
    float norm_vals[3] = {0.01712475f, 0.017507f, 0.01742919f};
    g_ncnn.SetNormalize(mean_vals, norm_vals);
    
    if (!g_ncnn.Init()) return false;
    g_initialized = true;
    return true;
}

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
    if (max_idx == -1 || max_area < 150) return false; // 稍微降低面积门槛，防止远距离丢帧
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

    // //序列化存图逻辑
    // static int photo_index = 1;       // 照片编号：1, 2, 3...
    // static bool has_saved_this_block = false; // 存图锁：防止对同一个红块狂刷照片
    // static int red_lost_count = 0;    // 丢失计数：用于判定是否已经开过红块

    if (found_red) {
        // 坐标转换：映射回 flipped 的坐标系
        cv::Rect final_roi = red_roi_in_search;
        final_roi.x += search_area.x;
        final_roi.y += search_area.y;
        final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
        
        // 【关键】从翻转后的图像截取，保证存出来的图不再是倒着的
        cv::Mat cropped = flipped(final_roi);

    //     // 如果是新遇到的红块（还没存过图），则存图并锁定
    //     if (!has_saved_this_block && photo_index <= 50) {
    //         char filename[64];
    //         sprintf(filename, "block_%d.jpg", photo_index);
    //         cv::imwrite(filename, cropped); 
            
    //         printf("[已识别到红块\n", filename);
            
    //         photo_index++;            
    //         has_saved_this_block = true; // 锁定，直到这个红块离开视野
    //     }
    //     red_lost_count = 0; // 只要能看到红色，就重置消失计数

        // 送去推理
        return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
    } 
    // else {
    //     // 连续 10 帧完全没看到红色，认为已经开过这个红块了，解锁下一次保存
    //     if (++red_lost_count > 10) {
    //         has_saved_this_block = false; 
    //     }
    // }

    //return false;
}