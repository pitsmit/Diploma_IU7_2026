#pragma once
#include <string>
#include <nlohmann/json.hpp>

struct Device {
    size_t id;
    std::string name;
    std::string vendorId;
    std::string productId;
    std::string serial;
    std::string validTo;
};

inline void to_json(nlohmann::json& j, const Device& d) {
    j = nlohmann::json{
        {"id", d.id},
        {"name", d.name},
        {"vendorId", d.vendorId},
        {"productId", d.productId},
        {"serial", d.serial},
        {"validTo", d.validTo}
    };
}