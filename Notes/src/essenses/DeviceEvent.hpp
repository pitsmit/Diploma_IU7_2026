#pragma once

#include <string>

#include "DeviceInfo.hpp"

enum EventType {
    INSERT,
    REMOVE
};

struct DeviceEvent {
    EventType type;
    std::string timestamp;
    std::string devNode;
    std::string mountPoint;
    DeviceInfo dev;
};