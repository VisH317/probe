#ifndef __TASK_UTILS_H__
#define __TASK_UTILS_H__

#include <string>

// states for each input neuron, holds number of times iterated over, efficacy measure, and if currently being worked on
struct NeuronState {
    /** id of neuron */
    std::string id;

    /** Number of worker search iterations completed on the neuron */
    int timesIterated;

    /** if the neuron is currently being iterated on */
    bool current;

    /** Max depth achieved when searching a neuron (for task managing) */
    int maxDepth;
};

struct ProcessState {
    /** Process index */
    int pidx;

    /** Current layer of search for the process */
    int currentSearchlayer;

    /** Current state neuron ID for the process */
    std::string currentState;
};

#endif