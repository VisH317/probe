#ifndef __LOSS_MANAGER_H__
#define __LOSS_MANAGER_H__

#include <map>
#include <utility>
#include <algorithm>

class LossManager {
    private:
        
        std::map<int, double> lossMap;

    public:
        LossManager() = default;

        bool shouldUpdate(double loss, int netIteration);


};


#endif