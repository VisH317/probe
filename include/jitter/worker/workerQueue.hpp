#ifndef __WORKER_QUEUE_H__
#define __WORKER_QUEUE_H__

#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <utility>
#include "message.hpp"

class WorkerQueue {
    private:
        std::queue<std::unique_ptr<Message>> queue;
        std::mutex m;

    public:
        WorkerQueue() = default;
        WorkerQueue(std::queue<std::unique_ptr<Message>>&& queue) : queue(queue) {};

        void push(Message message);
        Message pop();


};

#endif