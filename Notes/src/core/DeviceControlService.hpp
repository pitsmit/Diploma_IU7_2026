#pragma once

#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountPointBuilder.hpp"
#include "MountUtils.hpp"

class DeviceControlService {
private:
    DeviceManager& deviceManager_;
    PolicyManager& policyManager_;
    MountRegistry& mountRegistry_;
    MountUtils& mountUtils_;

public:
    DeviceControlService(
        DeviceManager& deviceManager,
        PolicyManager& policyManager,
        MountRegistry& mountRegistry,
        MountUtils& mountUtils
    ) :
        deviceManager_(deviceManager),
        policyManager_(policyManager),
        mountRegistry_(mountRegistry),
        mountUtils_(mountUtils)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event", event.type == EventType::INSERT ? "INSERT" : "REMOVE");

        if (event.type == EventType::INSERT) {
            bool allowed = policyManager_.isAllowed(event.dev);
            std::string mountPoint =
                MountPointBuilder::build(event.dev);
            MountPointBuilder::ensureExists(mountPoint);
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