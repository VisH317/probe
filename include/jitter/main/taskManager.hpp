#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include "taskUtils.hpp"
#include "layer.hpp"

// current task distribution algorithm based on round robin

class TaskManager {
    private: 
        Layer* inputLayer;
        const int numProcesses;
        int currentSearchIndex;

        NeuronState[] neurons;
        ProcessState[] processes;

    public:
        TaskManager(int numProcesses, Layer* inputLayer);

        void createProcess();
        void setNewProcessState();
        void processFinished();

};

#endif