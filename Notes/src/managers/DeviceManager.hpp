#pragma once

#include "DeviceRepository.hpp"
#include "SecurityEvent.hpp"
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

    void patchValidTo(size_t id, const std::string& validTo) {
        repo.updateValidTo(id, validTo);
    }

    void handleMount(const SecurityEvent& event)
    {
        std::cout
            << "[MOUNT] "
            << event.mountPath
            << "\n";

        if (event.vendorId) {
            std::cout
                << "VID: "
                << *event.vendorId
                << "\n";
        }

        if (event.productId) {
            std::cout
                << "PID: "
                << *event.productId
                << "\n";
        }
    }

    void handleUnmount(const SecurityEvent& event)
    {
        std::cout
            << "[UNMOUNT] "
            << event.mountPath
            << "\n";
    }
};