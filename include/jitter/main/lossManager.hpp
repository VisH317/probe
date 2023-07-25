#ifndef __LOSS_MANAGER_H__
#define __LOSS_MANAGER_H__

#include <map>
#include <utility>
#include <algorithm>

/**
 * @brief Manages loss at each iteration to determine when updates should be made to the weights of the network
 * 
 */
class LossManager {
    private:
        /**
         * @brief Map of losses at each network iteration <iteration, maximum loss achieved>
         * 
         */
        std::map<int, double> lossMap;

    public:
        /** Default constructor */
        LossManager() = default;

        /**
         * @brief Determines if an update should be made to the network at the current net iteration and alters the maximum achived loss at the iteration if necessary
         * 
         * @param loss - loss measured at the current weight change
         * @param netIteration - net iteration of update that occurred
         * @return true - loss should be updated, maximum loss at iteration is altered
         * @return false - loss should not be updated, maximum loss at iteration is not altered
         */
        bool shouldUpdate(double loss, int netIteration);


};


#endif