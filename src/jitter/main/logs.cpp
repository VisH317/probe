#include "logs.hpp"

Logger::Logger() {
    file.open("updates.csv");
    runtimeFile.open("iterations.log");
}

Logger::~Logger() {
    file.close();
    runtimeFile.close();
}

void Logger::writeUpdate(int layer, std::string id, double update, double loss) {
    file << layer << ", " << ", " << id << ", " << update << ", " << loss << "\n";
}