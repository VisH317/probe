#ifndef __WORKER_H__
#define __WORKER_H__

#include <memory>
#include "responseQueue.hpp"
#include "evaluator.hpp"
#include "workerQueue.hpp"
#include "config.hpp"
#include "network.hpp"
#include "responseMessages.hpp"



class Worker {
    private:

        std::shared_ptr<Config> config;

        std::shared_ptr<ResponseQueue> responseQueue;

        WorkerQueue queue;

        Evaluator evaluator;

        Network net;


    public:
        Worker(std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue);
        ~Worker() = default;

        // queue
        void addTask(Message m);

        // main process
        void main();
        void start();

        // case functions
        void startJitter();
        void updateJitter();

        // utility functions
        void addNewNeuronTasks(int layernum);
        void pushResponse(ResponseMessage m);

};


#endif