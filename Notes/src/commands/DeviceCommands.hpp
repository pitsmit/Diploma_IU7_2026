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
        /*std::cout << "========== Device ==========" << std::endl;
        std::cout << "ID: " << device.id << std::endl;
        std::cout << "Valid To: " << (device.validTo ? *device.validTo : "N/A") << std::endl;
        std::cout << "---------- Device Info ----------" << std::endl;
        std::cout << "  Vendor ID:    " << (device.info.vendorId ? *device.info.vendorId : "N/A") << std::endl;
        std::cout << "  Product ID:   " << (device.info.productId ? *device.info.productId : "N/A") << std::endl;
        std::cout << "  Serial:       " << (device.info.serial ? *device.info.serial : "N/A") << std::endl;
        std::cout << "  Vendor Name:  " << (device.info.vendorName ? *device.info.vendorName : "N/A") << std::endl;
        std::cout << "  Product Name: " << (device.info.productName ? *device.info.productName : "N/A") << std::endl;
        std::cout << "==============================" << std::endl;*/

        ctx.deviceManager.addToWhitelist(device);
        mylog->info(*device.mountPath);
        //MountUtils::mountReadWrite(*device.mountPath);
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

    void execute(CommandContext& /*ctx*/) override
    {
        devices.clear();

        /*auto mounts = MountUtils::readMounts();

        UdevDeviceResolver resolver;

        for (const auto& mount : mounts) {
            auto info = resolver.resolve(mount);

            if (!info) {
                continue;
            }

            Device d;
            d.info = *info;
            d.mountPath = mount;
            devices.push_back(d);
        }*/
    }
};