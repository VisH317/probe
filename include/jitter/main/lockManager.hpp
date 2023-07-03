#ifndef __LOCK_MANAGER_H__
#define __LOCK_MANAGER_H__

#include <map>
#include <string>

class LockManager {
    private: 
        std::map<std::string, bool> locks;

    public:
        LockManager();
        LockManager(LockManager&&) = default;

        void setLock();
        void removeLock();
        void checkLock();
}

#endif