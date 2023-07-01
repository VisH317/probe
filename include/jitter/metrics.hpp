#ifndef __METRICS_H__
#define __METRICS_H__

#include <map>
#include <string>
#include <utility>

// utility metrics for the jitter algorithm - includes hashmap of all neurons and their respective distributions, new distribution calculation, change maker and evaluator per neuron

class Metrics {
    private:

    std::map<std::string, std::pair<float, float>> dist;

    

    public:
        Metrics()
}

#endif