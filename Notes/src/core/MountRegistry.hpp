#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <optional>
#include <algorithm>

#include "MountRecord.hpp"
#include "DevLogger.hpp"

class MountRegistry {
private:
    std::vector<MountRecord> records_;
    mutable std::mutex mtx_;

public:
    void add(const MountRecord& record)
    {
        std::lock_guard<std::mutex> lock(mtx_);

        auto it = std::find_if(
            records_.begin(),
            records_.end(),
            [&](const MountRecord& r) {
                return r.devNode == record.devNode;
            }
        );

        if (it != records_.end()) {
            *it = record;

            mylog->debug(
                "Updated mount record: devNode={}",
                record.devNode
            );
        }
        else {
            records_.push_back(record);

            mylog->debug(
                "Added mount record: devNode={}",
                record.devNode
            );
        }
    }

    std::optional<MountRecord> getByDevNode(
        const std::string& devNode)
    {
        std::lock_guard<std::mutex> lock(mtx_);

        auto it = std::find_if(
            records_.begin(),
            records_.end(),
            [&](const MountRecord& r) {
                return r.devNode == devNode;
            }
        );

        if (it == records_.end()) {
            mylog->debug(
                "Mount record not found for {}",
                devNode
            );

            return std::nullopt;
        }

        return *it;
    }

    std::optional<MountRecord> getById(
        const size_t& id)
    {
        std::lock_guard<std::mutex> lock(mtx_);

        auto it = std::find_if(
            records_.begin(),
            records_.end(),
            [&](const MountRecord& r) {
                return r.id == id;
            }
        );

        if (it == records_.end()) {
            mylog->debug(
                "Mount record not found for {}",
                id
            );

            return std::nullopt;
        }

        return *it;
    }

    void removeByDevNode(const std::string& devNode)
    {
        std::lock_guard<std::mutex> lock(mtx_);

        auto oldSize = records_.size();

        records_.erase(
            std::remove_if(
                records_.begin(),
                records_.end(),
                [&](const MountRecord& r) {
                    return r.devNode == devNode;
                }
            ),
            records_.end()
        );

        if (records_.size() != oldSize) {
            mylog->debug(
                "Removed mount record: devNode={}",
                devNode
            );
        }
    }

    std::vector<MountRecord> getAll()
    {
        std::lock_guard<std::mutex> lock(mtx_);

        return records_;
    }

    bool exists(const std::string& devNode)
    {
        std::lock_guard<std::mutex> lock(mtx_);

        return std::any_of(
            records_.begin(),
            records_.end(),
            [&](const MountRecord& r) {
                return r.devNode == devNode;
            }
        );
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lock(mtx_);

        return records_.size();
    }
};