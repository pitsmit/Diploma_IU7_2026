#pragma once

#include <optional>

#include "DeviceInfo.hpp"

class IDeviceResolver {
public:
    virtual ~IDeviceResolver() = default;
    virtual std::optional<DeviceInfo> resolve(const char* devNode) = 0;
};