#include <iostream>
#include <filesystem>
#include "nn_global_logger.h"
#include "onnx.h"

namespace nn {
Onnx::Onnx(const std::string &_model_path) : NerualNetworkBase() {
    namespace fs = std::filesystem;
    model_path_ = _model_path;
    name_ = fs::path(_model_path).filename().string();

    Onnx::init();
}

void Onnx::initHelper(std::stringstream                        &_ss,
                      std::string                              &_info,
                      std::vector< DataShapeVec >              &_shape,
                      std::vector< size_t >                    &_element_counts,
                      std::vector< size_t >                    &_element_size,
                      std::vector< size_t >                    &_total_size,
                      size_t                                   &_nodes_num,
                      std::vector< std::string >               &_node_names,
                      std::vector< const char * >              &_node_names_c,
                      std::vector< ONNXTensorElementDataType > &_types,
                      OnnxDataTypeVec                          &_tensors,
                      OnnxGetDataNameAllocatedFn                _onnxGetDataNameAllocatedFn,
                      OnnxGetInputTypeInfoFn                    _onnxGetInputTypeInfoFn) {
    _node_names.reserve(_nodes_num);
    _node_names_c.reserve(_nodes_num);
    _shape.reserve(_nodes_num);
    _types.reserve(_nodes_num);
    _element_counts.reserve(_nodes_num);
    _element_size.reserve(_nodes_num);
    _total_size.reserve(_nodes_num);

    for (size_t i = 0; i < _nodes_num; ++i) {
        auto name = (session_.*_onnxGetDataNameAllocatedFn)(i, allocator_);
        auto type_info = (session_.*_onnxGetInputTypeInfoFn)(i);
        auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
        auto shape = tensor_info.GetShape();

        auto type = tensor_info.GetElementType();
        auto element_size = getOnnxTensorElementDataTypeSize(type);

        size_t count = 0;
        if (!hasNegativeDim(shape)) {
            count = tensor_info.GetElementCount();
        } else {
            auto symbols = tensor_info.GetSymbolicDimensions();
            logDynamicTensorShape(model_path_, name.get(), shape, symbols);
        }
        auto size = count * element_size;

        _node_names.push_back(name.get());
        _node_names_c.push_back(_node_names.back().c_str());

        _shape.push_back(shape);
        _types.push_back(type);
        _element_counts.push_back(count);
        _element_size.push_back(element_size);
        _total_size.push_back(size);
    }
    for (size_t i = 0; i < _nodes_num; ++i) {
        auto name = _node_names_c[i];
        _ss << "- " << _info << " name " << i << " : " << name << std::endl;
        auto count = _element_counts[i];
        _ss << "- " << _info << " elem count " << i << " : " << count << std::endl;
        auto type = _types[i];
        _ss << "- " << _info << " type " << i << " : " << type << std::endl;
        auto size = _total_size[i];
        _ss << "- " << _info << " size " << i << " : " << size << std::endl;
        auto shape = _shape[i];
        _ss << "- " << _info << " shape " << i << " : ";
        for (size_t j = 0; j < shape.size(); ++j) {
            _ss << shape[j] << " ";
        }
        _ss << '\n'
            << std::endl;
    }
}
int Onnx::init() {
    env_ = Ort::Env(ORT_LOGGING_LEVEL_WARNING, name_.c_str());
    std::stringstream ss;
    ss << "model path: " << model_path_ << std::endl;
    session_options_ = Ort::SessionOptions();
    session_options_.SetGraphOptimizationLevel(ORT_ENABLE_ALL);
    session_ = Ort::Session(env_, model_path_.c_str(), session_options_);

    input_nodes_num_ = session_.GetInputCount();
    ss << "-- input_nodes_num: " << input_nodes_num_ << std::endl;
    std::string input_info = "Input";
    initHelper(ss, input_info, input_shape_, input_element_counts_, input_element_size_, input_total_size_, input_nodes_num_, input_node_names_, input_node_names_c_, input_types_, input_tensors_, &Ort::Session::GetInputNameAllocated, &Ort::Session::GetInputTypeInfo);

    output_nodes_num_ = session_.GetOutputCount();
    ss << "-- output_nodes_num: " << output_nodes_num_ << std::endl;
    std::string output_info = "Output";
    initHelper(ss, output_info, output_shape_, output_element_counts_, output_element_size_, output_total_size_, output_nodes_num_, output_node_names_, output_node_names_c_, output_types_, output_tensors_, &Ort::Session::GetOutputNameAllocated, &Ort::Session::GetOutputTypeInfo);

    NN_DEBUG("%s", ss.str().c_str());
    memory_info_ = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    return 0;
}

int Onnx::deinit() {
    return 0;
}

int Onnx::preprocess(NetBaseInputData &_input) {
    input_tensors_.resize(_input.size());  // 输入数量相同
    for (size_t i = 0; i < _input.size(); ++i) {
        assert(input_shape_[i] == _input[i].getShape());
        input_tensors_[i] = Ort::Value::CreateTensor(
            memory_info_,
            _input[i].getData(),
            _input[i].getTotalSize(),
            input_shape_[i].data(),
            input_shape_[i].size(),
            input_types_[i]);
    }
    return 0;
}
int Onnx::process(NetBaseInputData &_input, NetBaseOutputData &_output) {
    output_tensors_ = session_.Run(
        Ort::RunOptions{nullptr},
        input_node_names_c_.data(),
        input_tensors_.data(),
        input_nodes_num_,
        output_node_names_c_.data(),
        output_nodes_num_);

    assert(_output.size() == 0);
    _output.reserve(output_nodes_num_);
    for (size_t i = 0; i < output_nodes_num_; ++i) {
        // 零拷贝，_output的生命周期同output_tensors_, 即单次推理，与Onnx类同生命周期
        _output.emplace_back(output_tensors_[i].GetTensorRawData(), output_total_size_[i], output_element_size_[i], output_shape_[i], nn::NetData::noDelete);
    }
    return 0;
}

Onnx::~Onnx() {
    deinit();
}

std::string Onnx::repr() {
    return onnxRepr();
}
std::string Onnx::onnxRepr() {
    std::stringstream ss;
    ss << NerualNetworkBase::repr() << std::endl;
    ss << "-- Onnx Repr --" << std::endl;
    ss << "- input_nodes_num_ : " << input_nodes_num_ << std::endl;
    for (size_t i = 0; i < input_nodes_num_; ++i) {
        auto name = input_node_names_c_[i];
        ss << "- Input name " << i << " : " << name << std::endl;
        auto type = input_types_[i];
        ss << "- Input type " << i << " : " << type << std::endl;
    }
    ss << "- output_nodes_num_ : " << output_nodes_num_ << std::endl;
    for (size_t i = 0; i < output_nodes_num_; ++i) {
        auto name = output_node_names_c_[i];
        ss << "- Output name " << i << " : " << name << std::endl;
        auto type = output_types_[i];
        ss << "- Output type " << i << " : " << type << std::endl;
    }
    return ss.str();
}

int Onnx::getOnnxTensorElementDataTypeSize(const ONNXTensorElementDataType &type) {
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
bool Onnx::hasNegativeDim(const DataShapeVec &_shape) {
    for (const auto &dim : _shape) {
        if (dim < 0) {
            return true;
        }
    }
    return false;
}

void Onnx::logDynamicTensorShape(const std::string &_model_path, const char *_name, const DataShapeVec &_shape, const std::vector< const char * > &_symbols) {
    std::stringstream ss;
    ss << "model path: " << _model_path << " name:" << _name << std::endl;
    ss << "Tensor has dynamic shape, element count cannot be determined at load time. shape=[";

    for (size_t i = 0; i < _shape.size(); ++i) {
        ss << _shape[i];
        if (i + 1 < _shape.size()) {
            ss << ", ";
        }
    }
    ss << "]";

    if (!_symbols.empty()) {
        ss << ", symbolic_dims=[";
        for (size_t i = 0; i < _symbols.size(); ++i) {
            ss << _symbols[i];
            if (i + 1 < _symbols.size()) {
                ss << ", ";
            }
        }
        ss << "]";
    }

    ss << ".";
    NN_WARN("%s", ss.str().c_str());
}
}  // namespace nn