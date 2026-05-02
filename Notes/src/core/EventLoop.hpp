#pragma once

#include "EventQueue.hpp"
#include "SecurityEvent.hpp"
#include "DeviceControlService.hpp"

class EventLoop {
private:
    EventQueue<SecurityEvent>& queue_;

    DeviceControlService& service_;

public:
    EventLoop(
        EventQueue<SecurityEvent>& queue,
        DeviceControlService& service)
        :
        queue_(queue),
        service_(service) {}

    void run()
    {
        while (auto event = queue_.pop()) {
            service_.handleEvent(*event);
        }
    }
};