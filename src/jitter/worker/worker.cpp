#include "worker.hpp"


Worker::Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs) : config(config), responseQueue(responseQueue), netIteration(0), id(id) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage m) {
    this->netIteration = m.content.netIteration;
    this->net = m.content.net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m.content.net.getLayer(0)->getNeuron(m.content.neuronID);

    std::pair<double, double> out = evaluator.jitter(m.content.net, 0, m.content.neuronID, m.content.dist, std::get<1>(neuronInfo));

    double update = evaluator.updateDist(out.first, out.second);

    ResponseUpdateMessage res{this->id, m.content.neuronID, 0, out.first, out.second, update};

    responseQueue->push(res);
}

void Worker::updateJitter(UpdateSearchMessage m) {
    this->netIteration = m.content.netIteration;
    this->net = m.content.net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m.content.net.getLayer(0)->getNeuron(m.content.neuronID);
    std::tuple<int, torch::Tensor, torch::Tensor> prevNeuronInfo = m.net.getLayer(0)->getNeuron(m.content.prevNeuronID);

    std::pair<double, double> out = evaluator.jitter(m.content.net, m.content.layerNum, m.content.neuronID, m.content.dist, std::get<1>(neuronInfo));

    double update = evaluator.updateDist(out.first, out.second, std::get<1>(prevNeuronInfo));

    ResponseUpdateMessage res{this->id, m.content.neuronID, m.content.layerNum, out.first, out.second, update};

    responseQueue->push(res);
}

void Worker::main() {
    while(true) {
        if(queue.size()==0) continue;

        Message m = queue.pop();

        switch(m.getType()) {
            case MessageType::START:
                startJitter(dynamic_cast<std::unique_ptr<StartSearchMessage>>(m));
        }
    }
}