#pragma once

class DeviceManager;
class MountService;
class MountRegistry;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistry& mountRegistry;
    MountService& mountService;
};