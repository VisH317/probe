#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>

enum MessageType {
    START,
    UPDATE,
    STOP
};


struct Message {
    int pid;
    virtual MessageType getType();
};

struct StartSearchMessage : public virtual Message {
    std::string neuronID;
    std::pair<float, float> dist;
    MessageType getType() override { return START; };
};

struct UpdateSearchMessage : Message {
    std::string neuronID;
    std::pair<float, float> dist;
    int layerNum;
    MessageType getType() override { return UPDATE; };
};

struct StopMessage : Message {
    MessageType getType() override { return STOP; };
};


#endif