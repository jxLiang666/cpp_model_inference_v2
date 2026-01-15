#include "nerual_network_base.h"
namespace nn {
int NerualNetworkBase::pipeline(std::vector< std::vector< NetData > > &_input, std::vector< std::vector< NetData > > &_output) {
    int ret = 0;
    ret = preprocess(_input);
    if (ret != 0) return ret;
    ret = process(_output);
    if (ret != 0) return ret;
    ret = postprocess(_output);
    if (ret != 0) return ret;
    return 0;
}
}  // namespace nn
