#include "evaluator.hpp"

Evaluator::Evaluator(Network net, torch::Tensor input, std::vector<int> outputs) : currentNet(std::move(net)), input(input), outputIds(outputs) {
    initialOutput = currentNet.forward(input);
}


double Evaluator::jitter(Network net, int layer, std::string id, std::pair<float, float> dist) {
    
    currentNet = std::move(net);
    torch::Tensor netOut;

    try {
        netOut = this->currentNet.forward(this->input);
    } catch(...) {
        throw std::out_of_range("Dimension mismatch: Input and network dimension mismatch");
    }

    std::vector<torch::Tensor> tensors;
    try {
        for(int& idx : outputIds) tensors.push_back(netOut.index({idx}));
    } catch(...) {
        throw std::out_of_range("Torch Index out of bounds - choose index values within the boundaries of the output tensor");
    }
    
    torch::Tensor out = torch::stack(tensors);

    torch::Tensor differences = torch::sub(out, initialOutput);
    double sum = 0;
    for(int i = 0; i < outputIds.size(); i++) sum+=differences.index({i}).item<double>();
    return sum/outputIds.size();
}

double Evaluator::sample(std::pair<float, float> dist) {
    double r = std::rand() / double(RAND_MAX);
    boost::math::beta_distribution d(dist.first, dist.second);
    double rand = boost::math::quantile(d, r);
    return config.get()->temperature * static_cast<float>(rand);
}

void Evaluator::updateDist(float lossUpdate, float randomChange) {
    std::pair<float, float>& orig = dist[uuid];
    const float update = lossUpdate * randomChange * lambda1;

    float one = orig.first+update;
    float two = orig.second-update;
    one += one<=1 ? 1 : one>=99 ? -1 : 0;
    two += two<=1 ? 1 : two>=99 ? -1 : 0;

    dist[uuid] = { one, two };
}


void Evaluator::updateDist(float lossUpdate, float prevAlpha, torch::Tensor weight, float randomChange) {
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