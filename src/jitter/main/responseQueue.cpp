#include "responseQueue.hpp"

// void ResponseQueue::push(ResponseMessage message) {
//     std::lock_guard<std::mutex> lock(m);
//     queue.push(std::make_unique<ResponseMessage>(message));
// }

void ResponseQueue::push(std::unique_ptr<ResponseMessage> message) {
    std::lock_guard<std::mutex> lock(m);
    queue.push(std::move(message));
}

std::unique_ptr<ResponseMessage> ResponseQueue::pop() {
    std::lock_guard<std::mutex> lock(m);
    std::unique_ptr<ResponseMessage> ret = std::move(queue.front());
    queue.pop();
    return std::move(ret);
}

int ResponseQueue::size() {
    std::lock_guard<std::mutex> lock(m);
    return queue.size();
}