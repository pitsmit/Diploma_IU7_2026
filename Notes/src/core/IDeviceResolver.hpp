#pragma once

#include <optional>
#include <string>

class DeviceInfo;
enum MODE;

class IDeviceResolver {
public:
    virtual ~IDeviceResolver() = default;
    virtual std::optional<DeviceInfo> resolve(const char* devNode) = 0;
    virtual std::vector<std::string> getUsbDevNodes() = 0;
    virtual std::optional<std::string> getMountPoint(const std::string& devNode) = 0;
    virtual std::optional<MODE> getMountMode(const std::string& mountpoint) = 0;
};