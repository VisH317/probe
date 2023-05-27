#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <torch/torch.h>

class Network {
    private:
        std::vector<torch::nn::Linear> layers;

    public:
        Network();
        Network(std::vector<torch::nn::Linear> v);
        ~Network();

        void addLayer(torch::nn::Linear layer);
        bool checkValid();
        // probe method
};

#endif