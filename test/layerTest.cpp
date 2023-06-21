#include "test.hpp"

int Test::layerConstructor() {
    Layer l(100, 50);
    Layer l2(50, 1);
    return 0;
}

int Test::layerUUID() {
    Layer l(100, 50);
    std::tuple<std::string, torch::Tensor, torch::Tensor> info = l.getNeuron(0);
    if(std::get<1>(info).squeeze().size(0)==100 && std::get<2>(info).squeeze().size(0)==1) return 0;
    return 1;
}

int Test::layerDiffUUID() {
    Layer l(100, 50);
    auto info = l.getNeuron(0);
    auto info2 = l.getNeuron(1);
}