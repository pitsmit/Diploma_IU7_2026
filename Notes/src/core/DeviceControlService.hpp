#pragma once

#include <sys/mount.h>
#include <unistd.h>

#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "MountEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"

class DeviceControlService {
private:
    DeviceManager& deviceManager_;
    PolicyManager& policyManager_;
    MountRegistry& mountRegistry_;

public:
    DeviceControlService(
        DeviceManager& deviceManager,
        PolicyManager& policyManager,
        MountRegistry& mountRegistry
    ) :
        deviceManager_(deviceManager),
        policyManager_(policyManager),
        mountRegistry_(mountRegistry)
    {}

    void handleEvent(const MountEvent& event)
    {
        mylog->info("Start handle event");

        if (event.type == EventType::INSERT) {
            bool allowed = policyManager_.isAllowed(event.dev);
            std::string mountPoint =
                MountPointBuilder::build(event.dev);
            MountPointBuilder::ensureExists(mountPoint);
            MountUtils::mountDevice(
                event.devNode,
                mountPoint,
                !allowed
            );
            mountRegistry_.add(event.devNode, mountPoint);
        }
        else if (event.type == EventType::REMOVE) {
            std::string mountPoint;
            if (mountRegistry_.get(event.devNode, mountPoint)) {
                MountUtils::handleUnmount(mountPoint);
                mountRegistry_.remove(event.devNode);
            }
            else {
                mylog->warn("No mount point for devNode: " + event.devNode);
            }
        }
    }
};