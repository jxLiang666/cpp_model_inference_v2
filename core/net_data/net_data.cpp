#include "net_data.h"

namespace nn {
NetData::NetData(const size_t       &_total_size,
                 const size_t       &_unit_size,
                 const DataShapeVec &_shape,
                 FreeFunc            _p_free) : total_size_(_total_size),
                                     unit_size_(_unit_size),
                                     data_(std::shared_ptr< void >(std::malloc(_total_size), _p_free)),
                                     shape_(_shape) {
    NN_TRACE("create NetData, %s", repr().c_str());
    assert(total_size_ && unit_size_);
    assert(total_size_ % unit_size_ == 0);
    if (!data_) throw std::bad_alloc();
}

NetData::NetData(void               *_data,
                 const size_t       &_total_size,
                 const size_t       &_unit_size,
                 const DataShapeVec &_shape,
                 FreeFunc            _p_free) : total_size_(_total_size),
                                     unit_size_(_unit_size),
                                     data_(std::shared_ptr< void >(_data, _p_free)),
                                     shape_(_shape) {
    NN_TRACE("get NetData, %s", repr().c_str());
    assert(total_size_ && unit_size_);
    assert(total_size_ % unit_size_ == 0);
}

NetData::NetData(const void         *_data,
                 const size_t       &_total_size,
                 const size_t       &_unit_size,
                 const DataShapeVec &_shape,
                 FreeFunc            _p_free) : NetData(const_cast< void * >(_data), _total_size, _unit_size, _shape, _p_free) {}

NetData::NetData(NetData &&_other) noexcept
    : total_size_(_other.total_size_),
      unit_size_(_other.unit_size_),
      data_(std::move(_other.data_)),
      shape_(std::move(_other.shape_)) {

    _other.reset();
    NN_TRACE("move NetData, %s", repr().c_str());
}

NetData &NetData::operator=(NetData &&_other) noexcept {
    if (this != &_other) {
        total_size_ = _other.total_size_;
        unit_size_ = _other.unit_size_;
        data_ = std::move(_other.data_);
        shape_ = std::move(_other.shape_);

        _other.reset();
        NN_TRACE("operator= move NetData, %s", repr().c_str());
    }
    return *this;
}

void NetData::reset() noexcept {
    this->total_size_ = 0;
    this->unit_size_ = 0;
    this->data_.reset();
    this->shape_.clear();
}

std::string NetData::repr() const noexcept {
#if DEBUG
    std::stringstream ss;
    ss << "data start at " << std::hex << data_.get() << std::dec;
    ss << ", total size is " << total_size_ << ", unit size is " << unit_size_ << ", shape = [";
    for (size_t i = 0; i < shape_.size(); ++i) {
        ss << shape_[i];
        if (i + 1 < shape_.size()) ss << ", ";
    }
    ss << "]";

    return ss.str();
#else
    return "";
#endif
}
}  // namespace nn