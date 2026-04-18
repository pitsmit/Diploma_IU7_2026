#pragma once

class DeviceManager;
class ProcessManager;
class LogManager;
class SettingsManager;
class PolicyManager;

struct CommandContext {
    DeviceManager& deviceManager;
    ProcessManager& processManager;
    LogManager& logManager;
    SettingsManager& settingsManager;
    PolicyManager& policyManager;
};