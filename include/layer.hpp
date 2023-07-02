#ifndef __LAYER_H__
#define __LAYER_H__

#include <torch/torch.h>
#include <utility>
#include <vector>
#include <string>
#include <tuple>

/**
 * @brief wrapper around torch::nn::Linear to provide probing mechanisms with uuids for specific neuron analysis
 * 
 */
class Layer {
    private:
        /** torch linear layer that the Layer object wraps around */
        torch::nn::Linear layer = nullptr;

        /** list of uuids for each neuron in the layer */
        std::vector<std::string> id;

        /**
         * @brief private method generate an auto-incrementing id for each neuron at layer object construction
         * 
         * @return std::string 
         */
        std::string generateId();

        /** input size */
        int in;

        /** returned output size */
        int out;

        /** static to represent current autoincrement value for future instantiations */
        static long long curId;

    public: 

        /**
         * @brief Construct a new Layer object
         * 
         * @param in (int) of input features
         * @param out (int) Number of output features
         */
        Layer(int in, int out); // divide by out features: determines how much of each in feature

        /**
         * @brief Destroy the Layer object
         * 
         */
        ~Layer() = default;

        /**
         * @brief Get the linear layer that the Layer object wraps aruond
         * 
         * @return torch::nn::Linear& to torch layer representation under the wrappper 
         */
        torch::nn::Linear& getLayer();

        /**
         * @brief Get a specific neuron, its weights and bias for one specific feature in the layer
         * 
         * @param num (int) index of the neuron in the layer
         * @return std::tuple<std::string, torch::Tensor, torch::Tensor> tuple of id, weight tensor, and bias tensor
         */
        std::tuple<std::string, torch::Tensor, torch::Tensor> getNeuron(int num);

        /**
         * @brief Get the dimensions of the layer
         * 
         * @return std::pair<int, int> pair of input features, output feature
         */
        std::pair<int, int> getDims();
        // probe method
};

#endif