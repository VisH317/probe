#include "metrics.hpp"

void Metrics::addDist(std::string uuid) {
    dist[uuid] = { 50.0, 50.0 };
}

void Metrics::updateDist(std::string uuid, float lossUpdate, float randomChange) {
    std::pair<float, float>& orig = dist[uuid];
    const float finalLoss = lossUpdate * randomChange * lambda1;
    dist[uuid] = { orig.first+finalLoss, orig.second-finalLoss };
}

void Metrics::updateDist(std::string uuid, float lossUpdate, float prevAlpha, torch::Tensor weight, float randomChange) {
    torch::Tensor avgTen = torch::mean(weight, 0);
    const float avg = avgTen.item<float>();

    const float update = lambda2 * avg * prevAlpha * randomChange;
    std::pair<float, float>& orig = dist[uuid];
    dist[uuid] = { orig.first+update, orig.second-update };
}