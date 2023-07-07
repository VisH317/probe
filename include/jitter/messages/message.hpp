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
    VALID,
    REJECTED
};

// design question: should new networks be passed through messages each time or can the message calling be centralized to one manager
// message passing: need to control move semantics and creation/deletion of other parameters
// // important to have separate copies to prevent updates being run while a worker is running a forward on the thing
// centralized manager has to manage both and both have similar memory requirements, since each worker still has to hold their own copy of the network

struct Message {
    int pid;
    virtual MessageType getType() const;
};

struct StartSearchMessage : public virtual Message {
    std::string neuronID;
    // std::pair<float, float> dist;
    // Network net;
    MessageType getType() const override { return START; };
};

struct UpdateSearchMessage : Message {
    std::vector<std::string> neurons;
    // std::pair<float, float> dist;
    int layerNum;
    // Network net;
    MessageType getType() const override { return UPDATE; };
};

struct ValidMessage : Message {
    std::vector<std::string> neuronID;
    
    int layerNum;
    MessageType getType() const override { return VALID; };
};

struct RejectedMessage : Message {
    std::vector<std::string> neuronID;
    int layerNum;
    MessageType getType() const override { return REJECTED; };
};

struct StopMessage : Message {
    MessageType getType() const override { return STOP; };
};
 


#endif