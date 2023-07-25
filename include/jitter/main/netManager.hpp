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

/**
 * @brief main jitter manager class, holds the main process from each worker and delegates tasks
 * 
 */
class NetManager {
    private: 
        // Network mainNet;

        /** Vector of worker thread objects */
        std::vector<Worker> workers;

        /** Injected config shared among all workers and net manager */
        std::shared_ptr<Config> config;

        /** Injected response queue to receive messages from workers */
        std::shared_ptr<ResponseQueue> responseQueue;

        /** Injected net manager object to manage a global network state */
        std::shared_ptr<Net> net;

        /** Input tensor to run the jitter algo on */
        torch::Tensor input;
        
        /** list of outputs neuron indexes to consider when measuring jitter loss */
        std::vector<int> outputs;

        /** Desired loss to iterate to [DEPRECATED, will be replaced with number of iterations to run on] */
        double desiredLoss;

        /** Main process thread */
        std::shared_ptr<std::thread> thread;

        /** Task manager object */
        TaskManager tasks;

        /** Loss manager object */
        LossManager losses;


    public:
        /**
         * @brief Construct a new Jitter algorithm object
         * 
         * @param net - (Network) network to run jitter algorithm on
         * @param numWorkers - (int) number of workers to run searches
         * @param input - (torch::Tensor) input to jitter on
         * @param outputs - (std::vector<int>) outputs to measure jitter changes
         * @param config - (Config) config of jitter algorithm + hyperparameters
         * @param desiredLoss - (double) desired loss to optimize to
         */
        NetManager(Network& net, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config, double desiredLoss);
        ~NetManager();

        /**
         * @brief start the main process thread to parse response queue messages and update dist / network params
         * 
         */
        void start();

        /**
         * @brief Main process to receive response queue messages to assign new searches and parse network updates
         * 
         */
        void process();

        /**
         * @brief handler for update distribution message
         * 
         * @param m - (std::unique_ptr<ResponseUpdateMessage>) message to receive update info
         */
        void updateDist(std::unique_ptr<ResponseUpdateMessage> m);

        /**
         * @brief Done message handler object in main process
         * 
         * @param m - (std::unique_ptr<ResponseDoneMessage>) message to receive done info and assign a new neuron search to a worker
         */
        void createNewSearch(std::unique_ptr<ResponseDoneMessage> m);


};


#endif