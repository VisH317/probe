#include "workerQueue.hpp"

void WorkerQueue::push(Message message) {
    std::lock_guard<std::mutex> lock(m);
    queue.push(std::make_unique<Message>(message));
}

std::unique_ptr<Message> WorkerQueue::pop() {
    std::lock_guard<std::mutex> lock(m);
    std::unique_ptr<Message> ret = std::move(queue.front());
    queue.pop();
    return std::move(ret);
}