#pragma once

#include <string>
#include <filesystem>
#include <cstdlib>

static std::string createLoopFs(const std::string& fsType)
{
    std::string img = "/tmp/usb.img";

    system("losetup -D");

    std::string mkfsCmd;

    if (fsType == "vfat") {
        mkfsCmd = "mkfs.vfat " + img;
    }
    else if (fsType == "ext4") {
        mkfsCmd = "mkfs.ext4 " + img;
    }
    else if (fsType == "ntfs") {
        mkfsCmd = "mkfs.ntfs " + img;
    }
    else if (fsType == "exfat") {
        mkfsCmd = "mkfs.exfat " + img;
    }
    else if (fsType == "fat32") {
        mkfsCmd = "mkfs.fat32 " + img;
    }
    else {
        throw std::runtime_error("unsupported filesystem: " + fsType);
    }

    std::string cmd1 = "dd if=/dev/zero of=" + img + " bs=1M count=32";

    if (system(cmd1.c_str()) != 0 ||
        system(mkfsCmd.c_str()) != 0 ||
        system(("losetup -fP " + img).c_str())) {
        throw std::runtime_error("failed to create loop fs");
    }

    FILE* pipe = popen("losetup -a | grep usb.img | cut -d: -f1", "r");

    if (!pipe) {
        throw std::runtime_error("popen failed");
    }

    char buffer[128];
    std::string devNode;

    if (fgets(buffer, sizeof(buffer), pipe)) {
        devNode = buffer;
        devNode.erase(devNode.find_last_not_of(" \n\r\t") + 1);
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

static int simulate_write(std::string mount_path)
{
    pid_t pid = fork();

    if (pid == 0) {
        seteuid(1000);
        setegid(1000);

        std::string testFile = mount_path + "/test_write.txt";

        int fd = open(testFile.c_str(), O_CREAT | O_WRONLY, 0644);
        if (fd < 0) {
            _exit(1);
        }

        const char* msg = "hello";
        write(fd, msg, 5);
        close(fd);

        std::string folderPath = mount_path + "/mydir";
        if (mkdir(folderPath.c_str(), 0755) != 0) {
            _exit(1);
        }

        testFile = folderPath + "/test_write.txt";

        fd = open(testFile.c_str(), O_CREAT | O_WRONLY, 0644);
        if (fd < 0) {
            _exit(1);
        }

        _exit(0);
    }

    int status;
    waitpid(pid, &status, 0);

    return status;
}