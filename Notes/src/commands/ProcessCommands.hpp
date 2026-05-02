#pragma once

#include "Command.hpp"
#include "CommandContext.hpp"
#include "Process.hpp"
#include <cstddef>
#include <string>
#include <iostream>


class ProcessCommand : public Command {
public:
    void execute([[maybe_unused]] CommandContext& ctx) override {}
};

class GetWhiteListProcessCommand : public ProcessCommand {
public:
    void execute(CommandContext& ctx) override {
        auto list = ctx.processManager.getWhitelist();

        for (const auto& p : list) {
            std::cout << "Process: " << p.name
                    << " path: " << p.path << std::endl;
        }
    }
};

class AddProcessToWhiteListCommand : public ProcessCommand {
private:
    Process process;

public:
    AddProcessToWhiteListCommand(const Process& p)
        : process(p) {}

    void execute(CommandContext& ctx) override {
        ctx.processManager.addToWhitelist(process);
    }
};

class DeleteProcessFromWhiteListCommand : public ProcessCommand {
private:
    size_t id;

public:
    DeleteProcessFromWhiteListCommand(size_t id)
        : id(id) {}

    void execute(CommandContext& ctx) override {
        ctx.processManager.removeFromWhitelist(id);
    }
};

class PatchValidToProcessCommand : public ProcessCommand {
private:
    size_t id;
    std::string validTo;

public:
    PatchValidToProcessCommand(size_t id, const std::string& validTo)
        : id(id), validTo(validTo) {}

    void execute(CommandContext& ctx) override {
        ctx.processManager.patchValidTo(id, validTo);
    }
};