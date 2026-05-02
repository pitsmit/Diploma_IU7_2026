// ============================================================
// DeviceInfo.hpp
// ============================================================

#pragma once

#include <string>
#include <optional>

struct DeviceInfo {
    std::string devNode;

    std::optional<std::string> vendorId;
    std::optional<std::string> productId;
    std::optional<std::string> serial;
};