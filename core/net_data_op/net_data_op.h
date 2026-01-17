#pragma once
#include "net_data.h"
namespace nn {
class NetDataOp {
public:
    NetDataOp() = delete;
    ~NetDataOp() = delete;
    NetDataOp(const NetDataOp &) = delete;
    NetDataOp &operator=(const NetDataOp &) = delete;
    NetDataOp(NetDataOp &&) = delete;
    NetDataOp &operator=(NetDataOp &&) = delete;

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void hwc2chw(nn::NetData &_data);

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void chw2hwc(nn::NetData &_data);

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void bhwc2bchw(nn::NetData &_data);

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void bchw2bhwc(nn::NetData &_data);
};
}  // namespace nn
#include "net_data_op_impl.h"
