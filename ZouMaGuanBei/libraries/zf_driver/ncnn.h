
#pragma once

#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <net.h>

/*!
 * @brief NCNN推理类
 * @details 用于加载NCNN模型并进行图像分类推理
 */
class LQ_NCNN
{
public:
    /*!
     * @brief   NCNN无参构造函数
     */
    LQ_NCNN();

    /*!
     * @brief   初始化NCNN模型
     * @return  true:成功 false:失败
     */
    bool Init();

    /*!
     * @brief   推理函数
     * @param   bgr_image : 输入BGR图像
     * @return  推理结果（类别名称或编号）
     */
    std::string Infer(const cv::Mat& bgr_image);

    /*!
     * @brief   设置模型路径
     * @param   param_path : 模型参数文件路径(.param)
     * @param   bin_path   : 模型权重文件路径(.bin)
     */
    void SetModelPath(const std::string& param_path, const std::string& bin_path);

    /*!
     * @brief   设置输入尺寸
     * @param   width  : 输入宽度
     * @param   height : 输入高度
     */
    void SetInputSize(int width, int height);

    /*!
     * @brief   设置类别标签
     * @param   labels : 类别名称列表
     */
    void SetLabels(const std::vector<std::string>& labels);

    /*!
     * @brief   设置归一化参数
     * @param   mean_vals  : 均值
     * @param   norm_vals : 标准差
     */
    void SetNormalize(const float mean_vals[3], const float norm_vals[3]);

    /*!
     * @brief   NCNN析构函数
     */
    ~LQ_NCNN();
    // 推理，返回类别ID和置信度（概率）
    bool InferWithConfidence(const cv::Mat& bgr_image, int& class_id, float& confidence);

private:
    /*!
     * @brief   获取最大概率类别索引
     * @param   logits : 模型输出
     * @return  类别索引
     */
    int Argmax(const ncnn::Mat& logits);

private:
    ncnn::Net                 m_net;              // NCNN网络
    std::vector<std::string>  m_labels;           // 类别标签
    bool                     m_initialized;      // 初始化标志

    // 模型路径
    std::string              m_param_path;
    std::string              m_bin_path;

    // 输入参数
    int                      m_input_width;
    int                      m_input_height;
    float                    m_mean_vals[3];
    float                    m_norm_vals[3];

    // 输入输出blob名称
    std::string              m_input_name;
    std::string              m_output_name;
};
