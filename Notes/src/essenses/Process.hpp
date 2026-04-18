#pragma once
#include <string>

struct Process {
    size_t id;
    std::string name;
    std::string path;
    std::string validTo;
};