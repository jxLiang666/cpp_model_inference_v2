#pragma once
#include <vector>
#include "data_adapter.h"
#include "net_data.h"
#include "nerual_network_base.h"
namespace nn {
class DemoDataAdapter : public DataAdapterBase< DemoDataAdapter > {
public:
    DemoDataAdapter() = default;

public:
    NetBaseInputData doCreateInput(std::vector< float > &_input_data, size_t _batch_size, DataShapeVec &_shape) {
        NetBaseInputData input;
        NetData          indata1 = NetData(static_cast< void * >(_input_data.data()), _batch_size * 3 * 16 * 16 * sizeof(float), sizeof(float), _shape);
        NetData          indata2 = NetData(static_cast< void * >(_input_data.data()), _batch_size * 3 * 16 * 16 * sizeof(float), sizeof(float), _shape);
        input.push_back(std::move(indata1));
        input.push_back(std::move(indata2));
        return input;
    }

    std::vector< float > doCreateOutput(NetBaseOutputData &_output) {
        std::vector< float > res;
        auto               &&o_data1 = static_cast< float * >(_output[0].getData());
        std::cout << _output[0].repr() << std::endl;
        res.push_back(o_data1[0]);
        std::cout << _output[1].repr() << std::endl;
        auto &&o_data2 = static_cast< float * >(_output[1].getData());
        res.push_back(o_data2[0]);
        return res;
    }
};
}  // namespace nn