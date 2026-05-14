#pragma once

#include "DeviceRepository.hpp"
#include "DeviceInfo.hpp"

#include <vector>

class DeviceManager {
private:
    DeviceRepository repo;

public:
    explicit DeviceManager(DBConnection& db)
        : repo(db) {}

    int addToWhitelist(const DeviceInfo& i, std::optional<std::string> validTo) {
        return repo.add(i, validTo);
    }

    std::vector<Device> getWhitelist() {
        return repo.getAll();
    }

    void removeFromWhitelist(size_t id) {
        repo.remove(id);
    }

    void patchValidTo(size_t id, std::optional<std::string> validTo) {
        repo.updateValidTo(id, validTo);
    }

    int isAllowed(const DeviceInfo& dev) { 
        return repo.findActiveId(dev);
    }
};