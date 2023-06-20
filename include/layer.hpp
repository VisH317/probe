#ifndef __LAYER_H__
#define __LAYER_H__

#include <torch/torch.h>
#include <utility>

class Layer {
    private:
        torch::nn::Linear layer = nullptr;
        std::vector<std::string> id;
        std::string generateId();
        int in;
        int out;

        static long long curId;

    public: 
        Layer(int in, int out); // divide by out features: determines how much of each in feature
        ~Layer() = default;

        torch::nn::Linear& getLayer();

        std::tuple<std::string, torch::Tensor, torch::Tensor> getNeuron(int num);
        std::pair<int, int> getDims();
        // probe method
};

#endif