#pragma once

#include <string>
#include <filesystem>
#include <cstdlib>

static std::string createLoopFs() {
    std::string img = "/tmp/usb.img";

    system("losetup -D");

    std::string cmd1 = "dd if=/dev/zero of=" + img + " bs=1M count=32";
    std::string cmd2 = "mkfs.vfat " + img;
    std::string cmd3 = "losetup -fP " + img;

    if (system(cmd1.c_str()) != 0 ||
        system(cmd2.c_str()) != 0 ||
        system(cmd3.c_str()) != 0) {
        throw std::runtime_error("failed to create loop fs");
    }

    FILE* pipe = popen("losetup -a | grep usb.img | cut -d: -f1", "r");
    char buffer[128];
    std::string devNode;

    if (fgets(buffer, sizeof(buffer), pipe)) {
        devNode = buffer;
        devNode.erase(devNode.find_last_not_of(" \n\r\t")+1);
    }

    pclose(pipe);

    if (devNode.empty()) {
        throw std::runtime_error("loop device not found");
    }

    return devNode;
}

static bool isMounted(const std::string& path) {
    std::ifstream mounts("/proc/mounts");
    std::string line;

    while (std::getline(mounts, line)) {
        std::istringstream iss(line);
        std::string device, target;
        iss >> device >> target;

        if (target == path)
            return true;
    }
    return false;
}

static bool isMountedAs(const std::string& mountPoint, const std::string& mode) {
    std::ifstream mounts("/proc/mounts");
    std::string line;

    while (std::getline(mounts, line)) {
        std::istringstream iss(line);

        std::string device, target, fs, opts;
        iss >> device >> target >> fs >> opts;

        if (target == mountPoint) {
            return opts.find(mode) != std::string::npos;
        }
    }
    return false;
}