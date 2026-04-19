#pragma once

#include <string>

enum class EventType {
    INSERT,
    REMOVE,
    READ,
    WRITE,
    EXECUTE
};

inline std::string toString(EventType type) {
    switch (type) {
        case EventType::INSERT: return "INSERT";
        case EventType::REMOVE: return "REMOVE";
        case EventType::READ:   return "READ";
        case EventType::WRITE:  return "WRITE";
        case EventType::EXECUTE:return "EXECUTE";
    }
    return "";
}

inline EventType fromString(const std::string& str) {
    if (str == "INSERT") return EventType::INSERT;
    if (str == "REMOVE") return EventType::REMOVE;
    if (str == "READ")   return EventType::READ;
    if (str == "WRITE")  return EventType::WRITE;
    if (str == "EXECUTE")return EventType::EXECUTE;

    throw std::runtime_error("Unknown EventType: " + str);
}