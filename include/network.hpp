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
        Network(std::vector<Layer>& l);
        Network(std::vector<Layer>&& l);
        ~Network() = default;

        void addLayer(Layer layer);
        std::optional<Layer> getLayer(int idx);

        torch::Tensor forward(torch::Tensor input);
        torch::Tensor operator() (torch::Tensor input) { return forward(input); };

        bool checkValid();
        // probe method
};

#endif