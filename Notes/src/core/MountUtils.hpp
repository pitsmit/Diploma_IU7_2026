#pragma once

#include <string>

#include "DevLogger.hpp"
#include "IMountSystem.hpp"
#include "Exceptions.hpp"

enum class FsType {
    VFAT,
    EXFAT,
    NTFS,
    EXT4,
    UNKNOWN
};

static FsType classifyFs(const std::string& fs)
{
    if (fs == "vfat") return FsType::VFAT;
    if (fs == "exfat") return FsType::EXFAT;
    if (fs == "ntfs") return FsType::NTFS;
    if (fs == "ext4") return FsType::EXT4;
    return FsType::UNKNOWN;
}

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

        std::string fs = sys.getFsType(devnode);

        if (fs.empty()) {
            throw UnknownFsError(("Unknown filesystem for devnode: " + devnode).c_str());
        }

        FsType type = classifyFs(fs);

        std::string opts;

        switch (type) {

            case FsType::VFAT:
            case FsType::EXFAT:
                opts = readOnly
                    ? "ro,uid=1000,gid=1000,umask=0000"
                    : "rw,uid=1000,gid=1000,umask=0000";
                break;

            case FsType::NTFS:
                opts = readOnly
                    ? "ro,uid=1000,gid=1000,windows_names"
                    : "rw,uid=1000,gid=1000,windows_names";
                break;

            case FsType::EXT4:
                opts = readOnly ? "ro" : "rw";
                break;

            default:
                opts = readOnly ? "ro" : "rw";
                break;
        }

        int res = sys.mount(
            devnode,
            mountPoint,
            fs,
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

        mylog->info("Mounted: {}", mountPoint);
    }

    void handleUnmount(const std::string& mountPoint)
    {
        sys.sync();

        if (sys.umount(mountPoint) < 0) {
            std::string msg = 
                std::string("unmount failed for mountPoint: ") 
                + mountPoint 
                + strerror(errno);
            throw UnMountError(msg.c_str());
        }

        mylog->info("Unmounted: {}", mountPoint);
    }

    void remountDevice(
        const std::string& devnode,
        const std::string& mountPoint,
        bool readOnly)
    {
        sys.sync();

        std::string fsType = sys.getFsType(devnode);

        if (fsType.empty()) {
            throw UnknownFsError(("Unknown filesystem for devnode: " + devnode).c_str());
        }

        int res = sys.remount(
            devnode,
            mountPoint,
            fsType,
            readOnly
        );

        if (res < 0) {
            std::string msg = 
                std::string("remount failed for devnode: ") 
                + devnode 
                + std::string(" and mountPoint: ") 
                + mountPoint 
                + strerror(errno);
            throw MountError(msg.c_str());
        }

        mylog->info("Remounted: {}", mountPoint);
    }
};