#include "worker.hpp"


Worker::Worker(std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs) : config(config), responseQueue(responseQueue) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage m) {

}