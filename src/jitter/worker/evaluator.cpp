#include "evaluator.hpp"
using namespace boost::math;

Evaluator::Evaluator(torch::Tensor input, std::vector<int> outputs, std::shared_ptr<Config> config) : input(input), outputIds(outputs), config(config) {}


std::tuple<double, double, torch::Tensor> Evaluator::jitter(Network& currentNet, int layer, std::string id, std::pair<float, float> dist, torch::Tensor weight) {
    
    std::cout<<"EVALUATOR: jitter: "<<std::endl;
    torch::Tensor initialOutput = currentNet.forward(this->input);
    torch::Tensor netOut;
    torch::Tensor update;
    double randomChange;

    try {
        auto info = currentNet.getLayer(layer)->getNeuron(id);
        update = torch::zeros_like(std::get<1>(info));
        randomChange = sample(dist);
        update+=randomChange;
        currentNet.getLayer(layer)->changeNeuronWeight(id, update);
        netOut = currentNet.forward(this->input);
    } catch(...) {
        throw std::out_of_range("Dimension mismatch: Input and network dimension mismatch");
    }

    // std::vector<torch::Tensor> tensors;
    // try {
    //     for(int& idx : outputIds) tensors.push_back(netOut.index({idx}));
    // } catch(...) {
    //     throw std::out_of_range("Torch Index out of bounds - choose index values within the boundaries of the output tensor");
    // }
    
    // torch::Tensor out = torch::stack(tensors);

    torch::Tensor differences = torch::sub(netOut, initialOutput);
    // std::cout<<"DIFF: "<<netOut<<", INITIAL: "<<initialOutput<<", DUFFERECES: "<<differences<<std::endl;
    double sum = 0;
    for(int i = 0; i < outputIds.size(); i++) {
        torch::Tensor diff = differences.index({i});
        sum += diff.item<double>();
    }
    return {sum/outputIds.size(), randomChange, update};
}

double Evaluator::sample(std::pair<float, float> dist) {
    double r = std::rand() / double(RAND_MAX);
    boost::math::beta_distribution<double> distr(50.0, 50.0);
    double randFromDist = boost::math::quantile(distr, r);
    return randFromDist * this->config->temperature;
}

double Evaluator::updateDist(float lossUpdate, float randomChange) {
    const float update = lossUpdate * randomChange * this->config->lambda1;
    return update;
}


double Evaluator::updateDist(float lossUpdate, float randomChange, std::vector<std::string> neuronIds, int layerNum, Network& net) {

    float weights = 1;

    for(int i = 0; i < layerNum; i++) {
        std::tuple<int, torch::Tensor, torch::Tensor> weight = net.getLayer(i)->getNeuron(neuronIds[i]);
        torch::Tensor meanTen = torch::mean(std::get<1>(weight), 0);
        weights*=meanTen.item<float>();
    } 

    const float update = this->config->lambda2 * weights * lossUpdate * randomChange;
    return update;
}