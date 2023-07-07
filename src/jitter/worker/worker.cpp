#include "worker.hpp"

template <typename To, typename From, typename Deleter> 
std::unique_ptr<To, Deleter> dynamic_unique_cast(std::unique_ptr<From, Deleter>&& p) {
    if (To* cast = dynamic_cast<To*>(p.get()))
    {
        std::unique_ptr<To, Deleter> result(cast, std::move(p.get_deleter()));
        p.release();
        return result;
    }
    return std::unique_ptr<To, Deleter>(nullptr); // or throw std::bad_cast() if you prefer
}


Worker::Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs) : config(config), responseQueue(responseQueue), netIteration(0), id(id) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage* m) {
    this->netIteration = m->netIteration;
    this->net = m->net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m->net.getLayer(0)->getNeuron(m->neuronID);

    std::pair<double, double> out = evaluator.jitter(m->net, 0, m->neuronID, m->dist, std::get<1>(neuronInfo));

    double update = evaluator.updateDist(out.first, out.second);

    ResponseUpdateMessage res{this->id, m->neuronID, 0, out.first, out.second, update};

    responses[{m->neuronID}] = std::nullopt;
    responseQueue->push(res);

    delete m;
}

void Worker::updateJitter(UpdateSearchMessage* m) {
    this->netIteration = m->netIteration;
    this->net = m->net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = m->net.getLayer(0)->getNeuron(m->neurons.back());
    std::tuple<int, torch::Tensor, torch::Tensor> prevNeuronInfo = m->net.getLayer(0)->getNeuron(*(m->neurons.end()-2));

    std::pair<double, double> out = evaluator.jitter(m->net, m->layerNum, m->neurons.back(), m->dist, std::get<1>(neuronInfo));

    double update = evaluator.updateDist(out.first, out.second, std::get<1>(prevNeuronInfo));

    ResponseUpdateMessage res{this->id, m->neurons.back(), m->layerNum, out.first, out.second, update};

    responses[m->neurons] = std::nullopt;
    responseQueue->push(res);

    delete m;
}

void Worker::main() {

    bool isEnd = false;

    while(true) {
        if(queue.size()==0) continue;

        std::unique_ptr<Message> m = queue.pop();

        switch(m.get()->getType()) {
            case MessageType::START:
                startJitter(std::move(dynamic_cast<StartSearchMessage*>(m.get())));
                break;
            case MessageType::UPDATE:
                updateJitter(std::move(dynamic_cast<UpdateSearchMessage*>(m.get())));
                break;
            case MessageType::STOP:
                isEnd = true;
                break;
            case MessageType::VALID:
                setValid(std::move(dynamic_cast<ValidMessage*>(m.get())));
                break;
            case MessageType::REJECTED:
                setReject(std::move(dynamic_cast<RejectedMessage*>(m.get())));
                break;
            default: break;
        }

        if(isEnd) break;
    }
}


void Worker::setValid(ValidMessage* m) {
    responses[m->neuronID] = true;
    if(m->layerNum+1<net.getLayerLength()) {
        for(std::string& id : net.getLayer(m->layerNum+1)->getAllNeuronIds()) {
            std::vector v(m->neuronID);
            v.push_back(id);
            UpdateSearchMessage message{v, };
        }
    }

}

void Worker::setReject(ValidMessage* m) {
    responses[m->neuronID] = false;
}


void Worker::start() {
    thread = std::thread(main);
}

Worker::~Worker() {
    StopMessage stop;
    queue.push(stop);
    thread.join();
}