#ifndef __NET_H__
#define __NET_H__

#include <map>
#include <mutex>
#include <iostream>
#include <torch/torch.h>
#include "network.hpp"

/**
 * @brief Injected class to manage a global network state and distribution of weight change selections per neuron
 * 
 */
class Net {
    private:

        /** Current net iteration in the jitter algorithm */
        int currentNetIteration;
        
        /** Distribution mapping of layer and neuron ID pair to a beta distribution alpha and beta parameter pair */
        std::map<std::pair<std::string, int>, std::pair<float, float>> dist;
        /** mutex to lock to distribution reading in multithreaded environment */
        std::mutex distM;
        
        /** Main network to be altered on updates */
        Network mainNet;
        /** Mutex to control network alterations */
        std::mutex m;

    public:

        /**
         * @brief Construct a new Net Manager object for jitter algo stability checking
         * 
         * @param n - network to be observed and altered
         */
        Net(Network& n) : mainNet(n), currentNetIteration(0) {};

        /**
         * @brief Get the Current Info from a neuron in the network object
         * 
         * @param layerNum - layer that the desired neuron is in
         * @param uuid - ID of the neuron whose information is required
         * @return std::tuple<Network, std::pair<float, float>, int> - returns the network, a pair of the distribution for the specific neuron, and the current net iteration
         */
        std::tuple<Network, std::pair<float, float>, int> getCurrentInfo(int layerNum, std::string uuid);

        /**
         * @brief Updates the dist object for a specific neuron and alters the distribution parameters + network based on the update
         * 
         * @param uuid - (std::string) uuid of the neuron to be updated
         * @param layerNum - (int) layer of the neuron
         * @param update - (double) distribution update difference value
         * @param up - (torch::Tensor) update tensor
         */
        void updateDist(std::string uuid, int layerNum, double update, torch::Tensor up);

};


#endif