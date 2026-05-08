#pragma once

#include "DeviceRepository.hpp"
#include <vector>

class DeviceManager {
private:
    DeviceRepository repo;

public:
    explicit DeviceManager(DBConnection& db)
        : repo(db) {}

    int addToWhitelist(const MountRecord& d, std::optional<std::string> vld) {
        return repo.add(d, vld);
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
};