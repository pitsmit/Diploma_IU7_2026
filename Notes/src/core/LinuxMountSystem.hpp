#include "IMountSystem.hpp"

#include <sys/mount.h>
#include <libudev.h>
#include <unistd.h>

class LinuxMountSystem : public IMountSystem {
public:
    int mount(
        const std::string& dev,
        const std::string& target,
        const std::string& fs,
        bool readOnly,
        const std::string& opts) override
    {
        return ::mount(
            dev.c_str(),
            target.c_str(),
            fs.c_str(),
            readOnly ? MS_RDONLY : 0,
            opts.empty() ? nullptr : opts.c_str()
        );
    }

    int umount(const std::string& target) override {
        return ::umount2(target.c_str(), MNT_DETACH);
    }

    void sync() override {
        ::sync();
    }

    std::string getFsType(const std::string& devnode) override {
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
};