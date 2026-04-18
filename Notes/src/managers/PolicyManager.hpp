#include <string>

class PolicyManager {
public:
    bool isDeviceAllowed(const std::string&) { return true; }
    bool isProcessAllowed(const std::string&) { return true; }
};