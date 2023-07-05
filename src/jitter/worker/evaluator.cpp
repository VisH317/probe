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