#include "main.hpp"
#include <torch/torch.h>
#include <iostream>
#include "layer.hpp"
#include "network.hpp"

int main() {
    Layer one(100, 50);
    Layer two(50, 1);
    // std::tuple<std::string, torch::Tensor, torch::Tensor> probe = one.getNeuron(0);
    // printf("UUID: %s\n", static_cast<std::string>(std::get<0>(probe)).c_str());
    // std::cout<<"Weights: "<<std::get<1>(probe)<<std::endl;
    // std::cout<<"Biases: "<<std::get<2>(probe)<<std::endl; 
    std::vector<Layer> layers{one, two};
    Network n(std::move(layers));

    torch::Tensor input = torch::ones(100);

    
}

// structure planning
// create an observable neural network
// contains a list of linears for now:
// // get the weights from the linears and append an identifier list when creating
// classes required: updated linear with the uuids, updated neural network with the uuids