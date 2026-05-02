#pragma once

#include "DeviceManager.hpp"
#include "LogManager.hpp"
#include "ProcessManager.hpp"
#include "PolicyManager.hpp"
#include "SecurityEvent.hpp"
#include "DevLogger.hpp"

class DeviceControlService {
private:
    DeviceManager& deviceManager_;
    ProcessManager& processManager_;
    LogManager& logManager_;
    PolicyManager& policyManager_;

public:
    DeviceControlService(
        DeviceManager& deviceManager,
        ProcessManager& processManager,
        LogManager& logManager,
        PolicyManager& policyManager)
        :
        deviceManager_(deviceManager),
        processManager_(processManager),
        logManager_(logManager),
        policyManager_(policyManager)
    {
    }

    void handleEvent(const SecurityEvent& event)
    {
        mylog->info("Start handle event");
        logManager_.log(event);

        if (!policyManager_.isAllowed(event)) {
            return;
        }

        switch (event.type) {

            case EventType::INSERT:
                deviceManager_.handleMount(event);
                break;

            case EventType::REMOVE:
                deviceManager_.handleUnmount(event);
                break;

            default:
                break;
        }
    }
};