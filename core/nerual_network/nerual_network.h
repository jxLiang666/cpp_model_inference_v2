#pragma once
#include <memory>
#include "nerual_network_base.h"
#include "data_adapter.h"
namespace nn {
/// @brief 神经网络封装类
template < typename Adapter >
class NerualNetwork {
public:
    NerualNetwork() = default;
    NerualNetwork(NerualNetwork &&) = default;
    NerualNetwork(const NerualNetwork &) = default;
    NerualNetwork &operator=(NerualNetwork &&) = default;
    NerualNetwork &operator=(const NerualNetwork &) = default;
    ~NerualNetwork() = default;

    int init(std::unique_ptr< NerualNetworkBase > &_model, std::unique_ptr< Adapter > &_adapter) {  ///< NerualNetwork 初始化函数
        model_ = std::move(_model);
        adapter_ = std::move(_adapter);
        return 0;
    }

    // 顶部类支持model setInputShape
    void setInputShape(const std::vector< DataShapeVec > &_input_shape) {  ///< 设置输入的每个维度, 同时修改元素个数和所占的总内存

        model_->setInputShape(_input_shape);
    }

    // 顶部类支持model setOutputShape
    void setOutputShape(const std::vector< DataShapeVec > &_output_shape) {  ///< 设置输出的每个维度, 同时修改元素个数和所占的总内存

        model_->setOutputShape(_output_shape);
    }

    template < typename... Args >
    auto infer(Args &&..._args) -> decltype(auto) {  ///< 核心推理接口
        auto               input = adapter_->createInputData(std::forward< Args >(_args)...);
        NetBaseDataTypeVec output;
        auto               ret = model_->infer(input, output);
        if (ret) {
            std::stringstream ss;
            ss << "Error occurs in model infer of " << model_->getName() << " , ret is " << ret;
            NN_ERROR("%s", ss.str().c_str());
            throw std::runtime_error(ss.str());
        }
        return adapter_->createOutputData(output);
    }

protected:
    std::unique_ptr< NerualNetworkBase > model_;
    std::unique_ptr< Adapter >           adapter_;
};
}  // namespace nn