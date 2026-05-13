#pragma once

#include <optional>
#include <string>

#include "PolicyManager.hpp"
#include "MountUtils.hpp"
#include "IDeviceResolver.hpp"
#include "MountPointBuilder.hpp"
#include "MountRecord.hpp"
#include "Exceptions.hpp"

class MountService {
private:
    PolicyManager& policyManager_;
    MountUtils& mountUtils_;
    IDeviceResolver& resolver_;

public:
    MountService(
        PolicyManager& policyManager,
        MountUtils& mountUtils,
        IDeviceResolver& resolver
    ) :
        policyManager_(policyManager),
        mountUtils_(mountUtils),
        resolver_(resolver)
    {}

    std::optional<MountRecord>
    mount(const std::string& devNode)
    {
        std::optional<DeviceInfo> info =
            resolver_.resolve(devNode.c_str());
        
        if (!info)
            throw ResolveInfoError(("Coud not extract deviceInfo from devnode: " + devNode).c_str());

        std::string mountPoint =
            MountPointBuilder::build(*info);
        MountPointBuilder::ensureExists(mountPoint);
        int id = policyManager_.isAllowed(*info);

        mountUtils_.mountDevice(
            devNode,
            mountPoint,
            !id
        );

        auto record =
            MountRecordBuilder()
                .withDevNode(devNode)
                .withId(id)
                .withInfo(*info)
                .withMountPoint(mountPoint)
                .withMode(id ? MODE::RW : MODE::RO)
                .build();

        return record;
    }

    void unmount(const std::string& mountPoint)
    {
        mountUtils_.handleUnmount(mountPoint);
    }

    std::optional<MountRecord> remount(MountRecord &record)
    {
        unmount(record.mountPoint);
        return mount(record.devNode);
    }

    void remountSimple(MountRecord &record)
    {
        mountUtils_.remount(record);
    }
};