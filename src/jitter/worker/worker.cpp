#include "worker.hpp"


Worker::Worker(std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs) : config(config), responseQueue(responseQueue), netIteration(0) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage m) {
    this->netIteration = m.netIteration;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m.net.getLayer(0)->getNeuron(m.neuronID);

    double randomChange = evaluator.sample(m.dist);
}