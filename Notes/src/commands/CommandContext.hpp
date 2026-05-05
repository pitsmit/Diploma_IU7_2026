#pragma once

class DeviceManager;
class MountRegistry;
class MountUtils;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistry& mountRegistry;
    MountUtils& mountUtils;
};