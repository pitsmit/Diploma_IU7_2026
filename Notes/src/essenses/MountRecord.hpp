#pragma once

#include <string>

#include "DeviceInfo.hpp"

struct MountRecord {
    size_t id;
    std::string devNode;
    std::string mountPoint;
    DeviceInfo info;
};

class MountRecordBuilder {
private:
    MountRecord record_{};

public:
    MountRecordBuilder& withId(size_t id)
    {
        record_.id = id;
        return *this;
    }

    MountRecordBuilder& withDevNode(
        const std::string& devNode)
    {
        record_.devNode = devNode;
        return *this;
    }

    MountRecordBuilder& withMountPoint(
        const std::string& mountPoint)
    {
        record_.mountPoint = mountPoint;
        return *this;
    }

    MountRecordBuilder& withInfo(
        const DeviceInfo& info)
    {
        record_.info = info;
        return *this;
    }

    MountRecord build()
    {
        return record_;
    }
};