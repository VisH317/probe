#include "network.hpp"
#include "netManager.hpp"
#include "config.hpp"
#include <vector>
#include <string>
#include <torch/torch.h>
#include <iostream>
#include <boost/math/distributions.hpp>
using namespace boost::math;


int main(int argc, char* argv[]) {
    
    torch::nn::Sequential net(
        torch::nn::Linear(100, 50),
        torch::nn::ReLU(),
        torch::nn::Linear(50, 25),
        torch::nn::ReLU(),
        torch::nn::Linear(25, 5),
        torch::nn::Sigmoid()
    );

    std::shared_ptr<torch::nn::Module> mod(net->as<torch::nn::Module>());


    Network n(mod);

    // config
    int numWorkers = 1;
    torch::Tensor input = torch::rand(100);
    std::vector<int> outputs = { 0 };
    double desiredLoss = 5;
    // std::cout<<"argc: "<<argc<<", argv: "<<argv[1]<<std::endl;
    Config config(0.15, 0.07, 1, argc == 0 ? 100 : std::atoi(argv[1]));

    // double r = std::rand() / double(RAND_MAX);
    // beta_distribution<> dist(50.0, 50.0);
    // double randFromDist = quantile(dist, r);
    // std::cout<<randFromDist<<std::endl;

    std::cout<<"MAIN: initializing..."<<std::endl;

    NetManager m(n, numWorkers, input, outputs, config, desiredLoss);

    std::cout<<"MAIN: process initialized!"<<std::endl;

    m.start();
}