#pragma once

#include <string>
#include <optional>

struct DeviceInfo {
    std::optional<std::string> vendorId;
    std::optional<std::string> productId;
    std::optional<std::string> serial;
    std::optional<std::string> vendorName;
    std::optional<std::string> productName;
};