#ifndef __LOGS__
#define __LOGS__

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
// #include "RDataFrame.h"
// #include "TFile.h"

class Logger {
    private:

        /** writing update logs file in csv format */
        std::ofstream file;

        /** iteration log file (currently unused) */
        std::ofstream runtimeFile;

        // std::unique_ptr<TFile> rootFile;

        // std::unique_ptr<ROOT::RDataFrame>

    public:

        /**
         * @brief Construct a new Logger object
         * 
         */
        Logger();

        /**
         * @brief Destroy the Logger object
         * 
         */
        ~Logger();

        /**
         * @brief Write an update to the updates.csv file
         * 
         * @param layer layer of neuron with update
         * @param id id of neuron in layer that is being updated
         * @param update update performed
         * @param loss resulting loss from update
         */
        void writeUpdate(int layer, std::string id, double update, double loss);
};

#endif