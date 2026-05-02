#pragma once
#include <string>
#include "SecurityEvent.hpp"

class PolicyManager {
public:
    bool isAllowed([[maybe_unused]] const SecurityEvent r) { return true; }
};