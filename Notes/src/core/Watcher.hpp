#pragma once

#include <unordered_set>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

#include "MountUtils.hpp"
#include "UdevDeviceResolver.hpp"

#include "EventQueue.hpp"
#include "MountEvent.hpp"
#include "DevLogger.hpp"

class MountWatcher {
private:
    EventQueue<MountEvent>& queue_;

    std::unordered_set<std::string> knownMounts_;

public:
    explicit MountWatcher(
    EventQueue<MountEvent>& queue) : queue_(queue) {}

    void run()
    {
        knownMounts_ = MountUtils::readMounts();
        while (true) {
            auto current = MountUtils::readMounts();
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
                        mount));
            }
        }

        knownMounts_ = current;
    }

    MountEvent createEvent(const std::string& mountPath)
    {
        mylog->info("Create new event");
        MountEvent event;
        event.mountPath = mountPath;
        UdevDeviceResolver resolver;
        if (auto info = resolver.resolve(mountPath)) {
            event.dev = *info;
        }
        return event;
    }
};