#pragma once

#include <string>
#include <optional>
#include <nlohmann/json.hpp>

#include "DeviceInfo.hpp"
#include "Device.hpp"

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
        {"validTo", *d.validTo},
        {"mountPath", *d.mountPath}
    };
}


inline void from_json(const nlohmann::json& j, DeviceInfo& info)
{
    if (j.contains("vendorId") && !j["vendorId"].is_null()) {
        info.vendorId = j["vendorId"].get<std::string>();
    }
    if (j.contains("productId") && !j["productId"].is_null()) {
        info.productId = j["productId"].get<std::string>();
    }
    if (j.contains("serial") && !j["serial"].is_null()) {
        info.serial = j["serial"].get<std::string>();
    }
    if (j.contains("vendorName") && !j["vendorName"].is_null()) {
        info.vendorName = j["vendorName"].get<std::string>();
    }
    if (j.contains("productName") && !j["productName"].is_null()) {
        info.productName = j["productName"].get<std::string>();
    }
}

inline void from_json(const nlohmann::json& j, Device& d)
{
    d.id = 0;

    if (j.contains("info")) {
        d.info = j["info"].get<DeviceInfo>();
    } else {
        from_json(j, d.info);
    }

    if (j.contains("validTo") && !j["validTo"].is_null()) {
        d.validTo = j["validTo"].get<std::string>();
    }

    if (j.contains("mountPath") && !j["mountPath"].is_null()) {
        d.mountPath = j["mountPath"].get<std::string>();
    }
}