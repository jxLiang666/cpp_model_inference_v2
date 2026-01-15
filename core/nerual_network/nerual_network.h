#pragma once
#include <memory>
#include "nerual_network_base.h"
#include "data_adapter.h"
namespace nn {
/// @brief 神经网络封装类
/// @details
/// NerualNetwork 用于统一管理网络模型和数据适配器，提供一个简洁的接口
/// 来执行推理。它组合了 NerualNetworkBase（具体网络实现）和 DataAdapterBase
/// （数据输入/输出适配器），简化了推理流程。
class NerualNetwork {
public:
    NerualNetwork() = default;
    NerualNetwork(NerualNetwork &&) = default;
    NerualNetwork(const NerualNetwork &) = default;
    NerualNetwork &operator=(NerualNetwork &&) = default;
    NerualNetwork &operator=(const NerualNetwork &) = default;
    virtual ~NerualNetwork() = default;

    int init(std::unique_ptr< NerualNetworkBase > &_model, std::unique_ptr< DataAdapterBase > &_adapter) {
        model_ = std::move(_model);
        adapter_ = std::move(_adapter);
        return 0;
    }

    template < typename T, typename... Args >
    auto infer(Args &&..._args) -> T {
        // auto input = adapter_->createInputData(std::forward< Args >(_args)...);
        auto input = adapter_->createInputData(std::forward_as_tuple(_args...));

        std::vector< std::vector< nn::NetData > > output;

        auto ret = model_->infer(input, output);
        if (ret) {
            std::stringstream ss;
            ss << "Error occurs in model infer of " << model_->getName() << " , ret is " << ret;
            std::cerr << ss.str() << std::endl;
            throw std::runtime_error(ss.str());
        }
        return adapter_->createOutputData< T >(output);
    }

protected:
    std::unique_ptr< NerualNetworkBase > model_;
    std::unique_ptr< DataAdapterBase >   adapter_;
};
}  // namespace nn