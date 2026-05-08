#pragma once

#include <optional>
#include <string>
#include "IDeviceResolver.hpp"
#include "DeviceInfo.hpp"

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
};