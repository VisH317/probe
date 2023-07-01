#ifndef __METRICS_H__
#define __METRICS_H__

#include <map>
#include <string>
#include <utility>
#include <climits>
#include <cstdlib>
#include <torch/torch.h>
#include <boost/math/distributions/beta.hpp>


// utility metrics for the jitter algorithm - includes hashmap of all neurons and their respective distributions, new distribution calculation, distribution shifting, change maker and evaluator per neuron

class Metrics {
    private:
        /** map of uuids to beta distribution parameters */
        std::map<std::string, std::pair<float, float>> dist; // will switch to concurrent hashmap intel TBB when multithreaded format comes

        /** distribution shift scaler on same neuron */
        float lambda1;

        /** distribution shift scaler between neurons */
        float lambda2;

        /** scalar value - maximum change magnitude */
        float temperature;


    public:

        Metrics(float lambda1, float lambda2, float temperature) : lambda1(lambda1), lambda2(lambda2), temperature(temperature) {};

        /**
         * @brief creates a new neuron instance in the hashmap with the default alpha/beta parameters (50/50)
         * 
         * @param uuid id of the neuron to be added and stored
         */
        void addDist(std::string uuid);

        void updateDist(std::string uuid, float lossUpdate, float randomChange);

        void updateDist(std::string uuid, float lossUpdate, float prevAlpha, torch::Tensor weight, float randomChange);

        float sample(std::string uuid); // might need to use torch multiprocessing

};

// things to include in main loop function: tree search, calling input vs nested neuron, finding weight values, evaluating loss, pushing network changes

#endif