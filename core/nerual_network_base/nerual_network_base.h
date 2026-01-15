#pragma once
#include <string>
#include <vector>
#include "net_data.h"
namespace nn {
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
    virtual int infer(std::vector< std::vector< NetData > > &_input, std::vector< std::vector< NetData > > &_output) = 0;

    inline const std::string &getName() { return name_; }             ///< 获取模型名字
    inline const std::string &getModelPath() { return model_path_; }  ///< 获取模型路径

    inline const std::vector< std::vector< int64_t > > &getInputShape() { return input_shape_; }                   ///< 获取输入的每个维度
    inline const std::vector< size_t >                 &getInputElementCounts() { return input_element_counts_; }  ///< 获取输入总元素个数
    inline const std::vector< size_t >                 &getInputElementSize() { return input_element_size_; }      ///< 获取输入元素每个单位所占內存(byte)
    inline const std::vector< size_t >                 &getInputSize() { return input_size_; }                     ///< 获取输入元素每个所占总内存(byte)

    inline const std::vector< std::vector< int64_t > > &getOutputShape() { return output_shape_; }                   ///< 获取输出的每个维度
    inline const std::vector< size_t >                 &getOutputElementCounts() { return output_element_counts_; }  ///< 获取输出总元素个数
    inline const std::vector< size_t >                 &getOutputElementSize() { return output_element_size_; }      ///< 获取输出元素每个单位所占內存(byte)
    inline const std::vector< size_t >                 &getOutputSize() { return output_size_; }                     ///< 获取输出元素每个所占总内存(byte)

    inline void setInputShape(const std::vector< std::vector< int64_t > > _input_shape) { input_shape_ = _input_shape; }      ///< 设置输入的每个维度
    inline void setOutputShape(const std::vector< std::vector< int64_t > > _output_shape) { output_shape_ = _output_shape; }  ///< 设置输出的每个维度

protected:
    // 数据包装 [ batch1[data1,data2], batch2[data1,data2], batch3[data1,data2], batch4[data1,data2]]
    int         pipeline(std::vector< std::vector< NetData > > &_input, std::vector< std::vector< NetData > > &_output);  ///< 封装整个前处理到后处理流程
    virtual int init() = 0;
    virtual int deinit() = 0;
    virtual int preprocess(std::vector< std::vector< NetData > > &_input) = 0;
    virtual int process(std::vector< std::vector< NetData > > &_output) = 0;
    virtual int postprocess(std::vector< std::vector< NetData > > &_output) = 0;

protected:
    std::string name_;        ///< 网络名称
    std::string model_path_;  ///< 模型路径

    std::vector< std::vector< int64_t > > input_shape_;           ///< 输入的每个维度
    std::vector< size_t >                 input_element_counts_;  ///< 输入总元素个数
    std::vector< size_t >                 input_element_size_;    ///< 输入元素每个单位所占內存(byte)
    std::vector< size_t >                 input_size_;            ///< 输入元素每个所占总内存(byte) [即对应 input_element_counts_ * input_element_size_]

    std::vector< std::vector< int64_t > > output_shape_;           ///< 输出的每个维度
    std::vector< size_t >                 output_element_counts_;  ///< 输出总元素个数
    std::vector< size_t >                 output_element_size_;    ///< 输出元素每个单位所占內存(byte)
    std::vector< size_t >                 output_size_;            ///< 输出元素每个所占总内存(byte) [即对应 input_element_counts_ * input_element_size_]
};
}  // namespace nn
