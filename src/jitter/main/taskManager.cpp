#include "taskManager.hpp"

TaskManager::TaskManager(int numProcesses, Layer* inputLayer) : numProcesses(numProcesses), inputLayer(inputLayer), currentSearchIndex(0) {
    for(int i=0; i<std::min(numProcesses, inputLayer->getLength()); i++) {
        std::string id = std::get<0>(inputLayer->getNeuron(i));
        processes.push_back(ProcessState{i, 0, id, id});
    }
    
    for(int i=0;i<inputLayer->getLength();i++) {
        std::string id = std::get<0>(inputLayer->getNeuron(i));
        neurons.push_back(NeuronState{id, numProcesses>i ? 1 : 0, numProcesses>i, 0 });
    }
}