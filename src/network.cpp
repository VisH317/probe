#include "network.hpp"


std::vector<std::shared_ptr<torch::nn::Module>> getLayers(std::shared_ptr<torch::nn::Module>& mod) {
    std::vector<std::shared_ptr<torch::nn::Module>> children = mod->children();
    std::vector<std::shared_ptr<torch::nn::Module>> ret;
    if(children.size() == 0) {
        ret.push_back(mod);
        return ret;
    }

    for(std::shared_ptr<torch::nn::Module>& child: children) {
        auto values = getLayers(child);
        ret.insert(ret.end(), values.begin(), values.end());
    }

    return ret;
}


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

Network::Network(std::shared_ptr<torch::nn::Module>& mod) {
    auto initLayers = getLayers(mod);
    bool firstLinear = false;
    std::vector<std::shared_ptr<torch::nn::AnyModule>> currentAux;
    std::shared_ptr<torch::nn::Module> currentLinear;
    for(std::shared_ptr<torch::nn::Module>& layer : initLayers) {
        std::cout<<"name: "<<layer->name()<<std::endl;
        if(layer->name() == "torch::nn::LinearImpl") {
            if(!firstLinear) {
                initModules = currentAux;
                currentAux.clear();
                currentLinear = layer;
                firstLinear = true;
            }
            layers.push_back(Layer(std::dynamic_pointer_cast<torch::nn::Linear>(currentLinear), currentAux));
            currentLinear = layer;
            currentAux.clear();
        } else currentAux.push_back(std::shared_ptr<torch::nn::AnyModule>(layer->as<torch::nn::AnyModule>()));
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

int Network::getLayerLength() {
    return layers.size();
}