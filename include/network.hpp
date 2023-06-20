#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <torch/torch.h>
#include <iterator>
#include <vector>
#include "layer.hpp"

class Network {
    private:
        std::vector<Layer> layers;

    public:
        Network() {};
        Network(std::vector<torch::nn::Linear>& v);
        ~Network() = default;

        void addLayer(torch::nn::Linear layer);
        Layer getLayer(int idx);
        torch::Tensor forward(torch::Tensor input);
        bool checkValid();
        // probe method
};

#endif