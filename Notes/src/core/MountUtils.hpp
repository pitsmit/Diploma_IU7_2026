#pragma once

#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <libudev.h>

class MountUtils {
public:
    static std::unordered_set<std::string> readMounts()
    {
        std::unordered_set<std::string> mounts;
        struct udev* udev = udev_new();

        if (!udev) {
            return mounts;
        }

        struct udev_enumerate* enumerate =
            udev_enumerate_new(udev);
        udev_enumerate_add_match_subsystem(
            enumerate,
            "block");
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

            if (!dev) {
                continue;
            }

            struct udev_device* usb =
                udev_device_get_parent_with_subsystem_devtype(
                    dev,
                    "usb",
                    "usb_device");

            if (!usb) {
                udev_device_unref(dev);
                continue;
            }

            const char* devnode =
                udev_device_get_devnode(dev);

            if (!devnode) {
                udev_device_unref(dev);
                continue;
            }

            std::ifstream mountsFile("/proc/self/mountinfo");
            std::string line;

            while (std::getline(mountsFile, line)) {
                if (line.find(devnode) == std::string::npos) {
                    continue;
                }

                std::istringstream iss(line);
                std::string token;
                int field = 0;

                while (iss >> token) {
                    ++field;
                    if (field == 5) {
                        mounts.insert(token);
                        break;
                    }
                }
            }

            udev_device_unref(dev);
        }

        udev_enumerate_unref(enumerate);
        udev_unref(udev);
        return mounts;
    }
};