#pragma once

#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountPointBuilder.hpp"
#include "MountRecord.hpp"
#include "MountService.hpp"

class DeviceControlService {
private:
    MountRegistry& mountRegistry_;
    MountService& mountManager_;

public:
    DeviceControlService(
        MountRegistry& mountRegistry,
        MountService& mountManager
    ) :
        mountRegistry_(mountRegistry),
        mountManager_(mountManager)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event with devnode {}", event.type == EventType::INSERT ? "INSERT" : "REMOVE", event.devNode);

        if (event.type == EventType::INSERT) {
            auto record = mountManager_.mount(event.devNode);
            mountRegistry_.add(*record);
        }
        else if (event.type == EventType::REMOVE) {
            if (std::optional<MountRecord> record = mountRegistry_.getByDevNode(event.devNode)) {
                mountManager_.unmount(record->mountPoint);
                mountRegistry_.removeByDevNode(event.devNode);
            }
            else {
                mylog->warn("No mount point for {}", event.devNode);
            }
        }
    }
};