#pragma once
#include "zf_common_headfile.h"
#include <string>
#include <vector>

// 外部声明标签，方便主程序使用
extern const std::vector<std::string> class_labels;

// 初始化模型（只需调用一次）
bool ncnn_init(const std::string& param_path, const std::string& bin_path);

// 检测赛道内是否存在红块（减速用，不需要坐标）
bool is_red_present();
// 检测红块并返回其在二值化 ROI 区域中的矩形坐标（推理定位用）
//bool is_red_present(cv::Rect& red_rect_in_img);
bool detect_red_slow_down(const cv::Mat& bgr_image);
// 处理一帧图像（自动执行红色检测、ROI裁剪、推理）
// 返回值：true 表示成功识别到目标板（有红色块且推理有效）
// class_id: 0-5 对应六个小类
// confidence: 识别置信度 (0~1)
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence);
// 函数声明
bool detect_red_unified(const cv::Mat& bgr_image, cv::Rect& roi, bool& need_slow_down);
bool ncnn_infer_from_roi(const cv::Mat& bgr_image, const cv::Rect& roi, int& class_id, float& confidence);