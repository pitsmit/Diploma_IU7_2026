#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountUtils.hpp"
#include "MountRegistry.hpp"
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

        std::string mountPoint;
        if (ctx.mountRegistry.get(*device.devNode, mountPoint)) {
            ctx.mountUtils.handleUnmount(mountPoint);
            ctx.mountUtils.mountDevice(
                *device.devNode,
                mountPoint,
                false
            );
        }
        else {
            mylog->error("Failed to remount device: devNode={}.", *device.devNode);
        }
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
    std::optional<std::string> validTo;

public:
    PatchValidToDeviceCommand(
        size_t id,
        std::optional<std::string> validTo)
        : id(id), validTo(validTo) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.patchValidTo(id, validTo);
    }
};

class GetCurrentConnectedDevicesCommand : public Command {
public:
    std::vector<Device> devices;

    void execute(CommandContext& ctx) override
    {
        auto devNodes = ctx.mountRegistry.getAllDevNodes();

        UdevDeviceResolver resolver;

        for (const auto& devNode : devNodes) {
            auto info = resolver.resolve(devNode.c_str());

            if (!info) {
                continue;
            }

            devices.push_back(
                DeviceBuilder()
                    .withInfo(*info)
                    .withDevNode(devNode)
                    .build()
            );
        }
    }
};