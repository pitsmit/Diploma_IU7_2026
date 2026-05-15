#pragma once

#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountService.hpp"
#include "DeviceEventNotifier.hpp"

class DeviceControlService {
private:
    MountRegistryManager& mountRegistry_;
    MountService& mountManager_;
    DeviceEventNotifier& notifier_;

public:
    DeviceControlService(
        MountRegistryManager& mountRegistry,
        MountService& mountManager,
        DeviceEventNotifier& notifier
    ) :
        mountRegistry_(mountRegistry),
        mountManager_(mountManager),
        notifier_(notifier)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event with devnode {}", event.type == EventType::INSERT ? "INSERT" : "REMOVE", event.devNode);

        if (event.type == EventType::INSERT) {
            auto record = mountManager_.mount(event.devNode);
            mountRegistry_.add(*record);
            notifier_.notifyInsert(*record);
        }
        else if (event.type == EventType::REMOVE) {
            if (std::optional<std::string> mountPoint = 
                    mountRegistry_.getMountPointByDevNode(event.devNode)) {
                mountManager_.unmount(*mountPoint);

                notifier_.notifyRemove(*mountPoint);

                mountRegistry_.removeByDevNode(event.devNode);
            }
            else {
                mylog->warn("No mount point for {}", event.devNode);
            }
        }
    }
};