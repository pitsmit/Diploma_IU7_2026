#pragma once

#include "Command.hpp"
#include "Device.hpp"
#include "DeviceManager.hpp"
#include <iostream>

class DeviceCommand : public Command {
public:
    void execute([[maybe_unused]] CommandContext& ctx) override {}
};

class GetWhiteListDeviceCommand : public DeviceCommand {
public:
    std::vector<Device> list;

    void execute(CommandContext& ctx) override {
        list = ctx.deviceManager.getWhitelist();

        /*for (const auto& d : list) {
            std::cout << "Device: " << d.name
                    << " VID: " << d.vendorId
                    << " PID: " << d.productId
                    << std::endl;
        }*/
    }
};

class AddDeviceToWhiteListCommand : public DeviceCommand {
private:
    Device device;

public:
    AddDeviceToWhiteListCommand(const Device& d)
        : device(d) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.addToWhitelist(device);
    }
};

class DeleteDeviceFromWhiteListCommand : public DeviceCommand {
private:
    size_t id;

public:
    DeleteDeviceFromWhiteListCommand(size_t id)
        : id(id) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.removeFromWhitelist(id);
    }
};

class PatchValidToDeviceCommand : public DeviceCommand {
private:
    size_t id;
    std::string validTo;

public:
    PatchValidToDeviceCommand(size_t id, const std::string& validTo)
        : id(id), validTo(validTo) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.patchValidTo(id, validTo);
    }
};

class GetCurrentConnectedDevicesCommand : public DeviceCommand {
public:
    void execute([[maybe_unused]] CommandContext& ctx) override {
    }
};