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


Worker::Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs, std::shared_ptr<Net> netManager) : config(config), responseQueue(responseQueue), netIteration(0), id(id), netManager(netManager) {
    evaluator = Evaluator(input, outputs);
}


void Worker::addTask(Message m) {
    queue.push(m);
}


void Worker::startJitter(StartSearchMessage* m) {
    std::tuple<Network, std::pair<float, float>, int> info = netManager->getCurrentInfo(0, m->neuronID);
    this->netIteration = std::get<2>(info);
    // this->net = m->net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = std::get<0>(info).getLayer(0)->getNeuron(m->neuronID);

    std::tuple<double, double, torch::Tensor> out = evaluator.jitter(std::get<0>(info), 0, m->neuronID, std::get<1>(info), std::get<1>(neuronInfo));

    double update = evaluator.updateDist(std::get<0>(out), std::get<1>(out));

    ResponseUpdateMessage res{this->id, {m->neuronID}, 0, std::get<0>(out), std::get<1>(out), update, std::get<2>(info), std::get<2>(out)};

    responses[{m->neuronID}] = std::nullopt;
    responseQueue->push(res);

    delete m;
}

void Worker::updateJitter(UpdateSearchMessage* m) {
    std::tuple<Network, std::pair<float, float>, int> info = netManager->getCurrentInfo(0, m->neurons.back());
    this->netIteration = std::get<2>(info);
    // this->net = m->net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = std::get<0>(info).getLayer(0)->getNeuron(m->neurons.back());
    std::tuple<int, torch::Tensor, torch::Tensor> prevNeuronInfo = std::get<0>(info).getLayer(0)->getNeuron(*(m->neurons.end()-2));

    std::tuple<double, double, torch::Tensor> out = evaluator.jitter(std::get<0>(info), m->layerNum, m->neurons.back(), std::get<1>(info), std::get<1>(neuronInfo));

    double update = evaluator.updateDist(std::get<0>(out), std::get<1>(std::get<1>(out), std::get<0>(info).getLayer(m->layerNum)->getNeuron(m->neurons.back())));

    ResponseUpdateMessage res{this->id, m->neurons, m->layerNum, std::get<0>(out), std::get<1>(out), update, std::get<2>(info), std::get<2>(out)};

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

        bool searchFinished = true;

        for(auto it = responses.begin(); it!=responses.end(); it++) {
            if(!it->second) {
                searchFinished = false
                break;
            }
        }

        if(searchFinished) responseQueue->push(ResponseDoneMessage{id});

        if(isEnd) break;
    }
}

// setup later
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