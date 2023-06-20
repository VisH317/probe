#include "network.hpp"

Network::Network(std::vector<Layer>& l) {
    std::vector<Layer>::iterator ptr;
    for(ptr = l.begin(); ptr<l.end(); ptr++) {
        layers.push_back(*ptr);
    }
}

void Network::addLayer(Layer l) {
    layers.push_back(l);
}

Layer Network::getLayer(int idx) {
    try {
        return layers[idx];
    } catch(...) {
        std::cerr<<"Your selected layer index is out of bounds"<<std::endl;
    }
}