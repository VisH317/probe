#ifndef __WORKER_H__
#define __WORKER_H__

#include <memory>
#include <utility>
#include <string>
#include <torch/torch.h>
#include "responseQueue.hpp"
#include "evaluator.hpp"
#include "workerQueue.hpp"
#include "config.hpp"
#include "network.hpp"
#include "responseMessages.hpp"



class Worker {
    private:

        int id;

        std::shared_ptr<Config> config;

        std::shared_ptr<ResponseQueue> responseQueue;

        WorkerQueue queue;

        Evaluator evaluator;

        Network net;

        int netIteration;

    public:
        Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs);
        ~Worker() = default;

        // queue
        void addTask(Message m);

        // main process
        void main();
        void start();

        // case functions
        void startJitter(StartSearchMessage m);
        void updateJitter(UpdateSearchMessage m);

        // utility functions
        void addNewNeuronTasks(int layernum);
        void pushResponse(ResponseMessage m);

};


#endif