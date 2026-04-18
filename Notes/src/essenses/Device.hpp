#pragma once
#include <string>

struct Device {
    size_t id;
    std::string name;
    std::string vendorId;
    std::string productId;
    std::string serial;
    std::string validTo;
};