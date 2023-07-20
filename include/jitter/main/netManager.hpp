#ifndef __NET_MANAGER_H__
#define __NET_MANAGER_H__

#include <memory>
#include <utility>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <iostream>
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


// util function for casting unique_ptr to derived (sadge that std doesnt have this)
// link: https://stackoverflow.com/questions/26377430/how-to-perform-a-dynamic-cast-with-a-unique-ptr
template <typename To, typename From, typename Deleter>
std::unique_ptr<To, Deleter> dynamic_unique_cast(std::unique_ptr<From, Deleter>&& p);


class NetManager {
    private: 
        // Network mainNet;

        std::vector<Worker> workers;

        std::shared_ptr<Config> config;

        std::shared_ptr<ResponseQueue> responseQueue;

        std::shared_ptr<Net> net;

        torch::Tensor input;
        
        std::vector<int> outputs;

        double desiredLoss;

        std::shared_ptr<std::thread> thread;

        // state vars

        // int currentNetIteration;
        // std::map<std::pair<std::string, int>, std::pair<float, float>> dist;
        // std::mutex distM;

        // dependencies

        TaskManager tasks;

        LossManager losses;


    public:
        NetManager(Network& net, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config, double desiredLoss);
        ~NetManager();

        void start();

        void process();

        // dist function
        void updateDist(std::unique_ptr<ResponseUpdateMessage> m);

        void createNewSearch(std::unique_ptr<ResponseDoneMessage> m);


};


#endif