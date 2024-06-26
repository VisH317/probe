#include "lockManager.hpp"

void LockManager::setLock(std::string id) {
    locks[id] = true;
}

void LockManager::removeLock(std::string id) {
    locks[id] = false;
}

bool LockManager::checkLock(std::string id) {
    if(locks.find(id)==locks.end()) {
        locks[id] = false;
        return false;
    }
    return locks[id];
}