#pragma once

#include "IWebSocketServer.hpp"

#include <string>

class MockWebSocketServer: public IWebSocketServer {
private:

public:
    explicit MockWebSocketServer(int port) {}

    void start()
    {

    }

    void broadcast(const std::string& message)
    {

    }
};