#ifndef __RESPONSE_MESSAGES_H__
#define __RESPONSE_MESSAGES_H__

#include <string>
#include <vector>
#include <memory>
#include <torch/torch.h>

enum ResponseType
{
    RES_UPDATE,
    RES_DONE,
    RES_PARENT,
    RES_END
};

struct ResponseMessage
{
    virtual ResponseType getType() { return RES_PARENT; };
};

struct ResponseUpdateMessage : ResponseMessage
{
    /** id of worker sending the message */
    int workerId;

    /** list of neuron ids in path of update */
    std::vector<std::string, std::allocator<std::string>> uuid;

    /** layer number of current updated neuron */
    int layerNum;

    /** Loss at neuron */
    double loss;

    /** Random induced shift in neuron that was measured */
    double randomChange;

    /** Predicted update required to distribution based on weights change */
    double update;

    /** Net iteration of jitter iteration */
    int netIteration;

    /** Tensor of created updates */
    torch::Tensor updateTen;

    /**
     * @brief Construct a new Response Update Message object
     * 
     * @param workerId id of worker
     * @param uuid neuron path
     * @param layerNum layer number of updated neuron
     * @param loss loss measured at updated neuron
     * @param randomChange random change induced at the updated neuron
     * @param update predicted distribution update
     * @param netIteration iteration of network when update was induced
     * @param ten update tensor used
     */
    ResponseUpdateMessage(int workerId, std::vector<std::string> uuid, int layerNum, double loss, double randomChange, double update, int netIteration, torch::Tensor ten) : workerId(workerId), uuid(uuid), layerNum(layerNum), loss(loss), randomChange(randomChange), update(update), netIteration(netIteration), updateTen(ten) {};

    /**
     * @brief Get message type
     * 
     * @return ResponseType::RES_UPDATE 
     */
    ResponseType getType() override { return ResponseType::RES_UPDATE; };
};

struct ResponseDoneMessage : ResponseMessage
{
    /** worker id */
    int workerId;

    /**
     * @brief Construct a new Response Done Message object
     * 
     * @param workerId worker id
     */
    ResponseDoneMessage(int workerId) : workerId(workerId){};

    /**
     * @brief Get message type
     * 
     * @return ResponseType::RES_DONE
     */
    ResponseType getType() override { return ResponseType::RES_DONE; };
};

struct ResponseEndMessage : ResponseMessage {
    ResponseType getType override { return ResponseType::RES_END; };
};

#endif