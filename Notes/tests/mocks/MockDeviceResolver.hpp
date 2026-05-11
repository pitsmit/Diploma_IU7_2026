#pragma once

#include <optional>
#include <string>
#include <vector>

#include "IDeviceResolver.hpp"
#include "DeviceInfo.hpp"
#include "MountRecord.hpp"

class MockDeviceResolver : public IDeviceResolver {
private:
    DeviceInfo info_;
    bool shouldReturn_ = true;

public:
    void setResult(const DeviceInfo& info)
    {
        info_ = info;
        shouldReturn_ = true;
    }

    void setEmpty()
    {
        shouldReturn_ = false;
    }

    std::optional<DeviceInfo>
    resolve(const char* /*devNode*/) override
    {
        if (!shouldReturn_) {
            return std::nullopt;
        }
        return info_;
    }

    std::optional<std::string> getMountPoint(const std::string& devNode)
    {
        return nullptr;
    }

    std::optional<MODE> getMountMode(const std::string& mountpoint)
    {
        std::optional<MODE> a = MODE::RO;
        return a;
    }

    std::vector<std::string> getUsbDevNodes()
    {
        return {
            "/dev/sdb1",
            "/dev/sdc1",
            "/dev/sdd1"
        };
    }
};