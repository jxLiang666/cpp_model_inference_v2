#include <iostream>
#include <filesystem>

#include "onnx.h"

namespace nn {
Onnx::Onnx(const std::string &_model_path) : NerualNetworkBase() {
    namespace fs = std::filesystem;
    model_path_ = _model_path;
    name_ = fs::path(_model_path).filename().string();

    Onnx::init();
};

int Onnx::init() {
    env_ = Ort::Env(ORT_LOGGING_LEVEL_WARNING, name_.c_str());
    std::cout << "model path: " << model_path_ << std::endl;
    session_options_ = Ort::SessionOptions();
    session_ = Ort::Session(env_, model_path_.c_str(), session_options_);

    num_input_nodes_ = session_.GetInputCount();

    std::cout << "-- num_input_nodes: " << num_input_nodes_ << std::endl;
    input_node_names_.reserve(num_input_nodes_);
    input_node_names_c_.reserve(num_input_nodes_);
    input_types_.reserve(num_input_nodes_);
    input_element_counts_.reserve(num_input_nodes_);
    input_element_size_.reserve(num_input_nodes_);
    input_size_.reserve(num_input_nodes_);
    input_shape_.reserve(num_input_nodes_);

    for (size_t i = 0; i < num_input_nodes_; ++i) {
        auto name = session_.GetInputNameAllocated(i, allocator_);
        auto type_info = session_.GetInputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto type = tensor_info.GetElementType();
        auto count = tensor_info.GetElementCount();
        auto element_size = getONNXTensorElementDataTypeSize(type);
        auto size = count * element_size;
        auto shape = tensor_info.GetShape();

        input_node_names_.push_back(name.get());
        input_node_names_c_.push_back(input_node_names_.back().c_str());
        input_types_.push_back(type);
        input_element_counts_.push_back(count);
        input_element_size_.push_back(element_size);
        input_size_.push_back(size);
        input_shape_.push_back(shape);
    }
    for (size_t i = 0; i < num_input_nodes_; ++i) {
        auto name = input_node_names_c_[i];
        std::cout << "- Input name " << i << " : " << name << std::endl;
        auto count = input_element_counts_[i];
        std::cout << "- Input elem count " << i << " : " << count << std::endl;
        auto type = input_types_[i];
        std::cout << "- Input type " << i << " : " << type << std::endl;
        auto size = input_size_[i];
        std::cout << "- Input size " << i << " : " << size << std::endl;
        auto shape = input_shape_[i];
        std::cout << "- Input shape " << i << " : ";
        for (size_t j = 0; j < shape.size(); ++j) {
            std::cout << shape[j] << " ";
        }
        std::cout << '\n'
                  << std::endl;
    }

    num_output_nodes_ = session_.GetOutputCount();
    std::cout << "-- num_output_nodes: " << num_output_nodes_ << std::endl;
    output_node_names_.reserve(num_output_nodes_);
    output_node_names_c_.reserve(num_output_nodes_);
    output_types_.reserve(num_output_nodes_);
    output_element_counts_.reserve(num_output_nodes_);
    output_element_size_.reserve(num_output_nodes_);
    output_size_.reserve(num_output_nodes_);
    output_shape_.reserve(num_output_nodes_);

    for (size_t i = 0; i < num_output_nodes_; ++i) {
        auto name = session_.GetOutputNameAllocated(i, allocator_);
        auto type_info = session_.GetOutputTypeInfo(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto type = tensor_info.GetElementType();
        auto count = tensor_info.GetElementCount();
        auto element_size = getONNXTensorElementDataTypeSize(type);
        auto size = count * element_size;
        auto shape = tensor_info.GetShape();

        output_node_names_.push_back(name.get());
        output_node_names_c_.push_back(output_node_names_.back().c_str());
        output_types_.push_back(type);
        output_element_counts_.push_back(count);
        output_element_size_.push_back(element_size);
        output_size_.push_back(size);
        output_shape_.push_back(shape);
    }
    for (size_t i = 0; i < num_output_nodes_; ++i) {
        auto name = output_node_names_c_[i];
        std::cout << "- Output name " << i << " : " << name << std::endl;
        auto count = output_element_counts_[i];
        std::cout << "- Output elem count " << i << " : " << count << std::endl;
        auto type = output_types_[i];
        std::cout << "- Output type " << i << " : " << type << std::endl;
        auto size = output_size_[i];
        std::cout << "- Output size " << i << " : " << size << std::endl;
        auto shape = output_shape_[i];
        std::cout << "- Output shape " << i << " : ";
        for (size_t j = 0; j < shape.size(); ++j) {
            std::cout << shape[j] << " ";
        }
        std::cout << '\n'
                  << std::endl;
    }

    memory_info_ = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    return 0;
}

int Onnx::deinit() {
    return 0;
}

int Onnx::preprocess(std::vector< std::vector< NetData > > &_input) {
    input_tensors_.resize(_input.size());  // 对齐batch
    for (size_t i = 0; i < _input.size(); ++i) {
        input_tensors_[i].resize(_input[i].size());
        for (size_t j = 0; j < _input[i].size(); ++j) {
            input_tensors_[i][j] = Ort::Value::CreateTensor(
                memory_info_,
                _input[i][j].getData(),
                _input[i][j].getSize(),
                input_shape_[j].data(),
                input_shape_[j].size(),
                input_types_[j]);
        }
    }
    return 0;
}
int Onnx::process(std::vector< std::vector< NetData > > &_output) {
    output_tensors_.resize(input_tensors_.size());  // 对齐batch
    for (size_t i = 0; i < input_tensors_.size(); ++i) {
        output_tensors_[i] = std::move(session_.Run(Ort::RunOptions{nullptr}, input_node_names_c_.data(), input_tensors_[i].data(), input_tensors_[i].size(), output_node_names_c_.data(), output_node_names_.size()));
    }

    for (size_t i = 0; i < output_tensors_.size(); ++i) {
        std::vector< NetData > output;
        output.reserve(output_tensors_[i].size());
        for (size_t j = 0; j < output_tensors_[i].size(); ++j) {
            output.emplace_back(output_element_counts_[j] * getONNXTensorElementDataTypeSize(output_types_[j]));
            auto data = output_tensors_[i][j].GetTensorRawData();
            std::memcpy(output.back().getData(), data, output_element_counts_[j] * getONNXTensorElementDataTypeSize(output_types_[j]));
        }
        _output.push_back(std::move(output));
    }

    return 0;
}

Onnx::~Onnx() {
    deinit();
}

int Onnx::getONNXTensorElementDataTypeSize(const ONNXTensorElementDataType &type) {
    switch (type) {
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT:
        return sizeof(float);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT8:
        return sizeof(uint8_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT8:
        return sizeof(int8_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_UINT16:
        return sizeof(uint16_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT16:
        return sizeof(int16_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT32:
        return sizeof(int32_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_INT64:
        return sizeof(int64_t);
    case ONNX_TENSOR_ELEMENT_DATA_TYPE_DOUBLE:
        return sizeof(double);
    // case ONNX_TENSOR_ELEMENT_DATA_TYPE_BOOL:
    //     return sizeof(bool);
    // case ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT16:
    //     return 2;  // 半精度浮点
    // case ONNX_TENSOR_ELEMENT_DATA_TYPE_BFLOAT16:
    //     return 2;  // bfloat16
    // case ONNX_TENSOR_ELEMENT_DATA_TYPE_STRING:
    //     return sizeof(void *);  // 字符串用指针表示
    default:
        throw std::invalid_argument("Unsupported ONNXTensorElementDataType");
    }
}
}  // namespace nn