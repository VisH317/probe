#include "main.hpp"
#include <torch/torch.h>
#include <iostream>

int main() {
    torch::Tensor tensor = torch::rand({2, 3});
    std::cout << tensor << std::endl;
}

// structure planning
// create an observable neural network
// contains a list of linears for now:
// // get the weights from the linears and append an identifier list when creating
// classes required: updated linear with the uuids, updated neural network with the uuids