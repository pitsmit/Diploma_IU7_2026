#pragma once

#include <string>
#include <optional>

#include "EventType.hpp"

struct SecurityEvent {
    EventType type;

    std::string timestamp;

    std::string mountPath;
    std::string deviceNode;

    std::optional<std::string> vendorId;
    std::optional<std::string> productId;
    std::optional<std::string> serial;
};