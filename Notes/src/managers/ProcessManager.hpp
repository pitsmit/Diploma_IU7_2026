#pragma once

#include "ProcessRepository.hpp"

class ProcessManager {
private:
    ProcessRepository repo;

public:
    explicit ProcessManager(DBConnection& db)
        : repo(db) {}

    void addToWhitelist(const Process& p) {
        repo.add(p);
    }

    std::vector<Process> getWhitelist() {
        return repo.getAll();
    }

    void removeFromWhitelist(size_t id) {
        repo.remove(id);
    }

    void patchValidTo(size_t id, const std::string& validTo) {
        repo.updateValidTo(id, validTo);
    }
};