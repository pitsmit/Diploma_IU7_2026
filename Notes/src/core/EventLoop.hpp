#pragma once

#include "EventQueue.hpp"
#include "DeviceEvent.hpp"
#include "DeviceControlService.hpp"

class EventLoop {
private:
    EventQueue<DeviceEvent>& queue_;
    DeviceControlService& service_;

public:
    EventLoop(
        EventQueue<DeviceEvent>& queue,
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