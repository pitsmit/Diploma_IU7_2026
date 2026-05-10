#pragma once

#include <string>

#include "DevLogger.hpp"
#include "IMountSystem.hpp"
#include "MountRecord.hpp"
#include "Exceptions.hpp"

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
            throw UnknownFsError(("Unknown filesystem for devnode: " + devnode).c_str());
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
            std::string msg = 
                std::string("mount failed for devnode: ") 
                + devnode 
                + std::string(" and mountPoint: ") 
                + mountPoint 
                + strerror(errno);
            throw MountError(msg.c_str());
        }
    }

    void handleUnmount(const std::string& mountPoint)
    {
        sys.sync();

        if (sys.umount(mountPoint) < 0) {
            std::string msg = 
                std::string("mount failed for mountPoint: ") 
                + mountPoint 
                + strerror(errno);
            throw UnMountError(msg.c_str());
        }

        mylog->info("Unmounted: {}", mountPoint);
    }

    void remount(const MountRecord &record)
    {
        handleUnmount(record.mountPoint);
        mountDevice(
            record.devNode,
            record.mountPoint,
            record.mode == MODE::RO ? true : false
        );
    }
};