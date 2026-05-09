#pragma once

class DeviceManager;
class MountManager;
class MountRegistry;

struct CommandContext {
    DeviceManager& deviceManager;
    MountRegistry& mountRegistry;
    MountManager& mountManager;
};