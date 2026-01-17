#pragma once
#include "net_data_op.h"

namespace nn {
template < typename T >
void NetDataOp::hwc2chw(nn::NetData &_data) {
    const auto &shape = _data.getShape();
    const auto &height = shape[0];
    const auto &width = shape[1];
    const auto &channels = shape[2];

    assert(shape.size() == 3);
    assert(sizeof(T) == _data.getUnitSize());
    assert(_data.getTotalSize() == height * width * channels * sizeof(T));

    auto data = nn::NetData(_data.getTotalSize(), _data.getUnitSize(), {channels, height, width});
    auto buffer = reinterpret_cast< T * >(data.getMutableData());
    auto src = reinterpret_cast< T * >(_data.getData());

    for (size_t c = 0; c < channels; ++c) {
        for (size_t h = 0; h < height; ++h) {
            for (size_t w = 0; w < width; ++w) {
                buffer[c * height * width + h * width + w] =
                    src[h * width * channels + w * channels + c];
            }
        }
    }
    _data = std::move(data);
}

template < typename T >
void NetDataOp::chw2hwc(nn::NetData &_data) {
    const auto &shape = _data.getShape();
    const auto &channels = shape[0];
    const auto &height = shape[1];
    const auto &width = shape[2];

    assert(shape.size() == 3);
    assert(sizeof(T) == _data.getUnitSize());
    assert(_data.getTotalSize() == height * width * channels * sizeof(T));

    auto data = nn::NetData(_data.getTotalSize(), _data.getUnitSize(), {height, width, channels});
    auto buffer = reinterpret_cast< T * >(data.getMutableData());
    auto src = reinterpret_cast< T * >(_data.getData());

    for (size_t c = 0; c < channels; ++c) {
        for (size_t h = 0; h < height; ++h) {
            for (size_t w = 0; w < width; ++w) {
                buffer[h * width * channels + w * channels + c] =
                    src[c * height * width + h * width + w];
            }
        }
    }
    _data = std::move(data);
}

template < typename T >
void NetDataOp::bhwc2bchw(nn::NetData &_data) {
    const auto &shape = _data.getShape();
    const auto &batch = shape[0];
    const auto &height = shape[1];
    const auto &width = shape[2];
    const auto &channels = shape[3];

    assert(shape.size() == 4);
    assert(sizeof(T) == _data.getUnitSize());
    assert(_data.getTotalSize() == batch * height * width * channels * sizeof(T));

    auto data = nn::NetData(_data.getTotalSize(), _data.getUnitSize(), {batch, channels, height, width});
    auto buffer = reinterpret_cast< T * >(data.getMutableData());
    auto src = reinterpret_cast< T * >(_data.getData());

    const size_t hwc_stride = height * width * channels;
    const size_t chw_stride = channels * height * width;

    for (size_t b = 0; b < batch; ++b) {
        for (size_t c = 0; c < channels; ++c) {
            for (size_t h = 0; h < height; ++h) {
                for (size_t w = 0; w < width; ++w) {
                    buffer[b * chw_stride + c * height * width + h * width + w] =
                        src[b * hwc_stride + h * width * channels + w * channels + c];
                }
            }
        }
    }
    _data = std::move(data);
}

template < typename T >
void NetDataOp::bchw2bhwc(nn::NetData &_data) {
    const auto &shape = _data.getShape();
    const auto &batch = shape[0];
    const auto &channels = shape[1];
    const auto &height = shape[2];
    const auto &width = shape[3];

    assert(shape.size() == 4);
    assert(sizeof(T) == _data.getUnitSize());
    assert(_data.getTotalSize() == batch * height * width * channels * sizeof(T));

    auto data = nn::NetData(_data.getTotalSize(), _data.getUnitSize(), {batch, height, width, channels});
    auto buffer = reinterpret_cast< T * >(data.getMutableData());
    auto src = reinterpret_cast< T * >(_data.getData());

    const size_t chw_stride = channels * height * width;
    const size_t hwc_stride = height * width * channels;

    for (size_t b = 0; b < batch; ++b) {
        for (size_t c = 0; c < channels; ++c) {
            for (size_t h = 0; h < height; ++h) {
                for (size_t w = 0; w < width; ++w) {
                    buffer[b * hwc_stride + h * width * channels + w * channels + c] =
                        src[b * chw_stride + c * height * width + h * width + w];
                }
            }
        }
    }
    _data = std::move(data);
}
}  // namespace nn