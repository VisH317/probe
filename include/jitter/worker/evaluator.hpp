#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <thread>
#include <future>
#include <string>
#include <memory>
#include <utility>
#include <tuple>
#include <iostream>
#include <torch/torch.h>
#include "network.hpp"
#include "config.hpp"
#include <boost/math/distributions/beta.hpp>

class Evaluator {
    private: 

        torch::Tensor input;

        torch::Tensor initialOutput;
        
        std::vector<int> outputIds;

        std::shared_ptr<Config> config;

    public:
        Evaluator(torch::Tensor input, std::vector<int> outputs);

        void updateNetParams();

        std::tuple<double, double, torch::Tensor> jitter(Network& net, int layer, std::string id, std::pair<float, float> dist, torch::Tensor weight);

        double sample(std::pair<float, float> dist);

        double updateDist(float lossUpdate, float randomChange);

        double updateDist(float lossUpdate, float randomChange, std::vector<std::string> neuronIds, int layerNum, Network& net);

};


#endif