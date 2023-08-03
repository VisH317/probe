#include "netManager.hpp"

// dynamic_unique_cast util
template <typename To, typename From>
std::unique_ptr<To> dynamic_unique_cast(std::unique_ptr<From>&& p) {
    if(To* cast = dynamic_cast<To*>(p.get())) {
        std::unique_ptr<To> result(cast);
        p.release();
        return result;
    }
    throw std::bad_cast();
}


NetManager::NetManager(Network &network, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config, double desiredLoss) : config(std::make_shared<Config>(std::move(config))), outputs(std::move(outputs)), input(std::move(input)), tasks(numWorkers, *network.getLayer(0)), desiredLoss(desiredLoss)
{
    responseQueue = std::make_shared<ResponseQueue>();
    net = std::make_shared<Net>(network);

    std::cout<<"CONFIG:"<<this->config->temperature<<std::endl;

    for (int i = 0; i < numWorkers; i++)
    {
        Worker worker(i, this->config, responseQueue, this->input, this->outputs, net);
        workers.emplace_back(worker);
    }

    std::cout<<"MAIN PROCESS: Initialized!"<<std::endl;
}

void NetManager::start()
{
    for (int i = 0; i < workers.size(); i++) {
        ProcessState state = tasks.getProcessInfo(i);
        std::shared_ptr<Message> m;
        m = std::make_shared<StartSearchMessage>(state.currentState);
        workers[i].addTask(m);
        workers[i].start();
    }
    // thread =  std::make_shared<std::thread>(&NetManager::process, this);
    std::cout<<"MAIN PROCESS: main thread started!"<<std::endl;
    process();
}

void NetManager::process()
{
    int iterations = 0;

    while (true)
    {
        if (responseQueue->size() == 0)
            continue;

        std::unique_ptr<ResponseMessage> m = responseQueue->pop();

        std::cout<<"MAIN: message: "<<m->getType()<<std::endl;

        switch (m->getType())
        {
        case ResponseType::RES_UPDATE:
            // std::cout<<dynamic_cast<ResponseUpdateMessage*>(m.get())->uuid<<std::endl;
            updateDist(dynamic_unique_cast<ResponseUpdateMessage>(std::move(m)));
            break;
        case ResponseType::RES_DONE:
            createNewSearch(dynamic_unique_cast<ResponseDoneMessage>(std::move(m)));
            break;
        default:
            break;
        }
    }
}

void NetManager::updateDist(std::unique_ptr<ResponseUpdateMessage> m)
{
    std::cout<<"MAIN: updating parameters..."<<std::endl;
    bool shouldUpdate = losses.shouldUpdate(m->loss, m->netIteration);
    if (!shouldUpdate)
    {
        std::cout<<"MAIN: update rejected"<<std::endl;
        RejectedMessage rejected{m->uuid, m->layerNum};
        std::shared_ptr<Message> mes;
        mes = std::make_shared<RejectedMessage>(rejected);
        workers[m->workerId].addTask(mes);
        return;
    }

    std::cout<<"MAIN: update applied..."<<m->update<<", "<<m->uuid<<std::endl;
    net->updateDist(m->uuid.back(), m->layerNum, m->update, m->updateTen);

    logger.writeUpdate(m->layerNum, m->uuid.back(), m->update, m->loss);

    std::shared_ptr<Message> mes;
    mes = std::make_shared<ValidMessage>(m->uuid, m->layerNum);
    workers[m->workerId].addTask(mes);
}

NetManager::~NetManager()
{
    for (Worker &worker : workers)
    {
        std::shared_ptr<Message> m;
        m = std::make_shared<StopMessage>();
        worker.addTask(m);
    }
    std::cout<<"ENDING :)"<<std::endl;
    workers.clear();
    thread->join();
}

void NetManager::createNewSearch(std::unique_ptr<ResponseDoneMessage> m)
{
    std::string id = tasks.setNewProcessState(m->workerId);
    // StartSearchMessage message{id};
    std::shared_ptr<Message> mes;
    mes = std::make_shared<StartSearchMessage>(id);
    workers[m->workerId].addTask(mes);
}