#ifndef __LOG_MANAGER_H__
#define __LOG_MANAGER_H__

#include <map>
#include <utility>

class LogManager {
    private:
        
        std::map<int, double> lossMap;

    public:
        LogManager() = default;

        bool shouldUpdate();


}


#endif