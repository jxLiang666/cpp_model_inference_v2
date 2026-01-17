#pragma once
#include <string>
#include <vector>
#include "net_data.h"
#include "nn_global_logger.h"
namespace nn {
using NetBaseDataTypeVec = std::vector< NetData >;
/// @brief 神经网络基类
/// @details
/// NerualNetworkBase 提供了神经网络推理的统一接口，包括模型初始化、前处理、
/// 推理、后处理等完整流程。派生类需要实现具体的 init、deinit、preprocess、
/// process 和 postprocess 方法。
class NerualNetworkBase {
public:
    NerualNetworkBase() = default;
    NerualNetworkBase(NerualNetworkBase &&) = default;
    NerualNetworkBase(const NerualNetworkBase &) = default;
    NerualNetworkBase &operator=(NerualNetworkBase &&) = default;
    NerualNetworkBase &operator=(const NerualNetworkBase &) = default;
    virtual ~NerualNetworkBase() = default;
    virtual int         infer(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) = 0;  ///< 推理接口
    virtual std::string repr();                                                              ///< 打印详细的信息接口

    inline const auto &getName() const noexcept { return name_; }             ///< 获取模型名字
    inline const auto &getModelPath() const noexcept { return model_path_; }  ///< 获取模型路径

    inline const auto &getInputShape() const noexcept { return input_shape_; }                   ///< 获取输入的每个维度
    inline const auto &getInputElementCounts() const noexcept { return input_element_counts_; }  ///< 获取输入总元素个数
    inline const auto &getInputElementSize() const noexcept { return input_element_size_; }      ///< 获取输入元素每个单位所占內存(byte)
    inline const auto &getInputTotalSize() const noexcept { return input_total_size_; }          ///< 获取输入元素每个所占总内存(byte)

    inline const auto &getOutputShape() const noexcept { return output_shape_; }                   ///< 获取输出的每个维度
    inline const auto &getOutputElementCounts() const noexcept { return output_element_counts_; }  ///< 获取输出总元素个数
    inline const auto &getOutputElementSize() const noexcept { return output_element_size_; }      ///< 获取输出元素每个单位所占內存(byte)
    inline const auto &getOutputTotalSize() const noexcept { return output_total_size_; }          ///< 获取输出元素每个所占总内存(byte)

    // 需要在类初始化的时候，对 input_element_size_ 已经赋值才可以
    void setInputShape(const std::vector< DataShapeVec > &_input_shape);  ///< 设置输入的每个维度, 同时修改元素个数和所占的总内存

    // 需要在类初始化的时候，对 output_element_size_ 已经赋值才可以
    void setOutputShape(const std::vector< DataShapeVec > &_output_shape);  ///< 设置输出的每个维度, 同时修改元素个数和所占的总内存

protected:
    // 数据包装 [data1, data2]
    int         pipeline(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output);     ///< 封装整个前处理到后处理流程
    virtual int init() = 0;                                                            ///< 初始化
    virtual int deinit() = 0;                                                          ///< 反初始化
    virtual int preprocess(NetBaseDataTypeVec &_input) = 0;                            ///< 数据预处理
    virtual int process(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) = 0;  ///< 对应框架核心推理
    virtual int postprocess(NetBaseDataTypeVec &_output) = 0;                          ///< 模型数据后处理

    std::string nnbaseRepr();  ///< NerualNetworkBase内部调试信息

protected:
    std::string name_;        ///< 网络名称
    std::string model_path_;  ///< 模型路径

    // 最外围的vector是指输入输出的个数，并非batch_size
    std::vector< DataShapeVec > input_shape_;           ///< 输入的每个维度
    std::vector< size_t >       input_element_counts_;  ///< 输入总元素个数
    std::vector< size_t >       input_element_size_;    ///< 输入元素每个单位所占內存(byte)
    std::vector< size_t >       input_total_size_;      ///< 输入元素每个所占总内存(byte) [即对应 input_element_counts_ * input_element_size_]

    std::vector< DataShapeVec > output_shape_;           ///< 输出的每个维度
    std::vector< size_t >       output_element_counts_;  ///< 输出总元素个数
    std::vector< size_t >       output_element_size_;    ///< 输出元素每个单位所占內存(byte)
    std::vector< size_t >       output_total_size_;      ///< 输出元素每个所占总内存(byte) [即对应 input_element_counts_ * input_element_size_]
};
}  // namespace nn
