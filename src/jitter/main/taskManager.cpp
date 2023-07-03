#include "taskManager.hpp"

TaskManager::TaskManager(int numProcesses, Layer* inputLayer) : numProcesses(numProcesses), inputLayer(inputLayer), currentSearchIndex(0) {
    for(int i=0; i<numProcesses; i++) {
        processes.push_back(ProcessState{})
    }
}