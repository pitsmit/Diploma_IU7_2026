#pragma once

#include "DeviceCommands.hpp"
#include "CommandContext.hpp"
#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"
#include "IMountSystem.hpp"
#include "IDeviceResolver.hpp"
#include "MountManager.hpp"

class Facade {
private: 
    DeviceManager deviceManager;
    PolicyManager policyManager;
    MountRegistry mountRegistry;
    MountUtils mountUtils;
    MountManager mountManager;
    CommandContext ctx;

public:
    Facade(DBConnection &db, IMountSystem &sys, IDeviceResolver &res)
        : deviceManager(db),
          policyManager(db),
          mountRegistry(db),
          mountUtils(sys),
          mountManager(policyManager, mountUtils, res),
          ctx {deviceManager, mountRegistry, mountManager}
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() { return deviceManager; }
    PolicyManager& policies() { return policyManager; }
    MountManager& mounts() { return mountManager; }
    MountRegistry& registry() { return mountRegistry; }
    MountUtils& utils() { return mountUtils; }
};