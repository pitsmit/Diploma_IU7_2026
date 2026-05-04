#pragma once

#include <string>
#include <optional>

#include "DeviceInfo.hpp"

struct Device {
    size_t id;
    DeviceInfo info;
    std::optional<std::string> validTo;
    std::optional<std::string> devNode;
};