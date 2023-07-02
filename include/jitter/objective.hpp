#ifndef __OBJECTIVE_H__
#define __OBJECTIVE_H__

#include <torch/torch.h>
#include "network.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>

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

        std::vector<double> losses;

        double desiredValue;

        void evaluateOutput();

        double computeAvgLoss(torch::Tensor in, torch::Tensor start);

    public:
        Objective(Network* net, std::vector<int>& idxs, torch::Tensor input, double desiredValue);

        std::pair<double, bool> loss();
        
        void reset();

        double getDiff();
};



#endif