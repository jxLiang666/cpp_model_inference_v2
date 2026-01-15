#pragma once
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"

#include "nerual_network_base.h"
#include "box.h"

namespace nn {
/// @brief ONNX 神经网络实现类
/// @details
/// Onnx 类继承自 NerualNetworkBase，用于加载和运行 ONNX 格式的神经网络模型。
/// 它封装了 ONNX Runtime 的会话管理、输入输出张量处理、前处理和后处理。
class Onnx : public NerualNetworkBase {
public:
    Onnx(const std::string &_model_path);
    Onnx(Onnx &&) = default;
    Onnx(const Onnx &) = default;
    Onnx &operator=(Onnx &&) = default;
    Onnx &operator=(const Onnx &) = default;
    ~Onnx() override;

protected:
    int init() override;
    int deinit() override;
    int preprocess(std::vector< std::vector< NetData > > &_input) override;
    int process(std::vector< std::vector< NetData > > &_output) override;

protected:
    ///< 需要实现infer函数
    virtual int infer(std::vector< std::vector< NetData > > &_input, std::vector< std::vector< NetData > > &_output) = 0;
    ///< 需要实现postprocess函数
    virtual int postprocess(std::vector< std::vector< NetData > > &_output) = 0;

private:
    static int getONNXTensorElementDataTypeSize(const ONNXTensorElementDataType &_type);

private:
    Ort::Env                         env_{nullptr};              ///< ONNX Runtime 环境
    Ort::MemoryInfo                  memory_info_{nullptr};      ///< 内存信息
    Ort::SessionOptions              session_options_{nullptr};  ///< 会话选项
    Ort::Session                     session_{nullptr};          ///< 模型会话
    Ort::AllocatorWithDefaultOptions allocator_;                 ///< 默认分配器

    size_t                                   num_input_nodes_;     ///< 输入节点数量
    std::vector< std::string >               input_node_names_;    ///< 输入节点名字
    std::vector< const char * >              input_node_names_c_;  ///< 输入节点名字,c类型
    std::vector< ONNXTensorElementDataType > input_types_;         ///< 输入变量类型
    std::vector< std::vector< Ort::Value > > input_tensors_;       ///< 输入张量

    size_t                                   num_output_nodes_;     ///< 输出节点数量
    std::vector< std::string >               output_node_names_;    ///< 输出节点名字
    std::vector< const char * >              output_node_names_c_;  ///< 输出节点名字,c类型
    std::vector< ONNXTensorElementDataType > output_types_;         ///< 输出变量类型
    std::vector< std::vector< Ort::Value > > output_tensors_;       ///< 输出张量
};
}  // namespace nn