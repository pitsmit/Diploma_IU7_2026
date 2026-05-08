#pragma once

#include "DeviceInfo.hpp"
#include "DeviceRepository.hpp"

class PolicyManager {
private:
    DeviceRepository repo;

public:
    explicit PolicyManager(DBConnection& db)
        : repo(db) {}

    int isAllowed(const DeviceInfo& dev) { 
        return repo.exists(dev);
    }
};