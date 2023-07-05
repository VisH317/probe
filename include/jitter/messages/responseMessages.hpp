#ifndef __RESPONSE_MESSAGES_H__
#define __RESPONSE_MESSAGES_H__

#include <string>

enum ResponseType {
    UPDATE,
    DONE
};

struct ResponseMessage {
    virtual ResponseType getType();
};

struct ResponseUpdateMessage : ResponseMessage {
    int workerId;
    std::string uuid;
    int layerNum;
    double loss;
    double randomChange;
    ResponseType getType() override { return UPDATE; };
};

struct ResponseDoneMessage : ResponseMessage {
    int workerId;
    ResponseType getType() override { return DONE; };
};


#endif