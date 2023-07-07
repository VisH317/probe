#include "lossManager.hpp"

bool LossManager::shouldUpdate(double loss, int netIteration) {
    if(lossMap.find(netIteration)==lossMap.end()) {
        lossMap.insert({ netIteration, loss });
        return true;
    }

    if(lossMap[netIteration]>=loss) {
        lossMap[netIteration] = loss;
        return true;
    }

    return false;
    
}