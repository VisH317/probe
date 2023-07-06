#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <tuple>
#include "network.hpp"

enum MessageType {
    START,
    UPDATE,
    STOP,
};

// design question: should new networks be passed through messages each time or can the message calling be centralized to one manager
// message passing: need to control move semantics and creation/deletion of other parameters
// // important to have separate copies to prevent updates being run while a worker is running a forward on the thing
// centralized manager has to manage both and both have similar memory requirements, since each worker still has to hold their own copy of the network

struct Message {
    int pid;
    virtual MessageType getType();
    virtual std::vector getInfo();
};

struct StartSearchMessage : public virtual Message {
    SSMInfo content;
    MessageType getType() override { return START; };
};

struct UpdateSearchMessage : Message {
    USMInfo content;
    MessageType getType() override { return UPDATE; };
};


struct StopMessage : Message {
    StopInfo content;
    MessageType getType() override { return STOP; };
};

// info structs for variant

struct SSMInfo {
    std::string neuronID;
    std::pair<float, float> dist;
    Network net;
    int netIteration;
};

struct USMInfo {
    std::string neuronID;
    std::string prevNeuronID;
    std::pair<float, float> dist;
    int layerNum;
    Network net;
    int netIteration;
};

struct StopInfo {
    bool permanent;
};


#endif