#pragma once

class DeviceManager;
class MountRegistry;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistry& mountRegistry;
};