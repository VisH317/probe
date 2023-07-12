#include "workerQueue.hpp"

WorkerQueue::WorkerQueue() {
    m = new std::mutex();
}

WorkerQueue::~WorkerQueue() {
    delete m;
}

void WorkerQueue::push(Message message) {
    std::lock_guard<std::mutex> lock(*m);
    queue.push(std::make_unique<Message>(message));
}

void WorkerQueue::push(std::shared_ptr<Message> message) {
    std::lock_guard<std::mutex> lock(*m);
    queue.push(std::move(message));
}

std::shared_ptr<Message> WorkerQueue::pop() {
    std::lock_guard<std::mutex> lock(*m);
    std::shared_ptr<Message> ret = std::move(queue.front());
    queue.pop();
    return std::move(ret);
}

int WorkerQueue::size() {
    std::lock_guard<std::mutex> lock(*m);
    return queue.size();
}