#pragma once

#include <string>

#include "DeviceInfo.hpp"

struct MountEvent {
    std::string timestamp;
    std::string mountPath;
    DeviceInfo dev;
};