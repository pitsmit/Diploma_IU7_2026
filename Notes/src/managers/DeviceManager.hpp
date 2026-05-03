#pragma once

#include "DeviceRepository.hpp"
#include "MountEvent.hpp"

#include <vector>
#include <iostream>

class DeviceManager {
private:
    DeviceRepository repo;

public:
    explicit DeviceManager(DBConnection& db)
        : repo(db) {}

    void addToWhitelist(const Device& d) {
        repo.add(d);
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