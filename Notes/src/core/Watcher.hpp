#pragma once

#include <libudev.h>

#include "MountUtils.hpp"
#include "UdevDeviceResolver.hpp"
#include "EventQueue.hpp"
#include "DeviceEvent.hpp"
#include "DevLogger.hpp"

class UdevWatcher {
private:
    EventQueue<DeviceEvent>& queue_;

public:
    explicit UdevWatcher(EventQueue<DeviceEvent>& queue)
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
            mon, "block", "partition");
        udev_monitor_enable_receiving(mon);

        int fd = udev_monitor_get_fd(mon);

        while (true) {
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            if (select(fd + 1, &fds, nullptr, nullptr, nullptr) < 0) {
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

            const char* action = udev_device_get_action(dev);
            if (!action) {
                udev_device_unref(dev);
                continue;
            }

            std::string act(action);

            if (act == "add" || act == "change") {
                if (!isUsbDevice(dev)) {
                    udev_device_unref(dev);
                    continue;
                }

                auto event = buildEvent(dev, EventType::INSERT);
                if (!event.devNode.empty())
                    queue_.push(event);
            }
            else if (act == "remove") {
                auto event = buildEvent(dev, EventType::REMOVE);
                if (!event.devNode.empty())
                    queue_.push(event);
            }

            udev_device_unref(dev);
        }

        udev_monitor_unref(mon);
        udev_unref(udev);
    }

private:
    bool isUsbDevice(struct udev_device* dev)
    {
        struct udev_device* parent =
            udev_device_get_parent_with_subsystem_devtype(
                dev, "usb", "usb_device");

        return parent != nullptr;
    }

    DeviceEvent buildEvent(struct udev_device* dev, EventType type)
    {
        DeviceEvent event;
        event.type = type;
        const char* devnode = udev_device_get_devnode(dev);
        if (!devnode)
            return {};
        event.devNode = devnode;
        UdevDeviceResolver resolver;
        if (auto info = resolver.resolve(devnode))
            event.dev = *info;

        return event;
    }
};