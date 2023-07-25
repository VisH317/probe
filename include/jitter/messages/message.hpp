#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <string>
#include <utility>
#include <tuple>
#include "network.hpp"

/**
 * @brief enum of message types for messages sent to worker queues
 * 
 */
enum MessageType
{
    START,
    UPDATE,
    STOP,
    VALID,
    REJECTED,
    PARENT
};

/**
 * @brief abstract class of message 
 * 
 */
struct Message
{
    /**
     * @brief get type of the message - used for finding which handler to call
     * 
     * @return MessageType
     */
    virtual MessageType getType() = 0;
};

struct StartSearchMessage : public virtual Message
{
    /** id of neuron to start search on */
    std::string neuronID;

    /**
     * @brief Construct a new Start Search Message object
     * 
     * @param neuronID id of neuron
     */
    StartSearchMessage(std::string neuronID) : neuronID(neuronID){};

    /**
     * @brief Get type of message
     * 
     * @return MessageType::START
     */
    MessageType getType() override { return START; };
};

struct UpdateSearchMessage : Message
{
    /** List of neurons in the update message path */
    std::vector<std::string> neurons;

    /** current layer num of the latest neuron being searched */
    int layerNum;

    /**
     * @brief Construct a new Update Search Message object
     * 
     * @param neurons - list of neurons in the update path
     * @param layerNum - current layer of neuron
     */
    UpdateSearchMessage(std::vector<std::string> neurons, int layerNum) : layerNum(layerNum), neurons(neurons){};
    
    /**
     * @brief Get message type
     * 
     * @return MessageType::UPDATE
     */
    MessageType getType() override { return UPDATE; };
};

struct ValidMessage : Message
{
    /** path of neurons to validate and continue */
    std::vector<std::string> neuronID;

    /** Current layer num of latest neuron */
    int layerNum;

    /**
     * @brief Construct a new Valid Message object
     * 
     * @param neuronID - list of neuron IDs
     * @param layerNum - layer number
     */
    ValidMessage(std::vector<std::string> neuronID, int layerNum) : neuronID(neuronID), layerNum(layerNum){};

    /**
     * @brief Get message type
     * 
     * @return MessageType::VALID
     */
    MessageType getType() override { return VALID; };
};

struct RejectedMessage : Message
{
    /** neuron path to reject */
    std::vector<std::string> neuronID;

    /** Current layer number */
    int layerNum;

    /**
     * @brief Construct a new Rejected Message object
     * 
     * @param neuronID - neuron path to reject
     * @param layerNum - current layer number
     */
    RejectedMessage(std::vector<std::string> neuronID, int layerNum) : neuronID(neuronID), layerNum(layerNum){};

    /**
     * @brief Get message type
     * 
     * @return MessageType::REJECTED
     */
    MessageType getType() override { return REJECTED; };
};

struct StopMessage : Message
{
    /**
     * @brief Get message type
     * 
     * @return MessageType::STOP
     */
    MessageType getType() override { return STOP; };
};

#endif