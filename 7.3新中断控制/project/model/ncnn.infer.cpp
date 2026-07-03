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
bool is_red_present() {
    extern uint8_t Bmp[CROP_H][CROP_W];
    pthread_mutex_lock(&mutex);
    cv::Mat bin(CROP_H, CROP_W, CV_8UC1, (void*)Bmp);
    cv::Mat img;
    cv::flip(bin, img, -1);            // 倒装校正，得到正视图的 ROI 区域
    pthread_mutex_unlock(&mutex);

    // 左右裁剪，保留中间 80% 宽度（可根据赛道宽度微调）
    int margin = (int)(img.cols * 0.10);
    cv::Mat cropped = img(cv::Rect(margin, 0, img.cols - 2*margin, img.rows));

    // 1. 提取最大白色区域作为赛道
    std::vector<std::vector<cv::Point>> contours;
    cv::Mat temp = cropped.clone();
    cv::findContours(temp, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    if (contours.empty()) return false;

    double max_area = 0;
    int max_idx = -1;
    for (size_t i = 0; i < contours.size(); i++) {
        double a = cv::contourArea(contours[i]);
        if (a > max_area) { max_area = a; max_idx = i; }
    }
    if (max_idx == -1 || max_area < 600) return false;   // 赛道面积太小，忽略

    cv::Mat mask = cv::Mat::zeros(cropped.size(), CV_8UC1);
    cv::drawContours(mask, contours, max_idx, cv::Scalar(255), cv::FILLED);

    // 2. 只保留赛道内部，然后取反得到黑色区域
    cv::Mat only_track, black;
    cv::bitwise_and(cropped, mask, only_track);
    cv::bitwise_not(only_track, black);

    // 3. 开运算去噪（3x3 核，消除微小噪点）
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(black, black, cv::MORPH_OPEN, kernel);

    // 4. 查找黑色连通区域
    std::vector<std::vector<cv::Point>> black_contours;
    cv::findContours(black, black_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 5. 只要存在面积在 20~800 的黑色区域，就认为发现红块（减速触发条件）
    for (size_t i = 0; i < black_contours.size(); i++) {
        double area = cv::contourArea(black_contours[i]);
        if (area > 20 && area < 300) {  //面积阈值 大于350视为红砖（红块减速时
            return true;   // 存在一个即认为有红块
        }
    }
    return false;
}
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
    cv::Rect red_rect = cv::boundingRect(contours[max_idx]);

   int area_min = (red_rect.y < 60) ? 30 : 60;
   if (max_area < area_min) return false;

   // 扩展系数（调大）
   const float UP_SCALE   = 3.8f;   // 向上扩 5 倍红块高度
   const float DOWN_SCALE = 1.5f;   // 向下扩 1.5 倍（新增）
   const float SIDE_SCALE = 0.5f;   // 左右各扩 1.0 倍红块宽度

   int up_expand   = static_cast<int>(red_rect.height * UP_SCALE);
   int down_expand = static_cast<int>(red_rect.height * DOWN_SCALE);
   int side_expand = static_cast<int>(red_rect.width * SIDE_SCALE);

   int start_x = std::max(0, red_rect.x - side_expand);
   int start_y = std::max(0, red_rect.y - up_expand);
   int width  = red_rect.width + 2 * side_expand;
   int height = red_rect.height + up_expand + down_expand;
   // 边界裁剪
   if (start_x + width > bgr_image.cols) width = bgr_image.cols - start_x;
   if (start_y + height > bgr_image.rows) height = bgr_image.rows - start_y;
    roi = cv::Rect(start_x, start_y, width, height);
    return (roi.width > 0 && roi.height > 0);
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
// //调试1 ：26.5.13： 38cm识别
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
    if (!g_initialized || bgr_image.empty()) return false;

    // 1. 立即翻转！摄像头是倒装的，所以第一时间把图像转正
    cv::Mat flipped; 
    cv::flip(bgr_image, flipped, -1); 

    // 2. 在“正”的画面里定义搜索区域
    cv::Rect search_area(105, 90, 120, 140);
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
    static int photo_index = -1;
    static bool has_saved_this_block = false;
    static int red_lost_count = 0;

    // 首次调用时，从文件读取上次的序号
    if (photo_index < 0) {
        FILE* fp = fopen("photo_index.txt", "r");
        if (fp) {
            fscanf(fp, "%d", &photo_index);
            fclose(fp);
        } else {
            photo_index = 1;  // 文件不存在，从1开始
        }
    }

    if (found_red) {
    // 坐标转换：映射回 flipped 的坐标系
    cv::Rect final_roi = red_roi_in_search;
    final_roi.x += search_area.x;
    final_roi.y += search_area.y;
    // 【关键】先与图像边界求交集，并检查是否完全包含有效区域（避免边缘裁剪）
    final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
    if (final_roi.width <= 0 || final_roi.height <= 0) {
        return false;  // ROI 无效，跳过本帧
    }
    cv::Mat cropped = flipped(final_roi);
    // cv::Mat sharp;
    // cv::GaussianBlur(cropped, sharp, cv::Size(0,0), 3);
    // cv::addWeighted(cropped, 1.2, sharp, -0.2, 0, cropped);

   if (!has_saved_this_block) {
            char filename[64];
            sprintf(filename, "block_%d.jpg", photo_index);
            cv::imwrite(filename, cropped);
            // printf("[已保存截图: %s]\n", filename);
            
            // 更新序号文件
            FILE* fp = fopen("photo_index.txt", "w");
            if (fp) {
                fprintf(fp, "%d", photo_index + 1);
                fclose(fp);
            }
            photo_index++;
            has_saved_this_block = true;
        }
        red_lost_count = 0;
        return g_ncnn.InferWithConfidence(cropped, class_id, confidence);
    } 
    else {
        // 连续 20 帧完全没看到红色，认为已经开过这个红块了，解锁下一次保存
        if (++red_lost_count > 30) {
            has_saved_this_block = false; 
        }
    }

    return false;
}
// bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence) {
//     if (!g_initialized || bgr_image.empty()) return false;

//     static int photo_index = -1;
//     static bool has_saved_this_block = false;
//     static int red_lost_count = 0;

//     if (photo_index < 0) {
//         FILE* fp = fopen("photo_index.txt", "r");
//         if (fp) { fscanf(fp, "%d", &photo_index); fclose(fp); }
//         else photo_index = 1;
//     }

//     struct timeval t1, t2, t3, t4;
//     gettimeofday(&t1, NULL);

//     // 1. 翻转 + 搜索区域
//     cv::Mat flipped; 
//     cv::flip(bgr_image, flipped, -1); 
//     cv::Rect search_area(100, 80, 140, 140);
//     search_area &= cv::Rect(0, 0, flipped.cols, flipped.rows); 
//     cv::Mat search_region = flipped(search_area);

//     gettimeofday(&t2, NULL);

//     // 2. 红块检测（HSV）
//     cv::Rect red_roi_in_search;
//     bool found_red = detect_red_and_get_roi(search_region, red_roi_in_search);

//     gettimeofday(&t3, NULL);

//     if (found_red) {
//         cv::Rect final_roi = red_roi_in_search;
//         final_roi.x += search_area.x;
//         final_roi.y += search_area.y;
//         final_roi &= cv::Rect(0, 0, flipped.cols, flipped.rows);
//         if (final_roi.width <= 0 || final_roi.height <= 0) return false;
//         cv::Mat cropped = flipped(final_roi);

//         // 3. NCNN 推理
//         bool success = g_ncnn.InferWithConfidence(cropped, class_id, confidence);

//         gettimeofday(&t4, NULL);

//         // 打印分段耗时
//         long pre_time = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
//         long hsv_time = (t3.tv_sec - t2.tv_sec) * 1000000 + (t3.tv_usec - t2.tv_usec);
//         long ncnn_time = (t4.tv_sec - t3.tv_sec) * 1000000 + (t4.tv_usec - t3.tv_usec);
//         printf("Pre: %ld us, HSV: %ld us, NCNN: %ld us\n", pre_time, hsv_time, ncnn_time);

//         // 存图逻辑（保持不变）
//         if (!has_saved_this_block) {
//             char filename[64];
//             sprintf(filename, "block_%d.jpg", photo_index);
//             cv::imwrite(filename, cropped);
//             printf("[已保存截图: %s]\n", filename);
//             FILE* fp = fopen("photo_index.txt", "w");
//             if (fp) { fprintf(fp, "%d", photo_index + 1); fclose(fp); }
//             photo_index++;
//             has_saved_this_block = true;
//         }
//         red_lost_count = 0;
//         return success;
//     } else {
//         if (++red_lost_count > 20) has_saved_this_block = false;
//     }
//     return false;
// }