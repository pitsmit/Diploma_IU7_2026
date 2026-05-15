#pragma once

class DeviceManager;
class MountService;
class MountRegistryManager;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistryManager& mountRegistry;
    MountService& mountService;
};