#pragma once
#include "onnxruntime_c_api.h"
#include "onnxruntime_cxx_api.h"
#include "nerual_network_base.h"

namespace nn {
using OnnxDataTypeVec = std::vector< Ort::Value >;
using OnnxGetDataNameAllocatedFn = Ort::AllocatedStringPtr (Ort::Session::*)(size_t, OrtAllocator *) const;
using OnnxGetInputTypeInfoFn = Ort::TypeInfo (Ort::Session::*)(size_t) const;
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
    int                 init() override;
    int                 deinit() override;
    int                 preprocess(NetBaseDataTypeVec &_input) override;
    int                 process(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) override;
    virtual std::string repr();  ///< 打印详细的信息接口

protected:
    ///< 需要实现infer函数
    virtual int infer(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) = 0;
    ///< 需要实现postprocess函数
    virtual int postprocess(NetBaseDataTypeVec &_output) = 0;

private:
    void initHelper(
        std::stringstream                        &_ss,
        std::string                              &_info,
        std::vector< DataShapeVec >              &_shape,
        std::vector< size_t >                    &_element_counts,
        std::vector< size_t >                    &_element_size,
        std::vector< size_t >                    &_total_size,
        size_t                                   &_nodes_num,
        std::vector< std::string >               &_node_names,
        std::vector< const char * >              &_node_names_c,
        std::vector< ONNXTensorElementDataType > &_types,
        OnnxDataTypeVec                          &_tensors,
        OnnxGetDataNameAllocatedFn                _onnxGetDataNameAllocatedFn,
        OnnxGetInputTypeInfoFn                    _onnxGetInputTypeInfoFn);  ///< init函数的辅助函数

    std::string onnxRepr();                                                                                                                      ///< Onnx内部调试信息
    static int  getOnnxTensorElementDataTypeSize(const ONNXTensorElementDataType &_type);                                                        ///< onnx数据类型的空间大小(byte)
    static bool hasNegativeDim(const DataShapeVec &_shape);                                                                                      ///< 判断是否有负数维度，有返回true，没有返回false
    static void logDynamicTensorShape(const std::string &_model_path, const DataShapeVec &_shape, const std::vector< const char * > &_symbols);  ///< init辅助函数

private:
    Ort::Env                         env_{nullptr};              ///< ONNX Runtime 环境
    Ort::MemoryInfo                  memory_info_{nullptr};      ///< 内存信息
    Ort::SessionOptions              session_options_{nullptr};  ///< 会话选项
    Ort::Session                     session_{nullptr};          ///< 模型会话
    Ort::AllocatorWithDefaultOptions allocator_;                 ///< 默认分配器

    size_t                                   input_nodes_num_;     ///< 输入节点数量
    std::vector< std::string >               input_node_names_;    ///< 输入节点名字
    std::vector< const char * >              input_node_names_c_;  ///< 输入节点名字,c类型
    std::vector< ONNXTensorElementDataType > input_types_;         ///< 输入变量类型
    OnnxDataTypeVec                          input_tensors_;       ///< 输入张量

    size_t                                   output_nodes_num_;     ///< 输出节点数量
    std::vector< std::string >               output_node_names_;    ///< 输出节点名字
    std::vector< const char * >              output_node_names_c_;  ///< 输出节点名字,c类型
    std::vector< ONNXTensorElementDataType > output_types_;         ///< 输出变量类型
    OnnxDataTypeVec                          output_tensors_;       ///< 输出张量
};
}  // namespace nn