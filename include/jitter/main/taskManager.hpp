#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#include "taskUtils.hpp"
#include "layer.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
#include <stdexcept>


/**
 * @brief Round robin task distributor to assign search tasks to workers (one task for each input neuron)
 * 
 */
class TaskManager {
    private: 
        /** input layer to pull input neuron tasks from */
        Layer inputLayer;
        /** Number of created worker processes */
        const int numProcesses;
        /** static current search index to increment and assign new tasks on each setNewProcessState */
        int currentSearchIndex;

        /** List of neuron states */
        std::vector<NeuronState> neurons;
        /** List of states for each process (holds current working task) */
        std::vector<ProcessState> processes;

    public:
        /**
         * @brief Construct a new Task Manager object
         * 
         * @param numProcesses - number of processes to assign tasks to 
         * @param inputLayer - input layer to process to find input tasks
         */
        TaskManager(int numProcesses, Layer inputLayer);

        /**
         * @brief Get a process info object
         * 
         * @param id (int) id of process
         * @return ProcessState
         */
        ProcessState getProcessInfo(int id);

        /**
         * @brief Get a neuron info object from list of input neurons
         * 
         * @param id (int) id of neuron in the input layer
         * @return NeuronState 
         */
        NeuronState getNeuronInfo(std::string id);

        /**
         * @brief Set a new process state
         * 
         * @param pid (int) worker id
         * @return std::string 
         */
        std::string setNewProcessState(int pid);

        /**
         * @brief Update search status of a worker process to latest layer
         * 
         * @param pid (int) worker id
         * @param layer (Layer) layer that it is currently working on
         */
        void setNewProcessSearch(int pid, int layer);

};

#endif