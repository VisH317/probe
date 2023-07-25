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
#include <boost/math/distributions.hpp>


/**
 * @brief Runs the main jitter process and calculates random number to measure a loss and an update
 * 
 */
class Evaluator {
    private: 

        /** input tensor */
        torch::Tensor input;

        /** initial calculated output to calculate new loss from */
        torch::Tensor initialOutput;
        
        /** Output ids to measure loss from */
        std::vector<int> outputIds;

        /** injected config to use for jitter */
        std::shared_ptr<Config> config;

    public:

        /**
         * @brief Construct a new Evaluator object
         * 
         * @param input input tensor
         * @param outputs output tensor ids
         * @param config injected config object
         */
        Evaluator(torch::Tensor input, std::vector<int> outputs, std::shared_ptr<Config> config);

        /**
         * @brief calls the global net manager to update network parameters
         * 
         */
        void updateNetParams();

        /**
         * @brief main jitter function
         * 
         * @param net network to run jitter on
         * @param layer current layer of id to measure
         * @param id current neuron ID to measure
         * @param dist distribution to poll random changes from
         * @param weight weight tensor of current neuron
         * @return std::tuple<double, double, torch::Tensor> 
         */
        std::tuple<double, double, torch::Tensor> jitter(Network& net, int layer, std::string id, std::pair<float, float> dist, torch::Tensor weight);

        /**
         * @brief create and sample a beta distribution based on neuron ID parameters
         * 
         * @param dist distribution parameter pair for alpha and beta
         * @return double 
         */
        double sample(std::pair<float, float> dist);

        /**
         * @brief Calculate update distribution at the start of a search
         * 
         * @param lossUpdate - calculated loss
         * @param randomChange - induced random change
         * @return double 
         */
        double updateDist(float lossUpdate, float randomChange);

        /**
         * @brief Calculate update distribution at the middle of a search
         * 
         * @param lossUpdate - calculated loss
         * @param randomChange - induced random change
         * @param neuronIds - ids of neurons processed
         * @param layerNum - current layer number of updated neuron
         * @param net - network to be altered
         * @return double 
         */
        double updateDist(float lossUpdate, float randomChange, std::vector<std::string> neuronIds, int layerNum, Network& net);

};


#endif