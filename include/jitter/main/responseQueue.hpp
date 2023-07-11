#ifndef __RESPONSE_QUEUE_H__
#define __RESPONSE_QUEUE_H__

#include <queue>
#include <mutex>
#include <memory>
#include <utility>
#include <mutex>
#include "responseMessages.hpp"


class ResponseQueue {
    private:
        std::queue<std::unique_ptr<ResponseMessage>> queue;
        std::mutex m;

    public:
        ResponseQueue() = default;

        void push(ResponseMessage m);
        void push(std::unique_ptr<ResponseMessage> m);

        std::unique_ptr<ResponseMessage> pop();

        int size();
};


#endif