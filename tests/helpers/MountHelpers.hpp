#pragma once

#include <string>
#include <filesystem>
#include <cstdlib>
#include <unistd.h>

struct LoopFs
{
    std::string image;
    std::string device;

    LoopFs() = default;

    LoopFs(const LoopFs&) = delete;
    LoopFs& operator=(const LoopFs&) = delete;

    LoopFs(LoopFs&& other) noexcept
        : image(std::move(other.image)),
          device(std::move(other.device))
    {
        other.image.clear();
        other.device.clear();
    }

    ~LoopFs()
    {
        cleanup();
    }

    void cleanup()
    {
        if (!device.empty())
        {
            std::string cmd = "losetup -d " + device + " >/dev/null 2>&1";
            std::system(cmd.c_str());
            device.clear();
        }

        if (!image.empty())
        {
            std::error_code ec;
            std::filesystem::remove(image, ec);
            image.clear();
        }
    }
};

inline LoopFs createLoopFs(const std::string& fsType)
{
    LoopFs result;

    result.image =
        "/tmp/usb_" +
        std::to_string(::getpid()) +
        "_" +
        std::to_string(std::rand()) +
        ".img";

    std::string mkfsCmd;

    if (fsType == "vfat")
    {
        mkfsCmd = "mkfs.vfat " + result.image;
    }
    else if (fsType == "ext4")
    {
        mkfsCmd = "mkfs.ext4 -F " + result.image;
    }
    else if (fsType == "ntfs")
    {
        mkfsCmd = "mkfs.ntfs -F " + result.image;
    }
    else if (fsType == "exfat")
    {
        mkfsCmd = "mkfs.exfat " + result.image;
    }
    else
    {
        throw std::runtime_error(
            "unsupported filesystem: " + fsType);
    }

    std::string createImage =
        "dd if=/dev/zero of=" + result.image +
        " bs=1M count=32 status=none";

    if (std::system(createImage.c_str()) != 0)
    {
        throw std::runtime_error("failed to create image");
    }

    if (std::system(mkfsCmd.c_str()) != 0)
    {
        throw std::runtime_error("failed to create filesystem");
    }

    FILE* pipe = popen(
        ("losetup --find --show -P " + result.image).c_str(),
        "r");

    if (!pipe)
    {
        throw std::runtime_error("losetup failed");
    }

    char buffer[256]{};

    if (!fgets(buffer, sizeof(buffer), pipe))
    {
        pclose(pipe);
        throw std::runtime_error("cannot obtain loop device");
    }

    pclose(pipe);

    result.device = buffer;

    while (!result.device.empty() &&
           (result.device.back() == '\n' ||
            result.device.back() == '\r' ||
            result.device.back() == ' '))
    {
        result.device.pop_back();
    }

    if (result.device.empty())
    {
        throw std::runtime_error("empty loop device");
    }

    return result;
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