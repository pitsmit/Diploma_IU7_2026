#pragma once

#include <vector>
#include <string>
#include <optional>

class MountRecord;

#include "MountRepository.hpp"

class MountRegistry {
private:
    MountRepository repo;

public:
    explicit MountRegistry(DBConnection& db)
            : repo(db) {}

    void add(const MountRecord& record)
    {
        repo.add(record);
    }

    std::optional<MountRecord> getByDevNode(
        const std::string& devNode)
    {
        return repo.getByDevNode(devNode);
    }

    std::optional<MountRecord> getById(
        const size_t& id)
    {
        return repo.getById(id);
    }

    void removeByDevNode(const std::string& devNode)
    {
        repo.removeByDevNode(devNode);
    }

    void recreate(const MountRecord& record)
    {
        removeByDevNode(record.devNode);
        add(record);
    }

    std::vector<MountRecord> getAll()
    {
        return repo.getAll();
    }

    void refresh(const MountRecord& record)
    {
        repo.update(record);
    }
};