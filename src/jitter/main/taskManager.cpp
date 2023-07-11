#include "taskManager.hpp"

TaskManager::TaskManager(int numProcesses, Layer inputLayer) : numProcesses(numProcesses), inputLayer(inputLayer), currentSearchIndex(0) {
    for(int i=0; i<std::min(numProcesses, inputLayer->getLength()); i++) {
        std::string id = std::get<0>(inputLayer->getNeuron(i));
        processes.push_back(ProcessState{i, 0, id});
    }
    
    for(int i=0;i<inputLayer->getLength();i++) {
        std::string id = std::get<0>(inputLayer->getNeuron(i));
        neurons.push_back(NeuronState{id, numProcesses>i ? 1 : 0, numProcesses>i, 0 });
    }
}

std::string TaskManager::setNewProcessState(int pid) {
    std::string id = std::get<0>(inputLayer->getNeuron(currentSearchIndex));
    processes[pid].currentState = id;
    processes[pid].currentSearchlayer = 0;
    currentSearchIndex++;
    if(currentSearchIndex>=inputLayer->getLength()) currentSearchIndex=0;
    return id;
}

void TaskManager::setNewProcessSearch(int pid, int layer) {
    processes[pid].currentSearchlayer = layer;
}

ProcessState TaskManager::getProcessInfo(int id) {
    return processes[id];
}

NeuronState TaskManager::getNeuronInfo(std::string id) {
    for(int i = 0; i < neurons.size(); i++) {
        if(std::strcmp(neurons[i].id.c_str(), id.c_str())==0) return neurons[i];
    }
    throw std::runtime_error("Get neuron info failed");    
}