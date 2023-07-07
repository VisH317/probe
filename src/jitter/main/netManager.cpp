#include "netManager.hpp"

NetManager::NetManager(int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config) : config(std::make_shared<Config>(std::move(config))), outputs(std::move(outputs)), input(std::move(input)) {
    
    responseQueue = std::make_shared<ResponseQueue>(ResponseQueue());
    
    for(int i=0;i<numWorkers;i++) {
        workers.push_back(Worker(i, this->config, responseQueue, input, outputs));
    }
}

