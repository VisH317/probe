#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <torch/torch.h>
#include <iterator>
#include <vector>
#include <optional>
#include <iostream>
#include <stdexcept>
#include "layer.hpp"

/**
 * @brief Network of Layer wrappers to run forward processes and probe specific parts of each layer
 * 
 */

std::vector<std::shared_ptr<torch::nn::Module>> getLayers(torch::nn::Module mod);

class Network {
    private:
        /** List of layers in the network */
        std::vector<Layer> layers;

        std::vector<std::shared_ptr<torch::nn::AnyModule>> initModules;

    public:
        /**
         * @brief Construct an empty Network object
         * 
         */
        Network() {};

        Network(std::shared_ptr<torch::nn::Module>& mod);

        Network(std::shared_ptr<torch::nn::Module>&& mod) : Network(mod) {};

        /**
         * @brief Construct a new Network object from a vector of layers
         * 
         * @param l (std::vector<Layer>& l) Vector of layers to be added
         */
        Network(std::vector<Layer>& l);

        /**
         * @brief Construct a new Network object from a vector of layers (move semantics)
         * 
         * @param l (std::vector<Layer>&& l) rvalue reference of layer vectors (memory optimized)
         */
        Network(std::vector<Layer>&& l);

        /**
         * @brief Destroy the Network object
         * 
         */
        ~Network() = default;

        /**
         * @brief Add a layer to the list of layers within the network
         * 
         * @param layer (Layer) Layer object to be added
         */
        void addLayer(Layer layer);

        /**
         * @brief Get a Layer object within the network
         * 
         * @param idx (int) Index of layer to be fetched
         * @return std::optional<Layer> Layer object or nullopt if nonexistent
         * @throw out of bounds if idx not within range of layer indexes
         */
        std::optional<Layer> getLayer(int idx);

        /**
         * @brief Run a forward pass on the network
         * 
         * @param input (torch::Tensor) input tensor to be passed into the network
         * @return torch::Tensor output tensor received from being passed through all layers
         * @throw dimension mismatch if input tensor does not match first layer input dimension
         * @throw dimension mismatch if checkValid() fails
         */
        torch::Tensor forward(torch::Tensor input);

        /**
         * @brief Functor-style call for forward pass
         * 
         * @param input (torch::Tensor) see forward
         * @return torch::Tensor see forward()
         */
        torch::Tensor operator() (torch::Tensor input) { return forward(input); };

        /**
         * @brief Checks for dimension mismatches between layers
         * 
         * @return true if dimensions match between layers
         * @return false if dimensions do not match
         */
        bool checkValid();
        
        int getLayerLength();
};

#endif