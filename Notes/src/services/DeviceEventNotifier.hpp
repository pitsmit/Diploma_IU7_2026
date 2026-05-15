#pragma once

#include "IWebSocketServer.hpp"
#include "MountRecord.hpp"
#include "JsonUtils.hpp"

#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class DeviceEventNotifier {
private:
    IWebSocketServer& ws_;

public:
    explicit DeviceEventNotifier(IWebSocketServer& ws)
        : ws_(ws)
    {}

    void notifyInsert(const MountRecord& record)
    {
        json payload = {
            { "type", "insert" },
            { "data", record }
        };

        ws_.broadcast(payload.dump());
    }

    void notifyRemove(const std::string& mountPoint)
    {
        json payload = {
            { "type", "remove" },
            {
                "data",
                {
                    { "mountPoint", mountPoint }
                }
            }
        };

        ws_.broadcast(payload.dump());
    }
};