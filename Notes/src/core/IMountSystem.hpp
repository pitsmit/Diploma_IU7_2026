#pragma once

#include <string>

class IMountSystem {
public:
    virtual int mount(
        const std::string& dev,
        const std::string& target,
        const std::string& fs,
        bool readOnly,
        const std::string& opts) = 0;
    virtual int remount(
        const std::string& dev,
        const std::string& target,
        const std::string& fs,
        bool readOnly) = 0;
    virtual int umount(const std::string& target) = 0;
    virtual void sync() = 0;
    virtual std::string getFsType(const std::string& dev) = 0;
    virtual ~IMountSystem() = default;
};