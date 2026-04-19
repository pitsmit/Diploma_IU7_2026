#pragma once

class DeviceManager;
class ProcessManager;
class LogManager;
class PolicyManager;

struct CommandContext {
    DeviceManager& deviceManager;
    ProcessManager& processManager;
    LogManager& logManager;
    PolicyManager& policyManager;
};