#pragma once

#include "DeviceCommands.hpp"
#include "CommandContext.hpp"
#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"
#include "IMountSystem.hpp"

class Facade {
private: 
    DeviceManager deviceManager;
    PolicyManager policyManager;
    MountRegistry mountRegistry;
    MountUtils mountUtils;
    CommandContext ctx;

public:
    Facade(DBConnection &db, IMountSystem &sys)
        : deviceManager(db),
          policyManager(db),
          mountUtils(sys),
          ctx {deviceManager, mountRegistry, mountUtils}
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() { return deviceManager; }
    PolicyManager& policies() { return policyManager; }
    MountRegistry& registry() { return mountRegistry; }
    MountUtils& utils() { return mountUtils; }
};