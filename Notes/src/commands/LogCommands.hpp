#pragma once

#include "Command.hpp"
#include "CommandContext.hpp"
#include "LogEventOption.hpp"
#include "DevLogger.hpp"
#include <vector>

class SetLogEventsCommand : public Command {
private:
    std::vector<LogEventOption> options;

public:
    SetLogEventsCommand(const std::vector<LogEventOption>& opts)
        : options(opts) {}

    void execute(CommandContext& ctx) override {
    }
};

class GetLogsCommand : public Command {
public:
    void execute(CommandContext& ctx) override {
        logger->info("Device connected");
    }
};