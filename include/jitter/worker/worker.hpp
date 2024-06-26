#ifndef __WORKER_H__
#define __WORKER_H__

#include <memory>
#include <utility>
#include <string>
#include <torch/torch.h>
#include <thread>
#include <map>
#include <optional>
#include <tuple>
#include <iostream>
#include "responseQueue.hpp"
#include "evaluator.hpp"
#include "workerQueue.hpp"
#include "config.hpp"
#include "network.hpp"
#include "responseMessages.hpp"
#include "net.hpp"
#include "boost/math/distributions.hpp"


class Worker {
    private:
        
        int id;

        std::shared_ptr<Config> config;

        std::shared_ptr<std::thread> thread;

        std::map<std::vector<std::string>, std::optional<bool>> responses;

        std::shared_ptr<ResponseQueue> responseQueue;

        std::shared_ptr<Net> netManager;

        WorkerQueue queue;

        Evaluator evaluator;

        int netIteration;

    public:
        Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs, std::shared_ptr<Net> net);
        ~Worker();
        // Worker(Worker&&) {};
        // Worker(const Worker&) = delete;

        // queue
        void addTask(std::shared_ptr<Message> m);

        // main process
        void main();
        void start();

        // case functions
        void startJitter(std::shared_ptr<StartSearchMessage> m);
        void updateJitter(std::shared_ptr<UpdateSearchMessage> m);

        // validation functions
        void setValid(std::shared_ptr<ValidMessage> m);
        void setReject(std::shared_ptr<RejectedMessage> m);

        // utility functions
        void addNewNeuronTasks(int layernum);
        // void pushResponse(std::shared_ptr<ResponseMessage> m);

};


#endif