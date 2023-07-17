#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <tuple>
#include "network.hpp"

enum MessageType
{
    START,
    UPDATE,
    STOP,
    VALID,
    REJECTED,
    PARENT
};

// design question: should new networks be passed through messages each time or can the message calling be centralized to one manager
// message passing: need to control move semantics and creation/deletion of other parameters
// // important to have separate copies to prevent updates being run while a worker is running a forward on the thing
// centralized manager has to manage both and both have similar memory requirements, since each worker still has to hold their own copy of the network

struct Message
{
    // int pid;
    virtual MessageType getType() = 0;
};

struct StartSearchMessage : public virtual Message
{
    std::string neuronID;
    // std::pair<float, float> dist;
    // Network net;
    StartSearchMessage(std::string neuronID) : neuronID(neuronID){};
    MessageType getType() override { return START; };
};

struct UpdateSearchMessage : Message
{
    std::vector<std::string> neurons;
    // std::pair<float, float> dist;
    int layerNum;
    UpdateSearchMessage(std::vector<std::string> neurons, int layerNum) : layerNum(layerNum), neurons(neurons){};
    // Network net;
    MessageType getType() override { return UPDATE; };
};

struct ValidMessage : Message
{
    std::vector<std::string> neuronID;
    int layerNum;
    ValidMessage(std::vector<std::string> neuronID, int layerNum) : neuronID(neuronID), layerNum(layerNum){};
    MessageType getType() override { return VALID; };
};

struct RejectedMessage : Message
{
    std::vector<std::string> neuronID;
    int layerNum;
    RejectedMessage(std::vector<std::string> neuronID, int layerNum) : neuronID(neuronID), layerNum(layerNum){};
    MessageType getType() override { return REJECTED; };
};

struct StopMessage : Message
{
    MessageType getType() override { return STOP; };
};

#endif