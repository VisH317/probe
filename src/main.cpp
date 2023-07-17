#include "network.hpp"
#include "netManager.hpp"
#include "config.hpp"
#include <vector>
#include <torch/torch.h>

int main() {
    Layer l1(100, 50);
    Layer l2(50, 1);

    std::vector<Layer> layers = { l1, l2 };
    Network n(layers);

    // config
    int numWorkers = 1;
    torch::Tensor input = torch::rand(100);
    std::vector<int> outputs = { 0 };
    double desiredLoss = 5;
    Config config(0.15, 0.07, 1);

    std::cout<<"initializing..."<<std::endl;

    NetManager m(n, numWorkers, input, outputs, config, desiredLoss);

    std::cout<<"netmanager initialized!"<<std::endl;

    m.start();
}