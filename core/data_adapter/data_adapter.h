#pragma once
#include <vector>
#include <any>
#include "net_data.h"

namespace nn {
/// @brief 数据适配器基类 (CRTP)
/// @details
template < typename Derived >
class DataAdapterBase {
public:
    DataAdapterBase() = default;
    ~DataAdapterBase() = default;

    DataAdapterBase(const DataAdapterBase &) = default;
    DataAdapterBase &operator=(const DataAdapterBase &) = default;
    DataAdapterBase(DataAdapterBase &&) = default;
    DataAdapterBase &operator=(DataAdapterBase &&) = default;

    /// @brief 创建模型输入
    template < typename... Args >
    NetBaseInputData createInputData(Args &&..._args) noexcept(noexcept(std::declval< Derived & >().doCreateInput(std::forward< Args >(_args)...))) {
        static_assert(std::is_base_of_v< DataAdapterBase, Derived >,
                      "Derived must inherit from DataAdapterBase<Derived>");
        return static_cast< Derived * >(this)->doCreateInput(std::forward< Args >(_args)...);
    }

    /// @brief 创建模型输出
    auto createOutputData(NetBaseOutputData &_output) noexcept(noexcept(std::declval< Derived & >().doCreateOutput(_output))) -> decltype(auto) {
        static_assert(std::is_base_of_v< DataAdapterBase, Derived >,
                      "Derived must inherit from DataAdapterBase<Derived>");
        return static_cast< Derived * >(this)->doCreateOutput(_output);
    }
};

}  // namespace nn