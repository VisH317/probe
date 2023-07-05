#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <thread>
#include <future>
#include <string>
#include "network.hpp"
#include <boost/math/distributions/beta.hpp>

class Evaluator {
    private: 
        Network currentNet;
        
        int currentNetIteration;

        torch::Tensor input;
        
        std::vector<int> outputs;

    public:
        Evaluator(Network net, torch::Tensor input, std::vector<int> outputs) : currentNet(net), input(input), outputs(outputs) {};

        void updateNetParams();

        double jitter(int layer, std::string id, std::pair<float, float> dist);

        double sample(std::pair<float, float> dist);

}


#endif