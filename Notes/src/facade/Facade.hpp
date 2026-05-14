#pragma once

#include "DeviceCommands.hpp"
#include "CommandContext.hpp"
#include "DeviceManager.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"
#include "IMountSystem.hpp"
#include "IDeviceResolver.hpp"
#include "MountService.hpp"

class Facade {
private: 
    DeviceManager deviceManager;
    MountRegistry mountRegistry;
    MountUtils mountUtils;
    MountService mountManager;
    CommandContext ctx;

public:
    Facade(DBConnection &db, IMountSystem &sys, IDeviceResolver &res)
        : deviceManager(db),
          mountRegistry(db),
          mountUtils(sys),
          mountManager(deviceManager, mountUtils, res),
          ctx {deviceManager, mountRegistry, mountManager}
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() { return deviceManager; }
    MountService& mounts() { return mountManager; }
    MountRegistry& registry() { return mountRegistry; }
    MountUtils& utils() { return mountUtils; }
};