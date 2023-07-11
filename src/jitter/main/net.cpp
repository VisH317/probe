#include "net.hpp"

std::tuple<Network, std::pair<float, float>, int> Net::getCurrentInfo(int layerNum, std::string uuid) {
    std::pair<float, float> d = dist[{uuid, layerNum}];
    return { mainNet, d, currentNetIteration };
}