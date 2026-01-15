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

    ///< NetData拼接,支持多个NetData实例
    template < typename... Args >
    static NetData concat(Args &&...args) {
        static_assert((std::is_same_v< std::decay_t< Args >, NetData > && ...),
                      "All arguments to concat must be of type NetData");
        // 计算总大小
        size_t total_size = (args.getTotalSize() + ...);

        NetData result(total_size);
        size_t  offset = 0;

        // 把每个 NetData 拷贝到 result 内存
        static auto copy_fn = [&](NetData &nd) {
            std::memcpy(static_cast< char * >(result.getData()) + offset, nd.getData(), nd.getTotalSize());
            offset += nd.getTotalSize();
        };

        (copy_fn(args), ...);  // 展开参数包

        return result;
    }

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void hwc2chw(nn::NetData &_data, size_t _channels, size_t _height, size_t _width) {
        std::vector< T > buffer(_data.getTotalSize() / sizeof(T));
        auto             src = reinterpret_cast< T * >(_data.getData());

        for (int c = 0; c < _channels; ++c) {
            for (int h = 0; h < _height; ++h) {
                for (int w = 0; w < _width; ++w) {
                    buffer[c * _height * _width + h * _width + w] =
                        src[h * _width * _channels + w * _channels + c];
                }
            }
        }
        std::memcpy(_data.getData(), buffer.data(), _data.getTotalSize());
    }

    ///< 通道转换，T代表_data中的数据类型
    template < typename T >
    static void chw2hwc(nn::NetData &_data, size_t _channels, size_t _height, size_t _width) {
        std::vector< T > buffer(_data.getTotalSize() / sizeof(T));
        auto             src = reinterpret_cast< T * >(_data.getData());

        for (size_t c = 0; c < _channels; ++c) {
            for (size_t h = 0; h < _height; ++h) {
                for (size_t w = 0; w < _width; ++w) {
                    buffer[h * _width * _channels + w * _channels + c] =
                        src[c * _height * _width + h * _width + w];
                }
            }
        }
        std::memcpy(_data.getData(), buffer.data(), _data.getTotalSize());
    }
};

}  // namespace nn