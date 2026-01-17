#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <assert.h>
#include <memory>
#include "nn_global_logger.h"
namespace nn {
using DataShapeVec = std::vector< int64_t >;
/// @brief 网络数据结构
/// @details
/// NetData 是一个简单的数据缓冲结构，用于存储神经网络输入或输出数据。
/// 它管理动态分配的内存，支持移动语义，但禁用拷贝构造和拷贝赋值，
class NetData {
private:
    using FreeFunc = void (*)(void *);
public:
    static void noDelete(void *) {}  ///< 辅助函数
public:
    explicit NetData(const size_t       &_total_size,
                     const size_t       &_unit_size,
                     const DataShapeVec &_shape,
                     FreeFunc            _p_free = std::free);

    ///< 复用外部的data
    explicit NetData(void               *_data,
                     const size_t       &_total_size,
                     const size_t       &_unit_size,
                     const DataShapeVec &_shape,
                     FreeFunc            _p_free = noDelete);

    explicit NetData(const void         *_data,
                     const size_t       &_total_size,
                     const size_t       &_unit_size,
                     const DataShapeVec &_shape,
                     FreeFunc            _p_free = noDelete);

    // 禁止拷贝
    NetData(const NetData &) = delete;
    NetData &operator=(const NetData &) = delete;

    // 移动函数
    NetData(NetData &&_other) noexcept;
    NetData &operator=(NetData &&_other) noexcept;

public:
    inline const auto &getTotalSize() const noexcept { return total_size_; }    ///< 获取总内存大小
    inline const auto &getUnitSize() const noexcept { return unit_size_; }      ///< 获取单位内存大小
    inline const auto &getSize() const noexcept { return getUnitSize(); }       ///< 获取单位内存大小
    inline const auto  getRawData() const noexcept { return data_.get(); }      ///< 获取数据域指针
    inline const auto  getData() const noexcept { return getRawData(); }        ///< 获取数据域指针
    inline auto        getMutableData() const noexcept { return data_.get(); }  ///< 获取数据域指针 (非const)
    inline const auto  getSharedData() const noexcept { return data_; }         ///< 获取数据域shared指针
    inline const auto &getShape() const noexcept { return shape_; }             ///< 获取shape

private:
    ///< 用于将变量归0
    void reset() noexcept;

public:
    ///< 调试内部信息
    std::string repr() const noexcept;

private:
    size_t                  total_size_{0};  ///< 代表data_所在的内存空间总大小（单位byte）
    size_t                  unit_size_{0};   ///< 代表data_所在的内存空间单位大小（单位byte）
    std::shared_ptr< void > data_{nullptr};  ///< 数据域指针
    DataShapeVec            shape_;          ///< 数据shape
};
}  // namespace nn