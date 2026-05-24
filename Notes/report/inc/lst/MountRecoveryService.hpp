#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include "MountRecord.hpp"
#include "MountRegistry.hpp"
#include "MountService.hpp"
#include "IDeviceResolver.hpp"
#include "DeviceManager.hpp"
class MountRecoveryService {
private:
    MountRegistryManager &registry;
    IDeviceResolver &resolver;
    MountService &manager;
    DeviceManager &devman;
public:
    explicit MountRecoveryService(
        MountRegistryManager& mr, IDeviceResolver &rs,
        MountService& man, DeviceManager& devman_)
            : registry(mr), resolver(rs), manager(man),
              devman(devman_) {}