#include "layer.hpp"

long long Layer::curId = 0;

Layer::Layer(int in, int out) {
    this->in = in;
    this->out = out;
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

std::tuple<std::string, torch::Tensor, torch::Tensor> Layer::getNeuron(int num) {
    torch::Tensor neuronWeights = layer->weight;
    torch::Tensor ret = neuronWeights.select(0, num);
    
    torch::Tensor neuronBiases = layer->bias;
    torch::Tensor b = neuronBiases.select(0, num);

    std::string uuid = id[num];

    return {uuid, ret, b};
}

std::pair<int, int> Layer::getDims() {
    return { in, out }
}