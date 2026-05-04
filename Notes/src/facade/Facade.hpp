#pragma once

#include "DeviceCommands.hpp"
#include "CommandContext.hpp"
#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "MountRegistry.hpp"

class Facade {
private:
    DBConnection db;
    DeviceManager deviceManager;
    PolicyManager policyManager;
    MountRegistry mountRegistry;
    CommandContext ctx;

public:
    Facade()
        : db("app.db"),
          deviceManager(db),
          policyManager(db),
          ctx {deviceManager, mountRegistry}
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() { return deviceManager; }
    PolicyManager& policies() { return policyManager; }
    MountRegistry& registry() { return mountRegistry; }
};