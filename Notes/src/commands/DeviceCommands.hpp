#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountUtils.hpp"
#include "MountRegistry.hpp"

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
    const MountRecord record;
    std::optional<std::string> validTo;

public:
    AddDeviceToWhiteListCommand(const MountRecord& d, std::optional<std::string> vld)
        : record(d), validTo(vld) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.addToWhitelist(record, validTo);

        ctx.mountUtils.handleUnmount(record.mountPoint);
        ctx.mountUtils.mountDevice(
            record.devNode,
            record.mountPoint,
            false
        );
    }
};

class DeleteDeviceFromWhiteListCommand : public Command {
private:
    size_t id;

public:
    DeleteDeviceFromWhiteListCommand(size_t id)
        : id(id) {}

    void execute(CommandContext& ctx) override {
        std::optional<MountRecord> r = ctx.mountRegistry.getById(id);
        if (r) {
            ctx.mountUtils.handleUnmount(r->mountPoint);
            ctx.mountRegistry.removeByDevNode(r->devNode);
        }
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
    std::vector<MountRecord> records;

    void execute(CommandContext& ctx) override
    {
        auto records = ctx.mountRegistry.getAll();
    }
};