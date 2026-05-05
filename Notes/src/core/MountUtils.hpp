#pragma once

#include <string>

#include "DevLogger.hpp"
#include "IMountSystem.hpp"

class MountUtils {
private:
    IMountSystem &sys;
public:
    explicit MountUtils(IMountSystem& s) : sys(s) {}

public:
    void mountDevice(
        const std::string& devnode,
        const std::string& mountPoint,
        bool readOnly)
    {
        sys.sync();

        std::string fsType = sys.getFsType(devnode);

        if (fsType.empty()) {
            throw std::runtime_error("Unknown filesystem");
        }

        std::string opts;

        if (readOnly) {
            opts = "ro,umask=0000";
        } else {
            opts = "rw,umask=0000";
        }

        int res = sys.mount(
            devnode,
            mountPoint,
            fsType,
            readOnly,
            opts
        );

        if (res < 0) {
            mylog->error(
                std::string("mount failed: ") + strerror(errno)
            );
        }
    }

    void handleUnmount(const std::string& mountPoint)
    {
        sys.sync();

        if (sys.umount(mountPoint) < 0) {
            mylog->error(std::string("umount failed: ") +
                        strerror(errno));
        } else {
            mylog->info("Unmounted: {}", mountPoint);
        }
    }
};