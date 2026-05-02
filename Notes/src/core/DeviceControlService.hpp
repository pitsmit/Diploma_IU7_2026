#pragma once

#include "DeviceManager.hpp"
#include "PolicyManager.hpp"
#include "MountEvent.hpp"
#include "DevLogger.hpp"

class DeviceControlService {
private:
    DeviceManager& deviceManager_;
    PolicyManager& policyManager_;

public:
    DeviceControlService(
        DeviceManager& deviceManager,
        PolicyManager& policyManager) :
        deviceManager_(deviceManager),
        policyManager_(policyManager) {}

    void handleEvent(const MountEvent& event)
    {
        mylog->info("Start handle event");
        mylog->info(*event.dev.vendorName);
        mylog->info(*event.dev.productName);

        if (!policyManager_.isAllowed(event.dev)) {
            mylog->info("USB IS NOT ALLOWED");
            return;
        }

        mylog->info("USB IS ALLOWED");
    }
};