#pragma once

#include <string>
#include <unordered_set>

#include "DevLogger.hpp"
#include "IMountSystem.hpp"
#include "Exceptions.hpp"

enum class FsPermModel {
    NativePosix,
    Emulated
};

static FsPermModel classifyPermModel(const std::string& fs)
{
    static const std::unordered_set<std::string> nativePosix = {
        "ext2",
        "ext3",
        "ext4",
        "xfs",
        "btrfs"
    };

    return nativePosix.contains(fs)
        ? FsPermModel::NativePosix
        : FsPermModel::Emulated;
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
        bool readOnly) {
        sys.sync();

        const std::string fs = sys.getFsType(devnode);
        if (fs.empty()) {
            throw UnknownFsError(("Unknown filesystem for devnode: " + devnode).c_str());
        }

        const FsPermModel permModel = classifyPermModel(fs);
        const int uid = 1000;
        const int gid = 1000;
        std::string opts = "";

        if (permModel == FsPermModel::Emulated) {
            opts =
                std::string(readOnly ? "ro" : "rw") +
                ",uid=" + std::to_string(uid) +
                ",gid=" + std::to_string(gid) +
                ",umask=0000";

            if (fs == "ntfs" || fs == "ntfs3"
            ) {
                opts += ",windows_names";
            }
        }

        const int res = sys.mount(
            devnode,
            mountPoint,
            fs,
            readOnly,
            opts
        );

        if (res < 0) {
            const std::string msg =
                "mount failed for devnode: " + devnode +
                " mountPoint: " + mountPoint +
                " error: " + strerror(errno) +
                " opts: " + opts;
            throw MountError(msg.c_str());
        }

        if (!readOnly && permModel == FsPermModel::NativePosix) {
            if (chown(mountPoint.c_str(), uid, gid) < 0) {
                mylog->warn("chown failed for {}: {}",
                    mountPoint,
                    strerror(errno)
                );
            }
            if (chmod(mountPoint.c_str(), 0775) < 0) {
                mylog->warn("chmod failed for {}: {}",
                    mountPoint,
                    strerror(errno)
                );
            }
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
        const std::string& mountPoint,
        bool readOnly)
    {
        int res = sys.remount(
            mountPoint,
            readOnly
        );

        if (res < 0) {
            std::string msg = 
                std::string("remount failed for mountPoint: ") 
                + mountPoint 
                + strerror(errno);
            throw MountError(msg.c_str());
        }

        mylog->info("Remounted: {}", mountPoint);
    }
};