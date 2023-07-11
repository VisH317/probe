#include "workerQueue.hpp"

void WorkerQueue::push(Message message) {
    std::lock_guard<std::mutex> lock(m);
    queue.emplace(std::make_unique<Message>(message));
}

void WorkerQueue::push(std::unique_ptr<Message> message) {
    std::lock_guard<std::mutex> lock(m);
    queue.push(message);
}

std::unique_ptr<Message> WorkerQueue::pop() {
    std::lock_guard<std::mutex> lock(m);
    std::unique_ptr<Message> ret = std::move(queue.front());
    queue.pop();
    return std::move(ret);
}

int WorkerQueue::size() {
    std::lock_guard<std::mutex> lock(m);
    return queue.size();
}