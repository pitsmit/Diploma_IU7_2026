#pragma once

#include "Command.hpp"
#include "CommandContext.hpp"

#include "DeviceManager.hpp"
#include "ProcessManager.hpp"
#include "LogManager.hpp"
#include "PolicyManager.hpp"

class Facade {
private:
    DBConnection db;

    DeviceManager deviceManager;
    ProcessManager processManager;
    LogManager logManager;
    PolicyManager policyManager;

    CommandContext ctx;

public:
    Facade()
        : db("app.db"),
          deviceManager(db),
          processManager(db),
          logManager(db),
          policyManager(),
          ctx{
              deviceManager,
              processManager,
              logManager,
              policyManager
          }
    {}

    void execute(Command& command) {
        command.execute(ctx);
    }

    DeviceManager& devices() {
        return deviceManager;
    }

    ProcessManager& processes() {
        return processManager;
    }

    LogManager& logs() {
        return logManager;
    }

    PolicyManager& policies() {
        return policyManager;
    }
};