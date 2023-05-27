#include "layer.hpp"

Layer::Layer(int in, int out) {
    layer = torch::nn::Linear(in, out);
    for(int i=0;i<out;i++) {
        std::string uuid = generateId();
        id.push_back(uuid);
    }
}

std::string Layer::generateId() {
    curId++;
    return std::to_string(--curId);
}

torch::nn::Linear& Layer::getLayer() {
    return layer;
}

std::pair<torch::Tensor, std::string> Layer::getNeuron(int num) {
    torch::Tensor neuronWeights = layer.get().weight;
    torch::Tensor ret = neuronWeights.index(num);
    std::string uuid = id[num];

    return {ret, uuid};
}