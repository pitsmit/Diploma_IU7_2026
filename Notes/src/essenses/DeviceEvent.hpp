#pragma once

#include <string>

enum EventType {
    INSERT,
    REMOVE
};

struct DeviceEvent {
    EventType type;
    std::string devNode;
};


class DeviceEventBuilder {
private:
    DeviceEvent event_;

public:
    DeviceEventBuilder& withType(EventType type) {
        event_.type = type;
        return *this;
    }

    DeviceEventBuilder& withDevNode(const char *node) {
        event_.devNode = node;
        return *this;
    }

    DeviceEvent build() {
        return event_;
    }
};