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

bool Network::checkValid() {
    if(layers.size()==1) return true;
    bool check = true;
    for(int i=0;i<layers.size()-1;i++) {
        check = check && layers[i].getDims().second == layers[i+1].getDims().first;
    }

    return check
})