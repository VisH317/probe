#ifndef __RESPONSE_MESSAGES_H__
#define __RESPONSE_MESSAGES_H__

#include <string>
#include <vector>
#include <torch/torch.h>

enum ResponseType {
    UPDATE,
    DONE
};

struct ResponseMessage {
    virtual ResponseType getType();
};

struct ResponseUpdateMessage : ResponseMessage {
    int workerId;
    std::vector<std::string> uuid;
    int layerNum;
    double loss;
    double randomChange;
    double update;
    int netIteration;
    torch::Tensor update;
    // ResponseUpdateMessage(int workerId, std::vector<std::string> uuid, int layerNum, double loss, double randomChange, double update) : workerId(workerId), uuid(uuid), layerNum(layerNum), loss(loss), randomChange(randomChange), update(update) {};
    ResponseType getType() override { return ResponseType::UPDATE; };
};

struct ResponseDoneMessage : ResponseMessage {
    int workerId;
    ResponseType getType() override { return ResponseType::DONE; };
};


#endif