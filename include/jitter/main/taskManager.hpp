#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include "taskUtils.hpp"
#include "layer.hpp"
#include <vector>

// current task distribution algorithm based on round robin

class TaskManager {
    private: 
        Layer* inputLayer;
        const int numProcesses;
        int currentSearchIndex;

        std::vector<NeuronState> neurons;
        std::vector<ProcessState> processes;

    public:
        TaskManager(int numProcesses, Layer* inputLayer);

        ProcessState getProcessInfo();
        NeuronState getNeuronInfo();

        void setNewProcessState();
        void setNewProcessSearch();
        void processFinished();

};

#endif