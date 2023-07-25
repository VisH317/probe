#ifndef __LOCK_MANAGER_H__
#define __LOCK_MANAGER_H__

#include <map>
#include <string>

/**
 * @brief manages locks on neurons during concurrent changes to prevent repeated changes [DEPRECATED]
 * 
 */
class LockManager {
    private: 
        /** map of neuron UUIDs to lock value (being used or not) */
        std::map<std::string, bool> locks;

    public:
        /** basic constructor + move constructor */
        LockManager();
        LockManager(LockManager&&) = default;

        /**
         * @brief Set the Lock object key to true or create the lock value if it does not exist
         * 
         * @param id - id of neuron
         */
        void setLock(std::string id);

        /**
         * @brief Set the lock object key to false if exists
         * 
         * @param id - id of neuron to set to false
         */
        void removeLock(std::string id);

        /**
         * @brief Check if a lock is true or false
         * 
         * @param id - id to check
         * @return true - neuron is currently being used
         * @return false - neuron is currently available for alteration
         */
        bool checkLock(std::string id);
};

#endif