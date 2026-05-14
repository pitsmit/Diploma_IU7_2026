#pragma once

#include <string>

class IWebSocketServer {
public:
    virtual void start() = 0;
    virtual void broadcast(const std::string& message) = 0;
};