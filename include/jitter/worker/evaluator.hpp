#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <thread>
#include <future>
#include <string>
#include "network.hpp"

class Evaluator {
    private: 
        Network currentNet;
        
        int currentNetIteration;

        torch::Tensor input;
        
        std::vector<int> outputs;

    public:
        Evaluator(Network net, torch::Tensor input, std::vector<int> outputs) : currentNet(net), input(input), outputs(outputs) {};

        void updateNetParams();

        std::tuple<int, std::string, torch::Tensor> jitter(int layer, std::string id);

}


#endif