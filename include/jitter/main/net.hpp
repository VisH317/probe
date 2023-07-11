#ifndef __NET_H__
#define __NET_H__

#include <map>
#include "network.hpp"

class Net {
    public:

        int currentNetIteration;
        
        std::map<std::pair<std::string, int>, std::pair<float, float>> dist;
        
        Network mainNet;
        Net(Network& n) : mainNet(n), currentNetIteration(0) {};

        std::tuple<Network, std::pair<float, float>, int> getCurrentInfo(int layerNum, std::string uuid);
}


#endif