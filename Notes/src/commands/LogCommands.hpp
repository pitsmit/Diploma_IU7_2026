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
        ctx.logManager.setLogEvents(options);
    }
};

class GetLogEventsCommand : public Command {
public:
    void execute(CommandContext& ctx) override {
        auto events = ctx.logManager.getLogEvents();

        for (const auto& e : events) {
            std::cout << "Event: " << toString(e.type)
                    << " enabled: " << (e.enabled ? "true" : "false")
                    << std::endl;
        }
    }
};

class GetLogsCommand : public Command {
public:
    void execute(CommandContext& ctx) override {
    }
};