#pragma once

#include <vector>
#include <string>
#include <optional>
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
        MountRegistryManager& mr, 
        IDeviceResolver &rs,
        MountService& man,
        DeviceManager& devman_
    )
            : registry(mr),
              resolver(rs),
              manager(man),
              devman(devman_)
            {}

    void actualize(MountRecord &rec) {
        const auto& devNode = rec.devNode;
        const auto mountPoint = 
            resolver.getMountPoint(devNode);

        if (!mountPoint) {
            auto newrec = manager.mount(devNode);
            if (newrec) registry.recreate(*newrec);
            return;
        }

        auto md = resolver.getMountMode(*mountPoint);
        auto info = resolver.resolve(devNode.c_str());

        bool some_changes = false;

        if (!info) return;

        if (*info != rec.info) {
            rec.info = *info;
            some_changes = true;
        }

        MODE desired =
            devman.isAllowed(*info)
                ? MODE::RW
                : MODE::RO;

        if (rec.mode != desired) {
            rec.mode = desired;
            some_changes = true;
        }

        if (*md != desired) {
            manager.remount(rec);
        }

        if (*mountPoint != rec.mountPoint) {
            rec.mountPoint = *mountPoint;
            some_changes = true;
        }

        if (some_changes) {
            registry.refresh(rec);
        }
    }

    void run()
    {
        auto regs = registry.getAll();
        auto devNodes = resolver.getUsbDevNodes();

        std::unordered_map<std::string, MountRecord> regMap;
        for (const auto& reg : regs)
            regMap[reg.devNode] = reg;
        std::unordered_set<std::string> devSet(
            devNodes.begin(),
            devNodes.end());

        // удаляем отсутствующие устройства
        for (const auto& reg : regs) {
            if (!devSet.contains(reg.devNode)) {
                registry.removeByDevNode(reg.devNode);
                manager.unmount(reg.mountPoint);
            }
        }

        // актуализируем существующие
        // или создаём новые
        for (const auto& node : devNodes) {
            auto it = regMap.find(node);
            if (it != regMap.end()) {
                actualize(it->second);
            }
            else {
                auto rec = manager.mount(node);
                if (rec)
                    registry.add(*rec);
            }
        }
    }
};