#pragma once
#include <string>
#include <vector>
#include "net_data.h"
#include "nn_global_logger.h"
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

    // 需要在类初始化的时候，对 input_element_size_ 已经赋值才可以
    inline void setInputShape(const std::vector< std::vector< int64_t > > &_input_shape) {  ///< 设置输入的每个维度, 同时修改元素个数和所占的总内存
        if (_input_shape.size() != input_element_size_.size()) {
            NN_ERROR(
                "When set input shape, the size of _input_shape ({}) "
                "must be the same as input_element_size_ ({})!",
                _input_shape.size(),
                input_element_size_.size());
            throw std::invalid_argument(
                "setInputShape: _input_shape size mismatch with input_element_size_");
        }
        input_shape_ = _input_shape;
        input_element_counts_.resize(_input_shape.size());
        input_size_.resize(_input_shape.size());
        for (size_t i = 0; i < _input_shape.size(); ++i) {
            size_t element_counts = 1;
            for (size_t j = 0; j < _input_shape[i].size(); ++j) {
                element_counts *= _input_shape[i][j];
            }
            input_element_counts_[i] = element_counts;
            input_size_[i] = element_counts * input_element_size_[i];
        }
    }

    // 需要在类初始化的时候，对 output_element_size_ 已经赋值才可以
    inline void setOutputShape(const std::vector< std::vector< int64_t > > &_output_shape) {  ///< 设置输出的每个维度, 同时修改元素个数和所占的总内存
        if (_output_shape.size() != output_element_size_.size()) {
            NN_ERROR(
                "When set output shape, the size of _output_shape ({}) "
                "must be the same as output_element_size_ ({})!",
                _output_shape.size(),
                output_element_size_.size());
            throw std::invalid_argument(
                "setOutputShape: _output_shape size mismatch with output_element_size_");
        }
        output_shape_ = _output_shape;
        output_element_counts_.resize(_output_shape.size());
        output_size_.resize(_output_shape.size());
        for (size_t i = 0; i < _output_shape.size(); ++i) {
            size_t element_counts = 1;
            for (size_t j = 0; j < _output_shape[i].size(); ++j) {
                element_counts *= _output_shape[i][j];
            }
            output_element_counts_[i] = element_counts;
            output_size_[i] = element_counts * output_element_size_[i];
        }
    }

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
