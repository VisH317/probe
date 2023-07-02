#include "network.hpp"

Network::Network(std::vector<Layer>& l) {
    std::vector<Layer>::iterator ptr;
    for(ptr = l.begin(); ptr<l.end(); ptr++) {
        layers.push_back(*ptr);
    }
}

Network::Network(std::vector<Layer>&& l) {
    std::vector<Layer>::iterator ptr;
    for(ptr = l.begin(); ptr<l.end(); ptr++) {
        layers.push_back(*ptr);
    }
}

void Network::addLayer(Layer l) {
    layers.push_back(l);
}

std::optional<Layer> Network::getLayer(int idx) {
    try {
        return layers[idx];
    } catch(...) {
        throw std::out_of_range("Layer index out of range");
        return std::nullopt;
    }
}

bool Network::checkValid() {
    if(layers.size()==1) return true;
    bool check = true;
    for(int i=0;i<layers.size()-1;i++) {
        check = check && layers[i].getDims().second == layers[i+1].getDims().first;
    }

    return check;
}

torch::Tensor Network::forward(torch::Tensor input) {

    if(layers[0].getDims().first!=input.squeeze().size(0) || !checkValid()) throw std::out_of_range("Input tensor dimension mismatch");

    torch::Tensor* cur = &input;
    for(Layer& l : layers) {
        *cur = l.getLayer()->forward(*cur);
        torch::nn::Linear x(100, 50);
    }

    return *cur;
}