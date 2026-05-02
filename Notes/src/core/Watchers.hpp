#pragma once
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

#include "UdevDeviceResolver.hpp"

#include "EventQueue.hpp"
#include "SecurityEvent.hpp"
#include "DevLogger.hpp"

class Watcher {
public:
    virtual void run() = 0;
    virtual ~Watcher() = default;
};

class MountWatcher : public Watcher {
private:
    EventQueue<SecurityEvent>& queue_;

    std::unordered_set<std::string> knownMounts_;

public:
    explicit MountWatcher(
    EventQueue<SecurityEvent>& queue) : queue_(queue) {}

    void run() override
    {
        knownMounts_ = readMounts();

        while (true) {

            auto current = readMounts();

            processChanges(current);

            std::this_thread::sleep_for(
                std::chrono::seconds(1));
        }
    }

private:
    std::unordered_set<std::string> readMounts()
    {
        std::unordered_set<std::string> mounts;

        std::ifstream file("/proc/self/mountinfo");

        std::string line;

        while (std::getline(file, line)) {

            std::istringstream iss(line);

            std::string token;

            int field = 0;

            while (iss >> token) {

                ++field;

                if (field == 5) {
                    mounts.insert(token);
                    break;
                }
            }
        }

        return mounts;
    }

    void processChanges(
        const std::unordered_set<std::string>& current)
    {
        // новые mount
        for (const auto& mount : current) {

            if (!knownMounts_.contains(mount)) {

                queue_.push(
                    createEvent(
                        mount,
                        EventType::INSERT));
            }
        }

        // удалённые mount
        for (const auto& mount : knownMounts_) {

            if (!current.contains(mount)) {

                queue_.push(
                    createEvent(
                        mount,
                        EventType::REMOVE));
            }
        }

        knownMounts_ = current;
    }

    SecurityEvent createEvent(
        const std::string& mountPath,
        EventType type)
    {
        mylog->info("Create new event");
        SecurityEvent event;

        event.type = type;
        event.mountPath = mountPath;

        UdevDeviceResolver resolver;

        if (auto info = resolver.resolve(mountPath)) {

            event.deviceNode = info->devNode;
            event.vendorId = info->vendorId;
            event.productId = info->productId;
            event.serial = info->serial;
        }

        return event;
    }
};

/*class FanotifyWatcher : public Watcher {
public:
    void start(EventQueue& queue) override {
        queue.push({});
    }
};*/