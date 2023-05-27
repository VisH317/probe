#include "network.hpp"

Network::Network(std::vector<torch::nn::Linear> l) {
    std::vector<torch::nn::Linear>::iterator ptr;
    for(ptr = l.begin(), ptr<l.end(); ptr++) {
        layers.push_back(std::move(*ptr));
    }
}

void Network::addLayer(torch::nn::Linear l) {
    layers.push_back(l);
}