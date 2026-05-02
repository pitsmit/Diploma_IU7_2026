#pragma once

#include "CommandContext.hpp"

class Command {
public:
    virtual ~Command() = default;

    virtual void execute(CommandContext& ctx) = 0;
};