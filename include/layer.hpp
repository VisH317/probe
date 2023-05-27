#ifndef __LAYER_H__
#define __LAYER_H__

#include <torch/torch.h>

class Layer {
    private:
        torch::nn::Linear layer;
        std::vector<std::string> id;

    public: 
        Layer();
        ~Layer();

}

#endif