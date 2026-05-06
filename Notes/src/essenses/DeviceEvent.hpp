#pragma once

#include <string>

#include "DeviceInfo.hpp"

enum EventType {
    INSERT,
    REMOVE
};

struct DeviceEvent {
    EventType type;
    std::string devNode;
    DeviceInfo dev;
};


class DeviceEventBuilder {
private:
    DeviceEvent event_;

public:
    DeviceEventBuilder& withType(EventType type) {
        event_.type = type;
        return *this;
    }

    DeviceEventBuilder& withDevNode(const std::string& node) {
        event_.devNode = node;
        return *this;
    }

    DeviceEventBuilder& withDeviceInfo(const DeviceInfo& info) {
        event_.dev = info;
        return *this;
    }

    DeviceEvent build() {
        return event_;
    }
};