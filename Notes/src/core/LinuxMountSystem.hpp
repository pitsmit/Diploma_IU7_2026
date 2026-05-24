#include "IMountSystem.hpp"

#include <sys/mount.h>
#include <libudev.h>
#include <unistd.h>
#include <blkid/blkid.h>

#include "DevLogger.hpp"

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
            readOnly ? (MS_RDONLY | MS_NOEXEC) : 0,
            opts.empty() ? nullptr : opts.c_str()
        );
    }

    int remount(
        const std::string& target,
        bool readOnly) override
    {
        return ::mount(
            nullptr,
            target.c_str(),
            nullptr,
            readOnly ? (MS_REMOUNT | MS_RDONLY | MS_NOEXEC) : MS_REMOUNT,
            nullptr
        );
    }

    int umount(const std::string& target) override {
        return ::umount2(target.c_str(), MNT_DETACH);
    }

    void sync() override {
        ::sync();
    }

    std::string getFsType(const std::string& devnode) override {
        blkid_cache cache = nullptr;
        if (blkid_get_cache(&cache, nullptr) != 0) {
            return "";
        }
        char* type = blkid_get_tag_value(cache, "TYPE", devnode.c_str());
        std::string result = type ? type : "";
        blkid_put_cache(cache);
        return result;
    }
};