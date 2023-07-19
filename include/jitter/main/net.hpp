#ifndef __NET_H__
#define __NET_H__

#include <map>
#include <mutex>
#include <iostream>
#include <torch/torch.h>
#include "network.hpp"

class Net {
    private:

        int currentNetIteration;
        
        std::map<std::pair<std::string, int>, std::pair<float, float>> dist;
        std::mutex distM;
        
        Network mainNet;
        std::mutex m;

    public:

        Net(Network& n) : mainNet(n), currentNetIteration(0) {};

        std::tuple<Network, std::pair<float, float>, int> getCurrentInfo(int layerNum, std::string uuid);

        // dist
        void updateDist(std::string uuid, int layerNum, double update, torch::Tensor up);

};


#endif