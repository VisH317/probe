#ifndef __LAYER_H__
#define __LAYER_H__

#include <torch/torch.h>
#include <utility>

class Layer {
    private:
        torch::nn::Linear layer;
        std::vector<std::string> id;
        std::string generateId();

        static long long curId = 0;

    public: 
        Layer(int in, int out); // divide by out features: determines how much of each in feature
        ~Layer() = default;

        torch::nn::Linear& getLayer();

        std::pair<torch::Tensor, std::string> getNeuron(int num);
        // probe method
};

#endif