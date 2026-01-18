#include <memory>
#include <fstream>
#include "net_data.h"
#include "onnx.h"
#include "nerual_network.h"
#include "demo_data_adapter.h"
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
    std::unique_ptr< nn::NerualNetworkBase > net_base = std::make_unique< DebugNet >(model_path);
    int                                      batch_size = 10;

    std::vector< float > tmp_data(batch_size * 3 * 16 * 16, 0.1);
    NetBaseDataTypeVec   input;
    DataShapeVec         input_shape{batch_size, 3, 16, 16};
    DataShapeVec         output1_shape{batch_size, 10};
    DataShapeVec         output2_shape{batch_size, 16, 8, 8};
    net_base->setInputShape({input_shape, input_shape});
    net_base->setOutputShape({output1_shape, output2_shape});

    std::unique_ptr< DemoDataAdapter >                  adapter = std::make_unique< DemoDataAdapter >();
    std::unique_ptr< NerualNetwork< DemoDataAdapter > > net = std::make_unique< NerualNetwork< DemoDataAdapter > >();
    net->init(net_base, adapter);

    std::vector< float > out = net->infer(tmp_data, batch_size, input_shape);
    std::cout<<out[0]<<" "<<out[1]<<std::endl;
}