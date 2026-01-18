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

    int init(std::unique_ptr< NerualNetworkBase > &_model, std::unique_ptr< Adapter > &_adapter) {
        model_ = std::move(_model);
        adapter_ = std::move(_adapter);
        return 0;
    }

    template < typename... Args >
    auto infer(Args &&..._args) -> decltype(auto) {
        auto               input = adapter_->createInputData(std::forward< Args >(_args)...);
        NetBaseDataTypeVec output;
        auto               ret = model_->infer(input, output);
        if (ret) {
            std::stringstream ss;
            ss << "Error occurs in model infer of " << model_->getName() << " , ret is " << ret;
            std::cerr << ss.str() << std::endl;
            throw std::runtime_error(ss.str());
        }
        return adapter_->createOutputData(output);
    }

protected:
    std::unique_ptr< NerualNetworkBase > model_;
    std::unique_ptr< Adapter >           adapter_;
};
}  // namespace nn