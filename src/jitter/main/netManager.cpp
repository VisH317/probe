#include "netManager.hpp"

NetManager::NetManager(Network &network, int numWorkers, torch::Tensor input, std::vector<int> outputs, Config config, double desiredLoss) : config(std::make_shared<Config>(std::move(config))), outputs(std::move(outputs)), input(std::move(input)), tasks(numWorkers, *network.getLayer(0)), desiredLoss(desiredLoss)
{

    responseQueue = std::make_shared<ResponseQueue>();
    net = std::make_shared<Net>(network);

    for (int i = 0; i < numWorkers; i++)
    {
        Worker worker(i, this->config, responseQueue, input, outputs, net);
        workers.emplace_back(worker);
    }

    std::cout<<"Initialized!"<<std::endl;
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
    thread =  std::make_shared<std::thread>(&NetManager::process, this);
    std::cout<<"main thread started!"<<std::endl;
}

void NetManager::process()
{
    int iterations = 0;

    while (true)
    {
        if (responseQueue->size() == 0)
            continue;

        std::unique_ptr<ResponseMessage> m = responseQueue->pop();

        switch (m->getType())
        {
        case ResponseType::RES_UPDATE:
            updateDist(dynamic_cast<ResponseUpdateMessage *>(m.get()));
            break;
        case ResponseType::RES_DONE:
            createNewSearch(dynamic_cast<ResponseDoneMessage *>(m.get()));
            break;
        default:
            break;
        }
    }
}

void NetManager::updateDist(ResponseUpdateMessage *m)
{
    bool shouldUpdate = losses.shouldUpdate(m->loss, m->netIteration);
    if (!shouldUpdate)
    {
        RejectedMessage rejected{m->uuid, m->layerNum};
        std::shared_ptr<Message> mes;
        mes = std::make_shared<RejectedMessage>(rejected);
        workers[m->workerId].addTask(mes);
        return;
    }

    net->updateDist(m->uuid.back(), m->layerNum, m->update, m->updateTen);

    std::shared_ptr<Message> mes;
    ValidMessage valid{m->uuid, m->layerNum};
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
    workers.clear();
    thread->join();
}

void NetManager::createNewSearch(ResponseDoneMessage *m)
{
    std::string id = tasks.setNewProcessState(m->workerId);
    // StartSearchMessage message{id};
    std::shared_ptr<Message> mes;
    mes = std::make_shared<StartSearchMessage>(id);
    workers[m->workerId].addTask(mes);
}