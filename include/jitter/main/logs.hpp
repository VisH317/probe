#ifndef __LOGS__
#define __LOGS__

#include <fstream>
#include <iostream>
#include <string>

class Logger {
    private:
        std::ofstream file;
        std::string fileName;
        std::ofstream runtimeFile;

    public:
        Logger();
        ~Logger();
        void writeUpdate(int layer, std::string id, double update, double loss);
};

#endif