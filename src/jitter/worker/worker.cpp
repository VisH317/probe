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


Worker::Worker(int id, std::shared_ptr<Config> config, std::shared_ptr<ResponseQueue> responseQueue, torch::Tensor input, std::vector<int> outputs, std::shared_ptr<Net> netManager) : config(config), responseQueue(responseQueue), netIteration(0), id(id), netManager(netManager), evaluator(input, outputs, config) {
    std::cout<<"WORKER: initialized! "<<std::endl;
}


void Worker::addTask(std::shared_ptr<Message> m) {
    queue.push(m);
}


void Worker::startJitter(std::shared_ptr<StartSearchMessage> m) {
    std::tuple<Network, std::pair<float, float>, int> info = netManager->getCurrentInfo(0, m->neuronID);
    this->netIteration = std::get<2>(info);
    // this->net = m->net;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = std::get<0>(info).getLayer(0)->getNeuron(m->neuronID);


    std::tuple<double, double, torch::Tensor> out = evaluator.jitter(std::get<0>(info), 0, m->neuronID, std::get<1>(info), std::get<1>(neuronInfo));

    double update = evaluator.updateDist(std::get<0>(out), std::get<1>(out));

    std::vector<std::string> ns;
    ns.push_back(m->neuronID);

    std::unique_ptr<ResponseMessage> res;
    std::unique_ptr<ResponseUpdateMessage> rum = std::make_unique<ResponseUpdateMessage>(this->id, ns, 0, std::get<0>(out), std::get<1>(out), update, this->netIteration, std::get<2>(out));
    res = std::move(rum);

    responses[{m->neuronID}] = std::nullopt;
    responseQueue->push(std::move(res));

    // delete m;
}

void Worker::updateJitter(std::shared_ptr<UpdateSearchMessage> m) {
    std::tuple<Network, std::pair<float, float>, int> info = netManager->getCurrentInfo(m->layerNum, m->neurons.back());
    this->netIteration = std::get<2>(info);
    // this->net = m->net;

    std::cout<<"TESTINGGGG: "<<m->layerNum<<std::endl;

    std::tuple<int, torch::Tensor, torch::Tensor> neuronInfo = std::get<0>(info).getLayer(0)->getNeuron(m->neurons.back());
    std::tuple<int, torch::Tensor, torch::Tensor> prevNeuronInfo = std::get<0>(info).getLayer(0)->getNeuron(*(m->neurons.end()-2));

    std::tuple<double, double, torch::Tensor> out = evaluator.jitter(std::get<0>(info), m->layerNum, m->neurons.back(), std::get<1>(info), std::get<1>(neuronInfo));

    double update = evaluator.updateDist(std::get<0>(out), std::get<1>(out), m->neurons, m->layerNum, std::get<0>(info));

    std::unique_ptr<ResponseMessage> res;
    res = std::make_unique<ResponseUpdateMessage>(this->id, m->neurons, m->layerNum, std::get<0>(out), std::get<1>(out), update, this->netIteration, std::get<2>(out));
    // ResponseUpdateMessage res(this->id, m->neurons, m->layerNum, std::get<0>(out), std::get<1>(out), update, std::get<2>(out));

    responses[m->neurons] = std::nullopt;
    responseQueue->push(std::move(res));
}

void Worker::main() {

    std::cout<<"WORKER: starting worker thread..."<<std::endl;

    bool isEnd = false;

    double r = std::rand() / double(RAND_MAX);
    boost::math::beta_distribution<> dist(50.0, 50.0);
    double randFromDist = boost::math::quantile(dist, r);

    while(true) {
        if(queue.size()==0) continue;

        std::shared_ptr<Message> m = queue.pop();
        std::cout<<"WORKER: Message - "<<m->getType()<<std::endl;

        switch(m->getType()) {
            case MessageType::START:
                std::cout<<"WORKER: START MESSAGE RECEIVED"<<std::endl;
                startJitter(std::move(std::dynamic_pointer_cast<StartSearchMessage>(m)));
                break;
            case MessageType::UPDATE:
                std::cout<<"WORKER: UPDATE MESSAGE RECEIVED"<<std::endl;
                updateJitter(std::move(std::dynamic_pointer_cast<UpdateSearchMessage>(m)));
                break;
            case MessageType::STOP:
                std::cout<<"WORKER: STOP MESSAGE RECEIVED"<<std::endl;
                isEnd = true;
                break;
            case MessageType::VALID:
                std::cout<<"WORKER: VALID MESSAGE RECEIVED"<<std::endl;
                setValid(std::move(std::dynamic_pointer_cast<ValidMessage>(m)));
                break;
            case MessageType::REJECTED:
                std::cout<<"WORKER: REJECTED MESSAGE RECEIVED"<<std::endl;
                setReject(std::move(std::dynamic_pointer_cast<RejectedMessage>(m)));
                break;
            default: break;
        }

        bool searchFinished = true;

        for(auto it = responses.begin(); it!=responses.end(); it++) {
            if(!it->second) {
                searchFinished = false;
                break;
            }
        }

        if(searchFinished) {
            std::unique_ptr<ResponseMessage> rm;
            rm = std::make_unique<ResponseDoneMessage>(id);
            responseQueue->push(std::move(rm));
        }

        if(isEnd) break;
    }
}

// setup later
void Worker::setValid(std::shared_ptr<ValidMessage> m) {
    std::tuple<Network, std::pair<float, float>, int> info = netManager->getCurrentInfo(0, m->neuronID.back());
    responses[m->neuronID] = true;
    if(m->layerNum+1<std::get<0>(info).getLayerLength()) {
        for(std::string& id : std::get<0>(info).getLayer(m->layerNum+1)->getAllNeuronIds()) {
            std::vector v(m->neuronID);
            v.push_back(id);
            // UpdateSearchMessage message{v, m->layerNum+1};
            std::shared_ptr<Message> message;
            message = std::make_shared<UpdateSearchMessage>(v, m->layerNum+1);
            addTask(message);
        }
    }

}

void Worker::setReject(std::shared_ptr<RejectedMessage> m) {
    responses[m->neuronID] = false;
}


void Worker::start() {
    thread = std::make_shared<std::thread>(&Worker::main, this);
}

Worker::~Worker() {
    // StopMessage stop;
    std::cout<<"WORKER: Stopping..."<<std::endl;
    // queue.push(std::make_shared<Message>(stop));
    if(thread != nullptr) thread->join();
    // delete thread;
}