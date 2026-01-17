#include <iostream>
#include "box.h"
#include "net_data.h"
using namespace nn::tool;
int main(){
    NN_OPEN(0);
    NN_ADD_FILESINK("/home/liang/workspace/github_prj/my_prj/cpp_model_inference_v2/.log",5*1024*1024,0);
    Box box1 = Box{0,0,2,2};
    std::cout<<"box1's area is "<<box1.area()<<std::endl;

    Box box2 = Box{1,1,2,2};
    std::cout<<"the iou of box1 and box2 is "<<Box::iou(box1,box2)<<std::endl;
}