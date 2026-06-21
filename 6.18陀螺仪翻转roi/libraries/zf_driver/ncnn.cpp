#include "zf_common_headfile.h"
#include "ncnn.h"
#include <limits>
#include <stdexcept>
#include <opencv2/imgproc.hpp>

//* @函数名称：LQ_NCNN::LQ_NCNN()
// * @功能说明：NCNN类的构造函数
LQ_NCNN::LQ_NCNN()
    : m_initialized(false)
    , m_input_width(96)
    , m_input_height(96)
    , m_input_name("in0")
    , m_output_name("out0")
{
    // 默认使用ImageNet标准归一化
    m_mean_vals[0] = 123.675f;
    m_mean_vals[1] = 116.28f;
    m_mean_vals[2] = 103.53f;
    m_norm_vals[0] = 0.01712475f;
    m_norm_vals[1] = 0.017507f;
    m_norm_vals[2] = 0.01742919f;
}

// * @函数名称：bool LQ_NCNN::Init()
// * @功能说明：初始化NCNN模型
bool LQ_NCNN::Init()
{
    // 设置NCNN选项
    m_net.opt.use_vulkan_compute = false;
    m_net.opt.num_threads = 1;

    // 加载模型参数
    if (m_param_path.empty() || m_net.load_param(m_param_path.c_str()) != 0) {
        printf("NCNN: 加载参数文件失败: %s\n", m_param_path.c_str());
        return false;
    }

    // 加载模型权重
    if (m_bin_path.empty() || m_net.load_model(m_bin_path.c_str()) != 0) {
        printf("NCNN: 加载模型文件失败: %s\n", m_bin_path.c_str());
        return false;
    }

    m_initialized = true;
    printf("NCNN: 模型加载成功 (%s)\n", m_param_path.c_str());

    return true;
}

//* @函数名称：std::string LQ_NCNN::Infer(const cv::Mat& bgr_image)
std::string LQ_NCNN::Infer(const cv::Mat& bgr_image)
{
    if (!m_initialized) {
        throw std::runtime_error("NCNN not initialized. Call Init() first.");
    }
    if (bgr_image.empty()) {
        throw std::invalid_argument("Input image is empty.");
    }

    // 1. 调整图像尺寸 (保持不变)
    cv::Mat resized;
    cv::resize(bgr_image, resized, cv::Size(m_input_width, m_input_height));

    // 2. 【关键修改】：注释掉 BGR 转 RGB 的这两行
     cv::Mat rgb;
     cv::cvtColor(resized, rgb, cv::COLOR_BGR2RGB);

    // 3. 【关键修改】：将输入从 rgb.data 改为 resized.data，并把 PIXEL_RGB 改为 PIXEL_BGR
   // 即使图片本身是 BGR，这里告诉 NCNN 把它转换成模型需要的 RGB
ncnn::Mat input = ncnn::Mat::from_pixels(
    rgb.data, 
    ncnn::Mat::PIXEL_RGB, 
    m_input_width, 
    m_input_height
);

    // 剩下的归一化和推理逻辑保持不变
    input.substract_mean_normalize(m_mean_vals, m_norm_vals);

    ncnn::Extractor ex = m_net.create_extractor();
    ex.input(m_input_name.c_str(), input);

    ncnn::Mat logits;
    ex.extract(m_output_name.c_str(), logits);

    int class_id = Argmax(logits);

    if (class_id < 0) {
        throw std::runtime_error("Failed to get class id from output logits.");
    }

    // 返回类别名称或编号
    if (class_id >= 0 && class_id < static_cast<int>(m_labels.size())) {
        return m_labels[class_id];
    }
    return std::to_string(class_id);
}


//@函数名称：void LQ_NCNN::SetModelPath(const std::string& param_path, const std::string& bin_path)

void LQ_NCNN::SetModelPath(const std::string& param_path, const std::string& bin_path)
{
    m_param_path = param_path;
    m_bin_path = bin_path;
}


// * @函数名称：void LQ_NCNN::SetInputSize(int width, int height)
// * @功能说明：设置输入尺寸
void LQ_NCNN::SetInputSize(int width, int height)
{
    m_input_width = width;
    m_input_height = height;
}


// * @函数名称：void LQ_NCNN::SetLabels(const std::vector<std::string>& labels)
// * @功能说明：设置类别标签
void LQ_NCNN::SetLabels(const std::vector<std::string>& labels)
{
    m_labels = labels;
}

//@函数名称：void LQ_NCNN::SetNormalize(const float mean_vals[3], const float norm_vals[3])
void LQ_NCNN::SetNormalize(const float mean_vals[3], const float norm_vals[3])
{
    m_mean_vals[0] = mean_vals[0];
    m_mean_vals[1] = mean_vals[1];
    m_mean_vals[2] = mean_vals[2];
    m_norm_vals[0] = norm_vals[0];
    m_norm_vals[1] = norm_vals[1];
    m_norm_vals[2] = norm_vals[2];
}

// * @函数名称：int LQ_NCNN::Argmax(const ncnn::Mat& logits)
int LQ_NCNN::Argmax(const ncnn::Mat& logits)
{
    if (logits.w <= 0) {
        return -1;
    }

    int best_index = 0;
    float best_value = -std::numeric_limits<float>::infinity();

    for (int i = 0; i < logits.w; ++i) {
        const float value = logits[i];
        if (value > best_value) {
            best_value = value;
            best_index = i;
        }
    }
    return best_index;
}

//* @函数名称：LQ_NCNN::~LQ_NCNN()
LQ_NCNN::~LQ_NCNN()
{
    // NCNN会自动释放资源
}
bool LQ_NCNN::InferWithConfidence(const cv::Mat& bgr_image, int& class_id, float& confidence)
{
    if (!m_initialized || bgr_image.empty()) return false;

    // 1. 预处理
    cv::Mat resized;
    cv::resize(bgr_image, resized, cv::Size(m_input_width, m_input_height));
    
    ncnn::Mat input = ncnn::Mat::from_pixels(
        resized.data, 
        ncnn::Mat::PIXEL_BGR2RGB, 
        m_input_width, 
        m_input_height
    );
    input.substract_mean_normalize(m_mean_vals, m_norm_vals);

    // 2. 推理
    ncnn::Extractor ex = m_net.create_extractor();
    ex.input(m_input_name.c_str(), input);
    ncnn::Mat logits;
    ex.extract(m_output_name.c_str(), logits);

    // 3. 计算 Softmax —— 静态数组，避免动态分配
    const int MAX_CLASSES = 16;
    static float scores[MAX_CLASSES];
    
    int num_classes = logits.w;
    if (num_classes > MAX_CLASSES) num_classes = MAX_CLASSES;

    float max_logit = -FLT_MAX;
    for (int i = 0; i < num_classes; i++) {
        if (logits[i] > max_logit) max_logit = logits[i];
    }

    float sum = 0.0f;
    for (int i = 0; i < num_classes; i++) {
        scores[i] = exp(logits[i] - max_logit); 
        sum += scores[i];
    }

    class_id = 0;
    float max_score = 0;
    for (int i = 0; i < num_classes; i++) {
        float prob = scores[i] / sum;
        if (prob > max_score) {
            max_score = prob;
            class_id = i;
        }
    }
    
    confidence = max_score; 
    return true;
}