#include "nerual_network_base.h"
namespace nn {
int NerualNetworkBase::pipeline(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) {
    int ret = 0;
    ret = preprocess(_input);
    if (ret != 0) return ret;
    ret = process(_input, _output);
    if (ret != 0) return ret;
    ret = postprocess(_output);
    if (ret != 0) return ret;
    return 0;
}

std::string NerualNetworkBase::repr() {
    return nnbaseRepr();
}

void NerualNetworkBase::setInputShape(const std::vector< DataShapeVec > &_input_shape) {
    if (_input_shape.size() != input_element_size_.size()) {
        NN_ERROR(
            "When set input shape, the size of _input_shape ({}) "
            "must be the same as input_element_size_ ({})!",
            _input_shape.size(),
            input_element_size_.size());
        throw std::invalid_argument(
            "setInputShape: _input_shape size mismatch with input_element_size_");
    }
    input_shape_ = _input_shape;
    input_element_counts_.resize(_input_shape.size());
    input_total_size_.resize(_input_shape.size());
    for (size_t i = 0; i < _input_shape.size(); ++i) {
        size_t element_counts = 1;
        for (size_t j = 0; j < _input_shape[i].size(); ++j) {
            element_counts *= _input_shape[i][j];
        }
        input_element_counts_[i] = element_counts;
        input_total_size_[i] = element_counts * input_element_size_[i];
    }
}

void NerualNetworkBase::setOutputShape(const std::vector< DataShapeVec > &_output_shape) {  ///< 设置输出的每个维度, 同时修改元素个数和所占的总内存
    if (_output_shape.size() != output_element_size_.size()) {
        NN_ERROR(
            "When set output shape, the size of _output_shape ({}) "
            "must be the same as output_element_size_ ({})!",
            _output_shape.size(),
            output_element_size_.size());
        throw std::invalid_argument(
            "setOutputShape: _output_shape size mismatch with output_element_size_");
    }
    output_shape_ = _output_shape;
    output_element_counts_.resize(_output_shape.size());
    output_total_size_.resize(_output_shape.size());
    for (size_t i = 0; i < _output_shape.size(); ++i) {
        size_t element_counts = 1;
        for (size_t j = 0; j < _output_shape[i].size(); ++j) {
            element_counts *= _output_shape[i][j];
        }
        output_element_counts_[i] = element_counts;
        output_total_size_[i] = element_counts * output_element_size_[i];
    }
}

std::string NerualNetworkBase::nnbaseRepr() {
    std::stringstream ss;
    ss << "-- NerualNetworkBase Repr --" << std::endl;
    ss << "- name_ : " << name_ << std::endl;
    ss << "- model_path_ : " << model_path_ << std::endl;
    ss << "- input_nodes_num_ : " << input_shape_.size() << std::endl;

    for (size_t i = 0; i < input_shape_.size(); ++i) {
        ss << "  [Input " << i << "]" << std::endl;

        // shape
        ss << "    shape : [";
        for (size_t j = 0; j < input_shape_[i].size(); ++j) {
            ss << input_shape_[i][j];
            if (j + 1 < input_shape_[i].size()) ss << ", ";
        }
        ss << "]" << std::endl;

        // element count
        ss << "    element_counts : " << input_element_counts_[i] << std::endl;

        // element size
        ss << "    element_size   : " << input_element_size_[i] << " bytes" << std::endl;

        // total size
        ss << "    total_size     : " << input_total_size_[i] << " bytes" << std::endl;
    }

    ss << "- output_nodes_num_ : " << output_shape_.size() << std::endl;

    for (size_t i = 0; i < output_shape_.size(); ++i) {
        ss << "  [Output " << i << "]" << std::endl;

        // shape
        ss << "    shape : [";
        for (size_t j = 0; j < output_shape_[i].size(); ++j) {
            ss << output_shape_[i][j];
            if (j + 1 < output_shape_[i].size()) ss << ", ";
        }
        ss << "]" << std::endl;

        // element count
        ss << "    element_counts : " << output_element_counts_[i] << std::endl;

        // element size
        ss << "    element_size   : " << output_element_size_[i] << " bytes" << std::endl;

        // total size
        ss << "    total_size     : " << output_total_size_[i] << " bytes" << std::endl;
    }
    return ss.str();
}
}  // namespace nn
