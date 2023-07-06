#include "worker.hpp"


Worker::Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs) : config(config), responseQueue(responseQueue), netIteration(0), id(id) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage m) {
    this->netIteration = m.netIteration;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m.net.getLayer(0)->getNeuron(m.neuronID);

    std::pair<double, double> out = evaluator.jitter(m.net, 0, m.neuronID, m.dist, std::get<1>(neuronInfo));

    double update = evaluator.updateDist(out.first, out.second);

    ResponseUpdateMessage res{this->id, m.neuronID, 0, out.first, out.second, update};

    responseQueue->push(res);
}