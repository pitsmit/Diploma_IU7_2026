#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountUtils.hpp"
#include "UdevDeviceResolver.hpp"

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(CommandContext& ctx) = 0;
};

class GetWhiteListDeviceCommand : public Command {
public:
    std::vector<Device> list;

    void execute(CommandContext& ctx) override {
        list = ctx.deviceManager.getWhitelist();
    }
};

class AddDeviceToWhiteListCommand : public Command {
private:
    Device device;

public:
    AddDeviceToWhiteListCommand(const Device& d)
        : device(d) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.addToWhitelist(device);
    }
};

class DeleteDeviceFromWhiteListCommand : public Command {
private:
    size_t id;

public:
    DeleteDeviceFromWhiteListCommand(size_t id)
        : id(id) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.removeFromWhitelist(id);
    }
};

class PatchValidToDeviceCommand : public Command {
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

class GetCurrentConnectedDevicesCommand : public Command {
public:
    std::vector<Device> devices;

    void execute(CommandContext& /*ctx*/) override
    {
        devices.clear();

        auto mounts = MountUtils::readMounts();

        UdevDeviceResolver resolver;

        for (const auto& mount : mounts) {
            auto info = resolver.resolve(mount);

            if (!info) {
                continue;
            }

            Device d;
            d.info = *info;
            devices.push_back(d);
        }
    }
};