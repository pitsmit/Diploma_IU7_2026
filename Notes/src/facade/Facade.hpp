#pragma once

#include "Command.hpp"
#include "CommandContext.hpp"

#include "DeviceManager.hpp"
#include "ProcessManager.hpp"
#include "LogManager.hpp"
#include "SettingsManager.hpp"
#include "PolicyManager.hpp"

class Facade {
private:
    DBConnection db;

    DeviceManager deviceManager;
    ProcessManager processManager;
    LogManager logManager;
    SettingsManager settingsManager;
    PolicyManager policyManager;

    CommandContext ctx;

public:
    Facade()
        : db("app.db"),
          deviceManager(db),
          processManager(db),
          logManager(),
          settingsManager(),
          policyManager(),
          ctx{
              deviceManager,
              processManager,
              logManager,
              settingsManager,
              policyManager
          }
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }
};