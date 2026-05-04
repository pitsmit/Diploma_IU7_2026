#pragma once

#include <string>
#include <sys/mount.h>
#include <libudev.h>

class MountUtils {
private:
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

public:
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
            mylog->info("Unmounted: {}", mountPoint);
        }
    }
};