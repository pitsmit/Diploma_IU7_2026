#pragma once
#include <string>
#include "MountEvent.hpp"

class PolicyManager {
private:
    DeviceRepository repo;

public:
    explicit PolicyManager(DBConnection& db)
        : repo(db) {}

    bool isAllowed([[maybe_unused]] const DeviceInfo dev) { 
        return repo.exists(dev);
    }
};