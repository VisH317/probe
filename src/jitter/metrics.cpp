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

    float one = orig.first+update;
    float two = orig.second-update;
    one += one<=1 ? 1 : one>=99 ? -1 : 0;
    two += two<=1 ? 1 : two>=99 ? -1 : 0;

    dist[uuid] = { one, orig.second-update };
}

void Metrics::sample(std::string uuid) {
    const std::pair<float, float>& params = dist[uuid];
    boost::math::beta_distribution d(params.first, params.second);
    double rand = d.quantile();
    return temperature * rand;
}