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
    DataShapeVec         output_shape{batch_size, 10};
    net->setInputShape({input_shape});
    net->setOutputShape({output_shape});
    NetData indata = NetData(static_cast< void * >(tmp_data.data()), batch_size * 3 * 16 * 16 * sizeof(float), sizeof(float), input_shape);
    input.push_back(std::move(indata));
    NetBaseDataTypeVec output;
    net->infer(input, output);

    auto &&o_data = static_cast< float * >(output[0].getData());
    for (auto i = 0; i < batch_size * 10; ++i) {
        std::cout << i << ":" << o_data[i] << std::endl;
    }
    std::cout << net->repr() << std::endl;
}