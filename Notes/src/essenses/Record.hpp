#pragma once
#include <string>
#include "EventType.hpp"

enum class EventResult {
    SUCCESS,
    DENIED,
    ERROR
};

struct Record {
    size_t id;
    std::string datetime;
    EventType eventType;
    std::string devName;
    std::string vendorId;
    std::string productId;
    std::string serial;
    std::string procName;
    std::string procPath;
    std::string filePath;
    EventResult result;
};