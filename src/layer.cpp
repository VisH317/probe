#include "layer.hpp"

long long Layer::curId = 0;

Layer::Layer(int in, int out) {
    this->in = in;
    this->out = out;
    layer = std::make_shared<torch::nn::LinearImpl>(in, out);
    // this->aux = aux;
    for(int i=0;i<in;i++) {
        std::string uuid = std::to_string(curId);
        curId++;
        id.push_back(uuid);
    }
}

Layer::Layer(std::shared_ptr<torch::nn::LinearImpl> layer, std::vector<std::shared_ptr<torch::nn::AnyModule>> aux) : layer(layer), aux(aux) {
    std::cout<<"test: "<<getLength()<<std::endl;
    for(int i = 0; i < getLength(); i++) {
        std::string uuid = std::to_string(curId);
        curId++;
        id.push_back(uuid);
    }
}

std::string Layer::generateId() {
    return std::to_string(curId);
    curId++;
}

std::shared_ptr<torch::nn::LinearImpl>& Layer::getLayer() {
    return layer;
}

std::tuple<std::string, torch::Tensor, torch::Tensor> Layer::getNeuron(int num) {
    if(num>=in || num<0) throw std::out_of_range("The neuron " + std::to_string(num) + " you selected to probe was out of range of the layer's neurons"); 
    torch::Tensor neuronWeights = layer->weight;
    torch::Tensor ret = neuronWeights.select(1, num);
    
    torch::Tensor neuronBiases = layer->bias;
    torch::Tensor b = neuronBiases.select(0, 0);

    std::string uuid = id[num];

    return {uuid, ret, b};
}

std::tuple<int, torch::Tensor, torch::Tensor> Layer::getNeuron(std::string uuid) {
    // for(auto it = id.begin(); it!=id.end(); it++) std::cout<<"TET: "<<*it<<std::endl;
    auto it = std::find(id.begin(), id.end(), uuid);
    if(it==id.end()) throw std::runtime_error("Neuron with matching uuid not found: " + uuid);
    int idx = it - id.begin();

    std::tuple<std::string, torch::Tensor, torch::Tensor> neuronInfo = getNeuron(idx);

    return { idx, std::get<1>(neuronInfo), std::get<2>(neuronInfo) };
}

void Layer::changeNeuronWeight(std::string uuid, torch::Tensor update) {
    torch::autograd::GradMode::set_enabled(false);
    torch::Tensor* neuronWeight = &(layer->weight);
    std::tuple<int, torch::Tensor, torch::Tensor> neuron = getNeuron(uuid);
    neuronWeight->index_put_({"...", std::get<0>(neuron)}, update);
    torch::autograd::GradMode::set_enabled(true);
}

std::pair<int, int> Layer::getDims() {
    return { layer->weight.sizes()[1], layer->weight.sizes()[0] };
}

int Layer::getLength() {
    return layer->weight.sizes()[1];
}

std::vector<std::string> Layer::getAllNeuronIds() {
    return id;
}

torch::Tensor Layer::forward(torch::Tensor input) {
    torch::Tensor intermediate = layer->forward(input);
    for(auto& item : aux) intermediate = item->forward(intermediate);
    return intermediate;
}