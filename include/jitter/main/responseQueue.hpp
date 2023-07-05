#ifndef __RESPONSE_QUEUE_H__
#define __RESPONSE_QUEUE_H__

#include <queue>
#include <mutex>
#include <memory>
#include "responseMessages.hpp"


class ResponseQueue {
    private:
        std::queue<std::unique_ptr<ResponseMessage>> queue;

    public:
        ResponseQueue() = default;

        void push(ResponseMessage m);

        std::unique_ptr<ResponseMessage> pop();
}


#endif