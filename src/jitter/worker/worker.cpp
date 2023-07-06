#include "worker.hpp"


Worker::Worker(std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue) : config(config), responseQueue(responseQueue) {
    evaluator = Evaluator()
}