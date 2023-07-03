#include "workerQueue.hpp"

void WorkerQueue::push(Message message) {
    std::lock_guard<std::mutex> lock(m);
    queue.push(message);
}

Message WorkerQueue::pop() {
    std::lock_guard<std::mutex> lock(m);
    Message ret = queue.front();
    queue.pop();
    return ret;
}