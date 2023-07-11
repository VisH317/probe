#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__

#include <memory>
#include <utility>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <tuple>
#include <torch/torch.h>
#include "network.hpp"
#include "worker.hpp"
#include "responseQueue.hpp"
#include "taskManager.hpp"
#include "taskUtils.hpp"
#include "config.hpp"
#include "responseMessages.hpp"
#include "lossManager.hpp"
#include "message.hpp"
#include "net.hpp"


class NetManager {
    private: 
        // Network mainNet;

        std::vector<Worker> workers;

        std::shared_ptr<Config> config;

        std::shared_ptr<ResponseQueue> responseQueue;

        std::shared_ptr<Net> net;

        torch::Tensor input;
        
        std::vector<int> outputs;

        std::thread thread;

        // state vars

        // int currentNetIteration;
        // std::map<std::pair<std::string, int>, std::pair<float, float>> dist;
        // std::mutex distM;

        // dependencies

        TaskManager tasks;

        LossManager losses;


    public:
        NetManager(Network& net, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config);
        ~NetManager();

        void start();

        void process();

        // dist function
        void updateDist(ResponseUpdateMessage* m);

        void createNewSearch(ResponseDoneMessage* m);


};


#endif