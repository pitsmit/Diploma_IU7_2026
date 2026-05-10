#pragma once

#include "Device.hpp"
#include "DeviceManager.hpp"
#include "CommandContext.hpp"
#include "MountRegistry.hpp"
#include "MountManager.hpp"

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
    MountRecord record;
    std::optional<std::string> validTo;

public:
    AddDeviceToWhiteListCommand(MountRecord& d, std::optional<std::string> vld)
        : record(d), validTo(vld) {}

    void execute(CommandContext& ctx) override {
        ctx.deviceManager.addToWhitelist(record, validTo);
        record.mode = MODE::RW;
        ctx.mountManager.remount(record);
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
            r->mode = MODE::RO;
            const std::optional<MountRecord> newrec = ctx.mountManager.remount(*r);
            ctx.mountRegistry.refresh(*newrec);
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

    void execute(CommandContext& ctx) override {
        auto records = ctx.mountRegistry.getAll();
    }
};