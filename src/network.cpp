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
        std::cerr<<"Your selected layer index is out of bounds"<<std::endl;
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
    torch::Tensor* cur = &input;
    for(Layer& l : layers) {
        *cur = l.getLayer()->forward(*cur);
        torch::nn::Linear x(100, 50);
    }

    return *cur;
}