#pragma once

#include <optional>
#include <libudev.h>
#include <libmount/libmount.h>
#include <sys/stat.h>
#include <string.h>

#include "MountRecord.hpp"
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

    bool isUsbDevice(struct udev_device* dev)
    {
        struct udev_device* parent =
            udev_device_get_parent_with_subsystem_devtype(
                dev, "usb", "usb_device");

        return parent != nullptr;
    }

    std::vector<std::string> getUsbDevNodes()
    {
        std::vector<std::string> result;
        struct udev* udev = udev_new();
        if (!udev)
            return result;
        struct udev_enumerate* enumerate =
            udev_enumerate_new(udev);
        if (!enumerate) {
            udev_unref(udev);
            return result;
        }
        // block devices
        udev_enumerate_add_match_subsystem(
            enumerate,
            "block");
        // только разделы
        udev_enumerate_add_match_property(
            enumerate,
            "DEVTYPE",
            "partition");
        udev_enumerate_scan_devices(enumerate);
        struct udev_list_entry* devices =
            udev_enumerate_get_list_entry(enumerate);
        struct udev_list_entry* entry;

        udev_list_entry_foreach(entry, devices) {
            const char* path =
                udev_list_entry_get_name(entry);
            struct udev_device* dev =
                udev_device_new_from_syspath(
                    udev,
                    path);
            if (!dev)
                continue;
            if (!isUsbDevice(dev)) {
                udev_device_unref(dev);
                continue;
            }
            const char* devNode =
                udev_device_get_devnode(dev);
            if (devNode)
                result.emplace_back(devNode);
            udev_device_unref(dev);
        }
        udev_enumerate_unref(enumerate);
        udev_unref(udev);
        return result;
    }


    std::optional<std::string> getMountPoint(const std::string& devNode)
    {
        struct libmnt_table* tb = mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            return std::nullopt;
        struct libmnt_iter* itr = mnt_new_iter(MNT_ITER_FORWARD);
        struct libmnt_fs* fs = nullptr;
        if (!itr) {
            mnt_free_table(tb);
            return std::nullopt;
        }
        while (mnt_table_next_fs(tb, itr, &fs) == 0) {
            const char* src = mnt_fs_get_source(fs);
            const char* target = mnt_fs_get_target(fs);
            if (!src || !target)
                continue;
            if (devNode == src) {
                std::string mountpoint(target);
                mnt_free_iter(itr);
                mnt_free_table(tb);
                return mountpoint;
            }
        }
        mnt_free_iter(itr);
        mnt_free_table(tb);
        return std::nullopt;
    }


    std::optional<MODE>
    getMountMode(const std::string& mountpoint)
    {
        libmnt_table* tb =
            mnt_new_table_from_file("/proc/self/mountinfo");
        if (!tb)
            return std::nullopt;
        libmnt_fs* fs =
            mnt_table_find_target(
                tb,
                mountpoint.c_str(),
                MNT_ITER_FORWARD);
        if (!fs) {
            mnt_free_table(tb);
            return std::nullopt;
        }
        const char* opts =
            mnt_fs_get_options(fs);
        if (!opts) {
            mnt_free_table(tb);
            return std::nullopt;
        }
        char* value = nullptr;
        size_t size = 0;
        std::optional<MODE> result;
        if (mnt_optstr_get_option(
                opts,
                "rw",
                &value,
                &size) == 0)
        {
            result = MODE::RW;
        }
        else if (mnt_optstr_get_option(
                    opts,
                    "ro",
                    &value,
                    &size) == 0)
        {
            result = MODE::RO;
        }
        mnt_free_table(tb);
        return result;
    }
};