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
        std::queue<std::shared_ptr<Message>> queue;
        std::mutex* m;

    public:
        WorkerQueue();
        ~WorkerQueue();

        void push(Message message);
        void push(std::shared_ptr<Message> message);
        std::shared_ptr<Message> pop();
        int size();

};

#endif