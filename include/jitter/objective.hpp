#ifndef __OBJECTIVE_H__
#define __OBJECTIVE_H__

#include <torch/torch.h>
#include "network.hpp"
#include <iostream>
#include <vector>

// functor object that takes in a network pointer as a parameter, computes new objective function (distance from original), keeps track of all objective values

class Objective {
    private:
        Network* net;

        /** List of output neurons being observed */
        std::vector<int> idxs;

        /** Input set to optimize on */
        torch::Tensor input;

        /** Concatenated list of all outputs based on the required output neurons being observed */
        std::vector<torch::Tensor> outputs;  

    public:
        Objective(Network* net, std::vector<int>& idxs, torch::Tensor input) : net(net), idxs(idxs), input(input) {};

        double loss();
        // void test();
};



#endif