#include "workerQueue.hpp"

WorkerQueue::WorkerQueue() {

}

WorkerQueue::WorkerQueue(const WorkerQueue& q) {
    queue = q.queue;
    // m = std::move(q.m);
}

WorkerQueue::~WorkerQueue() {
    // delete m;
}

// void WorkerQueue::push(Message message) {
//     std::lock_guard<std::mutex> lock(*m);
//     queue.push(std::make_unique<Message>(message));
// }

void WorkerQueue::push(std::shared_ptr<Message> message) {
    std::cout<<"QUEUE WORKER: pushing message... "<<message->getType()<<", "<<queue.size()<<std::endl;
    std::lock_guard<std::mutex> lock(m);
    queue.push(message);
    std::cout<<"PUSHED!"<<std::endl;
}

std::shared_ptr<Message> WorkerQueue::pop() {
    std::cout<<"QUEUE WORKER: popping message..."<<std::endl;
    std::lock_guard<std::mutex> lock(m);
    std::shared_ptr<Message> ret = std::move(queue.front());
    queue.pop();
    return std::move(ret);
}

int WorkerQueue::size() {
    // std::cout<<"CALLING SIZE..."<<std::endl;
    // std::lock_guard<std::mutex> lock(m);
    // std::cout<<"CALLED SIZE"<<std::endl;
    return queue.size();
}