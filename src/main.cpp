#include "main.hpp"
#include <torch/torch.h>
#include <iostream>
#include "layer.hpp"
#include "network.hpp"

int main() {
    Layer one(100, 50);
    Layer two(50, 1);
    std::vector<Layer> layers = { one, two };
    Network net(layers)
}

// structure planning
// create an observable neural network
// contains a list of linears for now:
// // get the weights from the linears and append an identifier list when creating
// classes required: updated linear with the uuids, updated neural network with the uuids