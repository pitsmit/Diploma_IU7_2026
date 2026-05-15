#pragma once

#include <vector>
#include <string>
#include <optional>
#include <unordered_set>

#include "MountRecord.hpp"
#include "MountRegistry.hpp"
#include "MountService.hpp"
#include "IDeviceResolver.hpp"

class MountRecoveryService {
private:
    MountRegistryManager &registry;
    IDeviceResolver &resolver;
    MountService &manager;

public:
    explicit MountRecoveryService(
        MountRegistryManager& mr, 
        IDeviceResolver &rs,
        MountService& man
    )
            : registry(mr),
              resolver(rs),
              manager(man)
            {}

    void actualize(MountRecord &rec) {
        const auto& devNode = rec.devNode;
        auto info = resolver.resolve(devNode.c_str());

        if (!info) return;

        const auto mountPoint = 
            resolver.getMountPoint(devNode);

        if (!mountPoint) {
            auto newrec = manager.mount(devNode);
            if (newrec) registry.recreate(*newrec);
            return;
        }

        auto md = resolver.getMountMode(*mountPoint);
        bool modeChanged = md != rec.mode;
        bool infoChanged = *info != rec.info;

        if (modeChanged || infoChanged) {
            auto newrec = manager.remount(rec);
            if (newrec) registry.recreate(*newrec);
            return;
        }

        if (*mountPoint != rec.mountPoint) {
            rec.mountPoint = *mountPoint;
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