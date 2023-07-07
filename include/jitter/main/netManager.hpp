#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__

#include <memory>
#include <utility>
#include <vector>
#include <thread>
#include <torch/torch.h>
#include "network.hpp"
#include "worker.hpp"
#include "responseQueue.hpp"
#include "taskManager.hpp"
#include "taskUtils.hpp"
#include "config.hpp"



class NetManager {
    private: 
        Network mainNet;

        std::vector<Worker> workers;

        std::shared_ptr<Config> config;

        std::shared_ptr<ResponseQueue> responseQueue;

        torch::Tensor input;
        
        std::vector<int> outputs;

        std::thread process;


    public:
        NetManager(int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config);
        ~NetManager();

        void start();

        void process();


};


#endif