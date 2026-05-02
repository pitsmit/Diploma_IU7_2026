#pragma once

#include "EventQueue.hpp"
#include "MountEvent.hpp"
#include "DeviceControlService.hpp"

class EventLoop {
private:
    EventQueue<MountEvent>& queue_;
    DeviceControlService& service_;

public:
    EventLoop(
        EventQueue<MountEvent>& queue,
        DeviceControlService& service) :
        queue_(queue),
        service_(service) {}

    void run()
    {
        while (auto event = queue_.pop()) {
            service_.handleEvent(*event);
        }
    }
};