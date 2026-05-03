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

class UdevWatcher {
private:
    EventQueue<MountEvent>& queue_;

public:
    explicit UdevWatcher(EventQueue<MountEvent>& queue)
        : queue_(queue) {}

    void run()
    {
        struct udev* udev = udev_new();
        if (!udev) {
            mylog->error("Failed to init udev");
            return;
        }

        struct udev_monitor* mon =
            udev_monitor_new_from_netlink(udev, "udev");

        if (!mon) {
            mylog->error("Failed to create udev monitor");
            udev_unref(udev);
            return;
        }

        udev_monitor_filter_add_match_subsystem_devtype(
            mon,
            "block",
            "partition"
        );

        udev_monitor_enable_receiving(mon);

        int fd = udev_monitor_get_fd(mon);

        while (true) {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            int ret = select(fd + 1, &fds, nullptr, nullptr, nullptr);

            if (ret < 0) {
                if (errno == EINTR)
                    continue;

                mylog->error("select() failed");
                break;
            }

            if (!FD_ISSET(fd, &fds))
                continue;

            struct udev_device* dev =
                udev_monitor_receive_device(mon);

            if (!dev)
                continue;

            const char* action =
                udev_device_get_action(dev);

            if (!action) {
                udev_device_unref(dev);
                continue;
            }

            std::string act(action);

            // =========================
            // INSERT / CHANGE
            // =========================
            if (act == "add" || act == "change") {

                struct udev_device* parent =
                    udev_device_get_parent_with_subsystem_devtype(
                        dev,
                        "usb",
                        "usb_device");

                if (!parent) {
                    udev_device_unref(dev);
                    continue;
                }

                MountEvent event = createEvent(dev);

                if (!event.devNode.empty()) {
                    event.type = EventType::INSERT;
                    queue_.push(event);
                }
            }

            // =========================
            // REMOVE
            // =========================
            else if (act == "remove") {

                const char* devnode =
                    udev_device_get_devnode(dev);

                if (devnode) {
                    MountEvent event;
                    event.type = EventType::REMOVE;
                    event.devNode = devnode;

                    queue_.push(event);
                }
            }

            udev_device_unref(dev);
        }

        udev_monitor_unref(mon);
        udev_unref(udev);
    }

private:
    MountEvent createEvent(struct udev_device* dev)
    {
        MountEvent event;

        const char* devnode =
            udev_device_get_devnode(dev);

        if (!devnode)
            return event;

        event.devNode = devnode;

        mylog->info("Create new event: " + event.devNode);

        UdevDeviceResolver resolver;

        if (auto info = resolver.resolve(devnode)) {
            event.dev = *info;
        }

        return event;
    }
};