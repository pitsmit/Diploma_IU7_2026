#pragma once

#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountPointBuilder.hpp"
#include "MountRecord.hpp"
#include "MountService.hpp"
#include "IWebSocketServer.hpp"
#include "JsonUtils.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class DeviceControlService {
private:
    MountRegistry& mountRegistry_;
    MountService& mountManager_;
    IWebSocketServer& ws_;

public:
    DeviceControlService(
        MountRegistry& mountRegistry,
        MountService& mountManager,
        IWebSocketServer& ws
    ) :
        mountRegistry_(mountRegistry),
        mountManager_(mountManager),
        ws_(ws)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event with devnode {}", event.type == EventType::INSERT ? "INSERT" : "REMOVE", event.devNode);

        if (event.type == EventType::INSERT) {
            auto record = mountManager_.mount(event.devNode);
            mountRegistry_.add(*record);

            json payload = {
                { "type", "insert" },
                { "data", *record }
            };

            ws_.broadcast(payload.dump());
        }
        else if (event.type == EventType::REMOVE) {
            if (std::optional<std::string> mountPoint = 
                    mountRegistry_.getMountPointByDevNode(event.devNode)) {
                mountManager_.unmount(*mountPoint);

                json payload = {
                    { "type", "remove" },
                    {
                        "data",
                        {
                            { "mountPoint", *mountPoint }
                        }
                    }
                };

                ws_.broadcast(payload.dump());

                mountRegistry_.removeByDevNode(event.devNode);
            }
            else {
                mylog->warn("No mount point for {}", event.devNode);
            }
        }
    }
};