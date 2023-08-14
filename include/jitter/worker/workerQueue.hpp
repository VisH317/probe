#ifndef __WORKER_QUEUE_H__
#define __WORKER_QUEUE_H__

#include <queue>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <utility>
#include "message.hpp"

/**
 * @brief singleton queue per worker to receive messages from main process
 * 
 */
class WorkerQueue {
    private:
        /** Main queue */
        std::queue<std::shared_ptr<Message>> queue;
        /** queue lock */
        mutable std::mutex m;

    public:
        /**
         * @brief Construct a new Worker Queue object
         * 
         */
        WorkerQueue();

        /**
         * @brief Construct a new Worker Queue object
         * 
         * @param q reference
         */
        explicit WorkerQueue(const WorkerQueue& q);

        /**
         * @brief Construct a new Worker Queue object
         * 
         * @param q moveable
         */
        WorkerQueue(WorkerQueue&& q) = delete;

        /**
         * @brief Destroy the Worker Queue object
         * 
         */
        ~WorkerQueue();

        /**
         * @brief push a message to the queue
         * 
         * @param message message to push (abstract class pointer)
         */
        void push(std::shared_ptr<Message> message);

        /**
         * @brief pop message from the queue
         * 
         * @return std::shared_ptr<Message> 
         */
        std::shared_ptr<Message> pop();

        /**
         * @brief Get current queue size
         * 
         * @return int 
         */
        int size();

};

#endif