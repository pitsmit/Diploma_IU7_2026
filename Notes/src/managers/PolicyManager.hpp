#pragma once

#include "DeviceInfo.hpp"
#include "DeviceRepository.hpp"

class PolicyManager {
private:
    DeviceRepository repo;

public:
    explicit PolicyManager(DBConnection& db)
        : repo(db) {}

    bool isAllowed(const DeviceInfo& dev) { 
        return repo.exists(dev);
    }
};