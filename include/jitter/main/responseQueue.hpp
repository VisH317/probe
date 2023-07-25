#ifndef __RESPONSE_QUEUE_H__
#define __RESPONSE_QUEUE_H__

#include <queue>
#include <mutex>
#include <memory>
#include <utility>
#include <mutex>
#include <iostream>
#include "responseMessages.hpp"

/**
 * @brief queue to push and receive response messages (network update and done)
 * 
 */
class ResponseQueue {
    private:
        /** Queue instance */
        std::queue<std::unique_ptr<ResponseMessage>> queue;
        /** Mutex to lock queu updates in multithreaded environment */
        std::mutex m;

    public:
        /** Default constructor */
        ResponseQueue() = default;

        /**
         * @brief push a message to the queue
         * 
         * @param m (std::unique_ptr<ResponseMessage>) message to push to queue
         */
        void push(std::unique_ptr<ResponseMessage> m);

        /**
         * @brief pop a message from teh queue
         * 
         * @return std::unique_ptr<ResponseMessage> 
         */
        std::unique_ptr<ResponseMessage> pop();

        /**
         * @brief Get size of queue
         * 
         * @return int 
         */
        int size();
};


#endif