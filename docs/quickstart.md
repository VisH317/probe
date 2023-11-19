# Torch-Probe: Quickstart

**Overview:** probe is a library that wraps pytorch layers and networks for analysis. It is able to assign unique IDs to neurons and run a jitter algorithm to determine network stability. 



## Torch-Probe Layers and Networks

**UUIDS:** The UUID assigning works by passing a torchlib layer instance into the Layer class, which adds a wrapper around the layer for analysis. A list of these Layer objects can be passed into the Network class, which acts as a wrapper to identify neurons and run analysis on a full neural network

_DISCLAIMER:_ probe Layers and Network objects only work for networks with linear layers as of now, support for activation functions, convolutional layers, and normalizations need to be added
&nbsp;

**Creating a Layer object:**
```cpp
#include <tuple>
#include <torch-probe/layer.hpp>


int main() {
    Layer layer(100, 50); // represents a torch-probe layer that takes in a 100 size input vector and outputs size 50
    std::tuple<std::string, torch::Tensor, torch::Tensor> info = layer.getNeuron(0);
    // getNeuron: fetches info on a neuron, as a tuple with the UUID, the weight tensor, and the bias tensor


}
```
&nbsp;

**Creating a Network object:**
```cpp
#include <vector>
#include <torch/torch.h>
#include <torch-probe/network.hpp>

int main() {
    Layer layer1(100, 50);
    Layer layer2(50, 10);

    std::vector<Layer> layers = { layer1, layer2 }; // layers need to be in a vector to be passed to network
    Network n(layers); // construct network with list of layers

    torch::Tensor input = torch::rand(100);
    torch::Tensor out = n.forward(input) // works same as torch forward, will return error on dimension mismatch

    Layer layer3(10, 5);
    n.addLayer(layer3); // adds layer to the network

    Layer l = n.getLayer(0); // used to get layer instance at an index in the network, for fetch neurons based on uuid
}
```
&nbsp;


## Jitter Algorithm
The jitter algorithm works by creating and measuring random fluctuation in layer weights, and measuring the corresponding change in output to determine the stability of the neural network. It does this in a configurable multithreaded process to determine the amount of exploration + concurrency/speed requirements

**Methodology:** The jitter algorithm generates random fluctuations for each input weight, and if it alters the chosen output in a specific way to decrease the loss (defined at the beginning), it will continue to fluctuate weights in future layers in connection to the original input. It uses a beta distribution to select random fluctuations. The beta distribution shifts over time based on previous decisions and their loss outcome

**Configuration:** The algorithm has hyperparameters to customize based on the desired level of fluctuation + measurement, it can be instantiated through the Config object

 * **lambda1** - determines the strength of distribution shift on initial neurons
 * **lambda2** - determines the strength of distribution shift on hidden neurons
 * **temperature** - determines the range of fluctuations (similar to alpha in backpropagation)
 * **maxIterations** - maximum number of iterations for jitter algorithm to run for before terminating


**Info to supply to algorithm:**
 * network: the Network object to run the jitter algorithm on
 * numWorkers: the number of concurrent workers generating random numbers over time
 * input: the input tensor (is supplied at the beginning because it is a control to measure output fluctuations)
 * outputs: a vector of output neuron indices to determine which output neurons will be measured for fluctuation
   * _NOTE:_ this allows for the jitter algorithm to measure changes in specific output locations/categories
 * config: config object specified above
 * desiredLoss: the desired amount of fluctuation measured in the output neurons
   * _NOTE:_ the loss is measured as an average of the fluctuation measured in all output neurons that require observation (denoted by the outputs vector)

&nbsp;

**Jitter Algorithm Usage:**
```cpp
#include <torch-probe/network.hpp>
#include <torch-probe/jitter/config.hpp>
#include <torch-probe/jitter/main/netManager.hpp> 

int main(int argc, char* argv[]) {
    Layer l1(100, 50);
    Layer l2(50, 1);

    std::vector<Layer> layers = { l1, l2 };
    Network n(layers); // construct network

    // config
    int numWorkers = 1; // number of multithreaded workers to run on
    torch::Tensor input = torch::rand(100); // input to use for jitter algorithm
    std::vector<int> outputs = { 0 }; // output neurons to observe
    double desiredLoss = 5; // desired deviation to achieve
    
    // create the config object
    Config config(0.15, 0.07, 1, argc == 0 ? 100 : std::atoi(argv[1])); // lambda1, lambda2, temperature, and maxiterations

    NetManager nm(n, numWorkers, input, outputs, config, desiredLoss); // main jitter algo object

    nm.start(); // start jitter process, will spawn worker threads and begin communication for jitter algo
}
```
&nbsp;

The output of the jitter algorithm should be shown in updates.csv, where the format is:
<blockquote style="padding: 10px"><code style="color: white">layer_index, neuron_id, update_value, resulting_loss</code></blockquote>
<!-- <blockquote style="padding: 10px; border-color: #ef4444; background-color: #ef444440; color: white"><code style="color: white">layer_index, neuron_id, update_value, resulting_loss</code></blockquote> -->

&nbsp;

## Using Pretrained Network Params

While the jitter algorithm works with regular neural networks, it is meant to analyze the effects of weights on specific outputs in a trained network. To do this, importing a trained network is slightly different from the code above.

```cpp
#include <torch/torch.h>
#include <torch-probe/network.hpp>
#include <torch-probe/jitter/config.hpp>
#include <torch-probe/jitter/main/netManager.hpp> 

// define a torch::nn::Module here (or any subclass)
// here I defined a sequential with 3 linear layers + activations
torch::nn::Sequential net(
    torch::nn::Linear(100, 50),
    torch::nn::ReLU(),
    torch::nn::Linear(50, 25),
    torch::nn::ReLU(),
    torch::nn::Linear(25, 5),
    torch::nn::Sigmoid()
)

// with whatever subclass you use, cast it to a shared pointer of torch::nn::Module (torch has a helper for this)

std::shared_ptr<torch::nn::Module> mod(net->as<torch::nn::Module>())

int main() {
    // create the probe network
    Network n(mod);

    // configuration for jitter algorithm
    int numWorkers = 1;
    torch::Tensor input = torch::rand(100);
    std::vector<int> outputs = { 0 };
    double desiredLoss = 5;
    
    // create config object
    Config config(0.15, 0.07, 1, argc == 0 ? 100 : std::atoi(argv[1]));

    // create jitter algo manager and start algo
    NetManager nm(n, numWorkers, input, outputs, config, desiredLoss);
    nm.start();
}
```

## Future Updates

 * Adding all info to one namespace to prevent conflicts
 * Fixing include paths to make declarations easier
 * Add better visualization for jitter algorithm
