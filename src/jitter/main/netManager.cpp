#include "netManager.hpp"

NetManager::NetManager(int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config) : config(std::make_shared<Config>(std::move(config))), outputs(std::move(outputs)), input(std::move(input)) {
    
    responseQueue = std::make_shared<ResponseQueue>(ResponseQueue());
    
    for(int i=0;i<numWorkers;i++) {
        workers.push_back(Worker(i, this->config, responseQueue, input, outputs));
    }
}

void NetManager::process() {
    int iterations = 0;

    while(true) {
        if(responseQueue->size()==0) continue;

        std::unique_ptr<ResponseMessage> m = responseQueue->pop();

        switch(m.getType()) {
            case 
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

    setDist(m->uuid.back(), m->layerNum, m->update);
    ValidMessage valid{m->uuid, m->layerNum};
    workers[m->workerId].addTask(valid);
}

void NetManager::setDist(std::string uuid, int layerNum, double update) {
    if(dist.find({ uuid, layerNum })==dist.end()) dist[{ uuid, layerNum }] = { 50.0+update, 50.0-update };
    std::pair<float, float> orig = dist[{ uuid, layerNum }];
    float x = orig.first+update>=99 ? orig.first+update-1 : orig.first+update<=1 ? orig.first+update+1 : orig.first+update; 
    float y = orig.second-update>=99 ? orig.second-update-1 : orig.second-update<=1 ? orig.second-update-1 : orig.second-update; 

    dist[{ uuid, layerNum }] = { x, y };
    
}