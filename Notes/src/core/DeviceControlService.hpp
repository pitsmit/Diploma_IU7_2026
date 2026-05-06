#pragma once

#include "PolicyManager.hpp"
#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountPointBuilder.hpp"
#include "MountUtils.hpp"

class DeviceControlService {
private:
    PolicyManager& policyManager_;
    MountRegistry& mountRegistry_;
    MountUtils& mountUtils_;

public:
    DeviceControlService(
        PolicyManager& policyManager,
        MountRegistry& mountRegistry,
        MountUtils& mountUtils
    ) :
        policyManager_(policyManager),
        mountRegistry_(mountRegistry),
        mountUtils_(mountUtils)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event", event.type == EventType::INSERT ? "INSERT" : "REMOVE");

        if (event.type == EventType::INSERT) {
            std::string mountPoint =
                MountPointBuilder::build(event.dev);
            MountPointBuilder::ensureExists(mountPoint);
            bool allowed = policyManager_.isAllowed(event.dev);
            mountUtils_.mountDevice(
                event.devNode,
                mountPoint,
                !allowed
            );
            mountRegistry_.add(event.devNode, mountPoint);
        }
        else if (event.type == EventType::REMOVE) {
            std::string mountPoint;
            if (mountRegistry_.get(event.devNode, mountPoint)) {
                mountUtils_.handleUnmount(mountPoint);
                mountRegistry_.remove(event.devNode);
            }
            else {
                mylog->warn("No mount point for {}", event.devNode);
            }
        }
    }
};