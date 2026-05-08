#pragma once

#include <optional>
#include <libudev.h>
#include <sys/stat.h>

#include "DeviceInfo.hpp"
#include "IDeviceResolver.hpp"

class UdevDeviceResolver : public IDeviceResolver {
public:
    std::optional<DeviceInfo>
    resolve(const char *devNode)
    {
        struct stat st{};
        if (stat(devNode, &st) < 0) {
            return std::nullopt;
        }

        struct udev* udev = udev_new();
        if (!udev) {
            return std::nullopt;
        }

        struct udev_device* dev =
            udev_device_new_from_devnum(
                udev,
                'b',
                st.st_rdev);
        if (!dev) {
            udev_unref(udev);
            return std::nullopt;
        }

        struct udev_device* usb =
            udev_device_get_parent_with_subsystem_devtype(
                dev,
                "usb",
                "usb_device");

        if (!usb) {
            struct udev_device* disk =
                udev_device_get_parent_with_subsystem_devtype(
                    dev,
                    "block",
                    "disk");

            if (disk) {
                usb =
                    udev_device_get_parent_with_subsystem_devtype(
                        disk,
                        "usb",
                        "usb_device");
            }
        }

        DeviceInfoBuilder builder;
        if (usb) {
            if (const char* vid = udev_device_get_sysattr_value(usb, "idVendor"))
                builder.withVendorId(vid);
            if (const char* pid = udev_device_get_sysattr_value(usb, "idProduct"))
                builder.withProductId(pid);
            if (const char* serial = udev_device_get_sysattr_value(usb, "serial"))
                builder.withSerial(serial);
            if (const char* vendorName = udev_device_get_sysattr_value(usb, "manufacturer"))
                builder.withVendorName(vendorName);
            if (const char* productName = udev_device_get_sysattr_value(usb, "product"))
                builder.withProductName(productName);
        }
        
        udev_device_unref(dev);
        udev_unref(udev);
        return builder.build();
    }
};