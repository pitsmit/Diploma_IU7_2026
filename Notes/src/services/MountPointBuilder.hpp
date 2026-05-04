#pragma once

#include <string>
#include <DeviceInfo.hpp>
#include <filesystem>

class MountPointBuilder {
public:
    static std::string build(const DeviceInfo& dev) {
        std::string base = "/media/dlp";
        std::string vid = dev.vendorId.value_or("unknown");
        std::string pid = dev.productId.value_or("unknown");
        std::string sn  = dev.serial.value_or("noserial");
        return base + "/" + vid + "_" + pid + "_" + sn;
    }

    static void ensureExists(const std::string& path) {
        std::filesystem::create_directories(path);
    }
};