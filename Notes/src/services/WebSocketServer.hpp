#pragma once

#include "IWebSocketServer.hpp"

#include <ixwebsocket/IXWebSocketServer.h>
#include <ixwebsocket/IXConnectionState.h>

#include <mutex>
#include <set>
#include <string>

class WebSocketServer: public IWebSocketServer {
private:
    ix::WebSocketServer server_;

    std::mutex mutex_;

    std::set<ix::WebSocket*> clients_;

public:
    explicit WebSocketServer(int port)
        : server_(port)
    {}

    void start()
    {
        server_.setOnClientMessageCallback(
            [this](
                std::shared_ptr<ix::ConnectionState>,
                ix::WebSocket& webSocket,
                const ix::WebSocketMessagePtr& msg
            )
            {
                if (msg->type == ix::WebSocketMessageType::Open) {
                    std::lock_guard lock(mutex_);

                    clients_.insert(&webSocket);
                }
                else if (msg->type == ix::WebSocketMessageType::Close) {
                    std::lock_guard lock(mutex_);

                    clients_.erase(&webSocket);
                }
            });

        auto result = server_.listen();

        if (!result.first) {
            throw std::runtime_error(result.second);
        }

        server_.start();
    }

    void broadcast(const std::string& message)
    {
        std::lock_guard lock(mutex_);

        for (auto* client : clients_) {
            client->send(message);
        }
    }
};