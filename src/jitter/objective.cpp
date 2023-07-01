#include "objective.hpp"


Objective::Objective(Network* net, std::vector<int>& idxs, torch::Tensor input, double desiredValue) : net(net), idxs(idxs), input(input), desiredValue(desiredValue) {
    evaluateOutput()
}

void Objective::evaluteOutput() {

    try {
        torch::Tensor netOut = this->net->forward(this->input);
    } catch(...) {
        throw std::out_of_range("Dimension mismatch: Input and network dimension mismatch");
    }

    std::vector<torch::Tensor> tensors;
    try {
        for(int& idx : idxs) {
            tensors.push_back(netOut.index({idx}));
        }
    } catch(...) {
        throw std::out_of_range("Torch Index out of bounds - choose index values within the boundaries of the output tensor")
    }
    
    torch::Tensor out = torch::stack(tensors);
    outputs.push_back(out);
    if(outputs.size()>1) losses.push_back(computeAvgLoss(out, outputs[0]));
}


std::pair<double, bool> Objective::loss() {
    evaluateOutput();
    double out = losses[losses.size()-1];
    return { out, std::abs(out)>(losses.size()<=2 ? 0 : std::abs(losses[losses.size()-2])) }
}

double Objective::computeAvgLoss(torch::Tensor in, torch::Tensor start) {
    torch::Tensor differences = start - in;
    return torch::mean(differences).item();
}

double Objective::getDiff() {
    return desiredValue - losses[losses.size()-1];
}

void Objective::reset() {
    outputs.clear();
}