#pragma once

#include <string>

#include "IMountSystem.hpp"

class MockMountSystem : public IMountSystem {
public:
    bool syncCalled = false;

    bool mountCalled = false;
    bool umountCalled = false;

    std::string lastDev;
    std::string lastTarget;
    std::string lastFs;
    int lastFlags;
    std::string lastOpts;

    std::string fsType = "ext4";
    int mountResult = 0;
    int umountResult = 0;

    void sync() override {
        syncCalled = true;
    }

    std::string getFsType(const std::string& devnode) override {
        lastDev = devnode;
        return fsType;
    }

    int mount(
        const std::string& dev,
        const std::string& target,
        const std::string& fs,
        bool flags,
        const std::string& opts) override
    {
        mountCalled = true;
        lastDev = dev;
        lastTarget = target;
        lastFs = fs;
        lastFlags = flags;
        lastOpts = opts;
        return mountResult;
    }

    int remount(
        const std::string& dev,
        const std::string& target,
        const std::string& fs,
        bool flags) override
    {
        mountCalled = true;
        lastDev = dev;
        lastTarget = target;
        lastFs = fs;
        lastFlags = flags;
        return mountResult;
    }

    int umount(const std::string& target) override {
        umountCalled = true;
        lastTarget = target;
        return umountResult;
    }
};