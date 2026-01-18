#include <memory>
#include <fstream>
#include "net_data.h"
#include "onnx.h"
using namespace nn;
class DebugNet : public Onnx {
public:
    DebugNet(const std::string &_model_path) : nn::Onnx(_model_path) {};

    int infer(NetBaseDataTypeVec &_input, NetBaseDataTypeVec &_output) {
        pipeline(_input, _output);
        return 0;
    }
    int postprocess(NetBaseDataTypeVec &_output) {
        return 0;
    }
};
int main() {
    std::ifstream f("data/model_path.txt");
    std::string   model_path;
    std::getline(f, model_path);
    std::unique_ptr< nn::NerualNetworkBase > net = std::make_unique< DebugNet >(model_path);
    int                                      batch_size = 10;

    std::vector< float > tmp_data(batch_size * 3 * 16 * 16, 0.1);
    NetBaseDataTypeVec   input;
    DataShapeVec         input_shape{batch_size, 3, 16, 16};
    DataShapeVec         output1_shape{batch_size, 10};
    DataShapeVec         output2_shape{batch_size, 16, 8, 8};
    net->setInputShape({input_shape, input_shape});
    net->setOutputShape({output1_shape, output2_shape});
    NetData indata1 = NetData(static_cast< void * >(tmp_data.data()), batch_size * 3 * 16 * 16 * sizeof(float), sizeof(float), input_shape);
    NetData indata2 = NetData(static_cast< void * >(tmp_data.data()), batch_size * 3 * 16 * 16 * sizeof(float), sizeof(float), input_shape);
    input.push_back(std::move(indata1));
    input.push_back(std::move(indata2));
    NetBaseDataTypeVec output;
    net->infer(input, output);

    auto &&o_data1 = static_cast< float * >(output[0].getData());
    std::cout << output[0].repr()<<std::endl;
    for (auto i = 0; i < 1; ++i) {
        std::cout << i << ":" << o_data1[i] << std::endl;
    }
    std::cout << output[1].repr()<<std::endl;
    auto &&o_data2 = static_cast< float * >(output[1].getData());
    for (auto i = 0; i < 1; ++i) {
        std::cout << i << ":" << o_data2[i] << std::endl;
    }
    std::cout << net->repr() << std::endl;
}