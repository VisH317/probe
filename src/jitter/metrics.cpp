#include "metrics.hpp"

void Metrics::addDist(std::string uuid) {
    dist[uuid] = { 50.0, 50.0 };
}


void Metrics::updateDist(std::string uuid, float lossUpdate, float randomChange) {
    std::pair<float, float>& orig = dist[uuid];
    const float update = lossUpdate * randomChange * lambda1;

    float one = orig.first+update;
    float two = orig.second-update;
    one += one<=1 ? 1 : one>=99 ? -1 : 0;
    two += two<=1 ? 1 : two>=99 ? -1 : 0;

    dist[uuid] = { one, two };
}


void Metrics::updateDist(std::string uuid, float lossUpdate, float prevAlpha, torch::Tensor weight, float randomChange) {
    torch::Tensor avgTen = torch::mean(weight, 0);
    const float avg = avgTen.item<float>();

    const float update = lambda2 * avg * lossUpdate * randomChange;
    std::pair<float, float>& orig = dist[uuid];

    float one = orig.first+update;
    float two = orig.second-update;
    one += one<=1 ? 1 : one>=99 ? -1 : 0;
    two += two<=1 ? 1 : two>=99 ? -1 : 0;

    dist[uuid] = { one, two };
}


float Metrics::sample(std::string uuid) {
    const std::pair<float, float>& params = dist[uuid];

    double r = std::rand() / double(RAND_MAX);
    boost::math::beta_distribution d(params.first, params.second);
    double rand = boost::math::quantile(d, r);
    return temperature * static_cast<float>(rand);
}