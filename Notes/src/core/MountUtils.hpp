#pragma once

#include <unordered_set>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/mount.h>
#include <filesystem>
#include <libudev.h>

class MountPointBuilder {
public:
    static std::string build(const DeviceInfo& dev) {
        std::string base = "/media/dlp";
        std::string vid = dev.vendorId.value_or("unknown");
        std::string pid = dev.productId.value_or("unknown");
        std::string sn  = dev.serial.value_or("noserial");
        return base + "/" + vid + "_" + pid + "_" + sn;
    }

    static void ensureExists(const std::string& path) {
        std::filesystem::create_directories(path);
    }
};

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

    static std::string getFsType(const std::string& devnode)
    {
        struct udev* udev = udev_new();

        struct udev_device* dev =
            udev_device_new_from_subsystem_sysname(
                udev,
                "block",
                devnode.substr(devnode.find_last_of('/') + 1).c_str());

        if (!dev) {
            udev_unref(udev);
            return "";
        }

        const char* fs =
            udev_device_get_property_value(dev, "ID_FS_TYPE");

        std::string result = fs ? fs : "";

        udev_device_unref(dev);
        udev_unref(udev);

        return result;
    }

    static void mountDevice(
        const std::string& devnode,
        const std::string& mountPoint,
        bool readOnly)
    {
        sync();

        std::string fsType = getFsType(devnode);

        if (fsType.empty()) {
            throw std::runtime_error("Unknown filesystem");
        }

        std::string opts;
        int flags = 0;

        if (readOnly) {
            flags = MS_RDONLY;
            opts = "ro,umask=0000";
        } else {
            opts = "rw,umask=0000";
        }

        if (mount(
                devnode.c_str(),
                mountPoint.c_str(),
                fsType.c_str(),
                flags,
                opts.c_str()) < 0)
        {
            throw std::runtime_error(
                std::string("mount failed: ") +
                std::strerror(errno));
        }
    }

    static void handleUnmount(const std::string& mountPoint)
    {
        sync();

        if (umount2(mountPoint.c_str(), MNT_DETACH) < 0) {
            mylog->error(std::string("umount failed: ") +
                        strerror(errno));
        } else {
            mylog->info("Unmounted: " + mountPoint);
        }
    }
};