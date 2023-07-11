#ifndef __RESPONSE_MESSAGES_H__
#define __RESPONSE_MESSAGES_H__

#include <string>
#include <vector>
#include <memory>
#include <torch/torch.h>

enum ResponseType {
    RES_UPDATE,
    RES_DONE
};

struct ResponseMessage {
    virtual ResponseType getType();
};

struct ResponseUpdateMessage : ResponseMessage {
    int workerId;
    std::vector<std::string, std::allocator<std::string>> uuid;
    int layerNum;
    double loss;
    double randomChange;
    double update;
    int netIteration;
    torch::Tensor updateTen;
    ResponseUpdateMessage(int workerId, std::vector<std::string> uuid, int layerNum, double loss, double randomChange, double update, torch::Tensor ten) : workerId(workerId), uuid(uuid), layerNum(layerNum), loss(loss), randomChange(randomChange), update(update), updateTen(ten) {};
    ResponseType getType() override { return ResponseType::RES_UPDATE; };
};

struct ResponseDoneMessage : ResponseMessage {
    int workerId;
    ResponseDoneMessage(int workerId) : workerId(workerId) {};
    ResponseType getType() override { return ResponseType::RES_DONE; };
};


#endif