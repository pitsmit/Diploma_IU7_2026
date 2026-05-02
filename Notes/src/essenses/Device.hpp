#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "DeviceInfo.hpp"

struct Device {
    size_t id;
    DeviceInfo info;
    std::string validTo;
};

inline void to_json(nlohmann::json& j, const DeviceInfo& info)
{
    j = nlohmann::json::object();

    j["vendorId"] =
        info.vendorId
            ? nlohmann::json(*info.vendorId)
            : nlohmann::json(nullptr);

    j["productId"] =
        info.productId
            ? nlohmann::json(*info.productId)
            : nlohmann::json(nullptr);

    j["serial"] =
        info.serial
            ? nlohmann::json(*info.serial)
            : nlohmann::json(nullptr);

    j["vendorName"] =
        info.vendorName
            ? nlohmann::json(*info.vendorName)
            : nlohmann::json(nullptr);

    j["productName"] =
        info.productName
            ? nlohmann::json(*info.productName)
            : nlohmann::json(nullptr);
}

inline void to_json(nlohmann::json& j, const Device& d)
{
    j = nlohmann::json{
        {"id", d.id},
        {"info", d.info},
        {"validTo", d.validTo}
    };
}