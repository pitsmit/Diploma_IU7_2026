#pragma once

#include "PolicyManager.hpp"
#include "DeviceEvent.hpp"
#include "DevLogger.hpp"
#include "MountRegistry.hpp"
#include "MountPointBuilder.hpp"
#include "IDeviceResolver.hpp"
#include "MountRecord.hpp"
#include "MountUtils.hpp"

class DeviceControlService {
private:
    PolicyManager& policyManager_;
    MountRegistry& mountRegistry_;
    MountUtils& mountUtils_;
    IDeviceResolver& resolver_;

public:
    DeviceControlService(
        PolicyManager& policyManager,
        MountRegistry& mountRegistry,
        MountUtils& mountUtils,
        IDeviceResolver& resolver
    ) :
        policyManager_(policyManager),
        mountRegistry_(mountRegistry),
        mountUtils_(mountUtils),
        resolver_(resolver)
    {}

    void handleEvent(const DeviceEvent& event)
    {
        mylog->info("Start handle {} event", event.type == EventType::INSERT ? "INSERT" : "REMOVE");

        if (event.type == EventType::INSERT) {
            std::optional<DeviceInfo> info = resolver_.resolve(event.devNode);
            std::string mountPoint = MountPointBuilder::build(*info);
            MountPointBuilder::ensureExists(mountPoint);

            int id = policyManager_.isAllowed(*info);
            mountUtils_.mountDevice(
                event.devNode,
                mountPoint,
                !id
            );
            auto record = MountRecordBuilder()
                            .withDevNode(event.devNode)
                            .withId(id)
                            .withInfo(*info)
                            .withMountPoint(mountPoint)
                            .build();

            mountRegistry_.add(record);
        }
        else if (event.type == EventType::REMOVE) {
            if (std::optional<MountRecord> record = mountRegistry_.getByDevNode(event.devNode)) {
                mountUtils_.handleUnmount((*record).mountPoint);
                mountRegistry_.removeByDevNode(event.devNode);
            }
            else {
                mylog->warn("No mount point for {}", event.devNode);
            }
        }
    }
};