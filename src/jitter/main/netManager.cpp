#include "netManager.hpp"

NetManager::NetManager(Network& network, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config) : config(std::make_shared<Config>(std::move(config))), outputs(std::move(outputs)), input(std::move(input)), tasks(numWorkers, *network.getLayer(0)) {
    
    responseQueue = std::make_shared<ResponseQueue>();
    
    for(int i=0;i<numWorkers;i++) {
        Worker worker(i, this->config, responseQueue, input, outputs, net);
        workers.push_back(worker);
    }
}

void NetManager::start() {
    for(int i=0;i<workers.size(); i++) workers[i].start();
    thread = new std::thread(&NetManager::process, this);
}

void NetManager::process() {
    int iterations = 0;

    while(true) {
        if(responseQueue->size()==0) continue;

        std::unique_ptr<ResponseMessage> m = responseQueue->pop();

        switch(m->getType()) {
            case ResponseType::RES_UPDATE:
                updateDist(dynamic_cast<ResponseUpdateMessage*>(m.get()));
                break;
            case ResponseType::RES_DONE:
                createNewSearch(dynamic_cast<ResponseDoneMessage*>(m.get()));
                break;
            default: break;
        }
    }
}

void NetManager::updateDist(ResponseUpdateMessage* m) {
    bool shouldUpdate = losses.shouldUpdate(m->loss, m->netIteration);
    if(!shouldUpdate) {
        RejectedMessage rejected{m->uuid, m->layerNum};
        workers[m->workerId].addTask(rejected);
        return;
    }

    net->updateDist(m->uuid.back(), m->layerNum, m->update, m->updateTen);
    
    ValidMessage valid{m->uuid, m->layerNum};
    workers[m->workerId].addTask(valid);
}

NetManager::~NetManager() {
    for(Worker& worker : workers) {
        worker.addTask(StopMessage{});
    }
    workers.clear();
    thread->join();
    delete thread;
}

void NetManager::createNewSearch(ResponseDoneMessage* m) {
    std::string id = tasks.setNewProcessState(m->workerId);
    StartSearchMessage message{id};
    workers[m->workerId].addTask(message);
}