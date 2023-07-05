#include "evaluator.hpp"

double Evaluator::jitter(int layer, std::string id, std::pair<float, float> dist) {

}

double Evaluator::sample(std::pair<float, float> dist) {
    double r = std::rand() / double(RAND_MAX);
    boost::math::beta_distribution d(params.first, params.second);
    double rand = boost::math::quantile(d, r);
    return temperature * static_cast<float>(rand);
}