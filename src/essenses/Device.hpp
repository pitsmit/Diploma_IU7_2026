#pragma once

#include <string>
#include <optional>

#include "DeviceInfo.hpp"

struct Device {
    size_t id;
    DeviceInfo info;
    std::optional<std::string> validTo;
};

class DeviceBuilder {
private:
    Device device_{};

public:
    DeviceBuilder& withId(size_t id) {
        device_.id = id;
        return *this;
    }

    DeviceBuilder& withInfo(const DeviceInfo& info) {
        device_.info = info;
        return *this;
    }

    DeviceBuilder& withValidTo(const std::string& validTo) {
        device_.validTo = validTo;
        return *this;
    }

    Device build() { return device_; }
};