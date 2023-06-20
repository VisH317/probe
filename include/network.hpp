#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <torch/torch.h>
#include <iterator>
#include <vector>

class Network {
    private:
        std::vector<torch::nn::Linear> layers;

    public:
        Network() {};
        Network(std::vector<torch::nn::Linear>& v);
        ~Network() = default;

        void addLayer(torch::nn::Linear layer);
        // bool checkValid();
        // probe method
};

#endif