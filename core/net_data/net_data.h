#pragma once
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <assert.h>
#include "nn_global_logger.h"
namespace nn {
/// @brief 网络数据结构
/// @details
/// NetData 是一个简单的数据缓冲结构，用于存储神经网络输入或输出数据。
/// 它管理动态分配的内存，支持移动语义，但禁用拷贝构造和拷贝赋值，
/// 同时提供深拷贝方法 `copy()`。
class NetData {
    using FreeFunc = void (*)(void *);

public:
    // #TODO: 删除
    explicit NetData(size_t _total_size, FreeFunc _p_free = std::free) : total_size_(_total_size), data_(std::malloc(_total_size)), free_func_(_p_free) {
        NN_TRACE("create NetData, size is %ld", total_size_);
        NN_TRACE("如果输出了这条消息，请查看netData这里，同时观察哪里调用了，因为此条后续会删除这种构造方式！");
        if (total_size_ == 0) {
            throw std::invalid_argument("NetData: total_size_ can not be zero!");
        }
        if (!data_) {
            throw std::bad_alloc();
        }
    }

    explicit NetData(size_t _total_size, size_t _size, FreeFunc _p_free = std::free) : total_size_(_total_size), size_(_size),
                                                                                       data_(std::malloc(_total_size)), free_func_(_p_free) {
        NN_TRACE("create NetData, size is %ld", total_size_);
        assert(total_size_ != 0 && "NetData: total_size_ can not be zero!");
        assert(size_ != 0 && "NetData: size_ can not be zero!");
        size_t real_div = total_size_ / size_;
        assert((size_ * real_div) == total_size_ && "NetData: total_size_ is not divisible by size_, size mismatch!");

        if (!data_) {
            throw std::bad_alloc();
        }
    }

    ///< 复用外部的data，不负责free
    explicit NetData(void *_data, size_t _total_size, size_t _size, FreeFunc _p_free = nullptr) : total_size_(_total_size), size_(_size),
                                                                                                  data_(_data), free_func_(_p_free) {
        NN_TRACE("get NetData, size is %ld", total_size_);
        assert(total_size_ != 0 && "NetData: total_size_ can not be zero!");
        assert(size_ != 0 && "NetData: size_ can not be zero!");
        size_t real_div = total_size_ / size_;
        assert((size_ * real_div) == total_size_ && "NetData: total_size_ is not divisible by size_, size mismatch!");

        if (!data_) {
            throw std::bad_alloc();
        }
    }

    ~NetData() {
        releaseData();
    }

    // 禁止拷贝
    NetData(const NetData &) = delete;
    NetData &operator=(const NetData &) = delete;

    NetData(NetData &&_other) noexcept
        : total_size_(_other.total_size_), size_(_other.size_), data_(_other.data_), free_func_(_other.free_func_) {
        reset(_other);
        NN_TRACE("move NetData, size is %ld", total_size_);
    }

    NetData &operator=(NetData &&_other) noexcept {
        if (this != &_other) {
            releaseData();
            total_size_ = _other.total_size_;
            data_ = _other.data_;
            size_ = _other.size_;
            free_func_ = _other.free_func_;

            reset(_other);
            NN_TRACE("operator= move NetData, size is %ld", total_size_);
        }
        return *this;
    }

    ///< 深拷贝
    NetData copy() const {
        NetData data(total_size_, size_, std::free);
        std::memcpy(data.data_, data_, total_size_);
        data.size_ = size_;
        NN_TRACE("copy NetData, size is %ld", total_size_);
        return data;
    }

    inline const size_t getTotalSize() const noexcept {
        return total_size_;
    };
    // #TODO: 修改返回变量为size_
    inline const size_t getSize() const noexcept {
        NN_TRACE("如果输出了这条消息，请查看netData这里，同时观察哪里调用了getSize()，因为此条后续会改为getTotalSize！");
        return total_size_;
    };
    inline const size_t getUnitSize() const noexcept { return size_; };
    inline void        *getData() const noexcept { return data_; };
    inline size_t       getCount() const noexcept { return (size_ == 0) ? 0 : (total_size_ / size_); };

private:
    ///< 释放data
    void releaseData() noexcept {
        if (data_ && free_func_) {
            NN_TRACE("release NetData, size is %ld", total_size_);
            free_func_(data_);
            data_ = nullptr;
        }
    }

    ///< 用于将变量归0
    static void reset(NetData &_net_data) noexcept {
        _net_data.total_size_ = 0;
        _net_data.size_ = 0;
        _net_data.data_ = nullptr;
        _net_data.free_func_ = nullptr;
    }

private:
    size_t   total_size_{0};       ///< 代表data_所在的内存空间总大小（单位byte）
    size_t   size_{0};             ///< 代表data_所在的内存空间单位大小（单位byte）
    void    *data_{nullptr};       ///< 数据域指针
    FreeFunc free_func_{nullptr};  ///< 释放data_的函数指针
};
}  // namespace nn