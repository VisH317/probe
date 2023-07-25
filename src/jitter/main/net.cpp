#include "net.hpp"

std::tuple<Network, std::pair<float, float>, int> Net::getCurrentInfo(int layerNum, std::string uuid) {
    std::lock_guard<std::mutex> lock(m);
    if(dist.find({ uuid, layerNum }) == dist.end()) dist[{ uuid, layerNum }] = { 50.0, 50.0 };
    std::pair<float, float> d = dist[{uuid, layerNum}];
    std::cout<<"NETWORK MANAGER: Getting current info..."<<layerNum<<std::endl;
    // std::lock_guard<std::mutex> lock2(m);
    return { mainNet, d, currentNetIteration };
}

void Net::updateDist(std::string uuid, int layerNum, double update, torch::Tensor up) {
    std::lock_guard<std::mutex> lock(m);
    mainNet.getLayer(layerNum)->changeNeuronWeight(uuid, up);

    // std::lock_guard<std::mutex> distLock(distM);
    if(dist.find({ uuid, layerNum })==dist.end()) dist[{ uuid, layerNum }] = { 50.0, 50.0 };
    std::pair<float, float> orig = dist[{ uuid, layerNum }];
    float x = orig.first+update>=99 ? orig.first+update-1 : orig.first+update<=1 ? orig.first+update+1 : orig.first+update; 
    float y = orig.second-update>=99 ? orig.second-update-1 : orig.second-update<=1 ? orig.second-update-1 : orig.second-update; 

    dist[{ uuid, layerNum }] = { x, y };
    currentNetIteration++;
}