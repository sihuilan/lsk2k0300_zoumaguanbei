#pragma once
#include "zf_common_headfile.h"
#include <string>
#include <vector>
// 外部声明标签，方便主程序使用
extern const std::vector<std::string> class_labels;
// 初始化模型（只需调用一次）
bool ncnn_init(const std::string& param_path, const std::string& bin_path);

// 处理一帧图像（自动执行红色检测、ROI裁剪、推理）
// 返回值：true 表示成功识别到目标板（有红色块且推理有效）
// class_id: 0-5 对应六个小类
// confidence: 识别置信度 (0~1)
bool ncnn_process_frame(const cv::Mat& bgr_image, int& class_id, float& confidence);
bool is_red_present();