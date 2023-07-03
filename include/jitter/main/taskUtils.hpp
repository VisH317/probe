#ifndef __TASK_UTILS_H__
#define __TASK_UTILS_H__

#include <string>

// states for each input neuron, holds number of times iterated over, efficacy measure, and if currently being worked on
struct NeuronState {
    std::string id;
    int timesIterated;
    bool current;
    int maxDepth; // measure of efficacy according to how deep the search went
};

struct ProcessState {
    int pidx;
    int currentSearchlayer;
    std::string currentState;
};

#endif