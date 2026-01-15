#pragma once
#include <opencv2/opencv.hpp>
#include "data_adapter.h"
namespace nn {
class DefaultDataAdapter final : public DataAdapterBase {
protected:
    explicit DefaultDataAdapter() {};
    std::vector< std::vector< NetData > > doCreateInput(std::any &&_input) {
        using TupleType = std::tuple< std::vector< float > & >;
        auto &&args = std::any_cast< TupleType & >(_input);
        std::vector< float > input = std::get< 0 >(args);

    };
    std::any doCreateOutput(std::vector< std::vector< NetData > > &_output) {

    };
};
}  // namespace nn