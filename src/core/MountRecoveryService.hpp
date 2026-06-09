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
            try {
                if (auto newrec = manager.mount(devNode))
                    registry.recreate(*newrec);
            }
            catch(const MountError& e) {
                mylog->error("Failed mount for {}", devNode);
            }
            return;
        }

        auto md = resolver.getMountMode(*mountPoint);
        auto info = resolver.resolve(devNode.c_str());

        bool some_changes = false;

        if (!info || !md) return;

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

        if (*mountPoint != rec.mountPoint) {
            rec.mountPoint = *mountPoint;
            some_changes = true;
        }

        if (*md != desired) {
            try {
                manager.remount(rec);
            }
            catch(const MountError& e) {
                mylog->error("Failed remount for {}", rec.devNode);
            }
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

        // удаление отсутствующих устройств
        for (const auto& reg : regs) {
            if (!devSet.contains(reg.devNode)) {
                try {
                    registry.removeByDevNode(reg.devNode);
                    manager.unmount(reg.mountPoint);
                }
                catch(const UnMountError& e) {
                    mylog->error("Failed unmount for {}", reg.mountPoint);
                }
            }
        }

        // актуализизация существующих и создание новых
        for (const auto& node : devNodes) {
            auto it = regMap.find(node);
            if (it != regMap.end()) {
                actualize(it->second);
            }
            else {
                try {
                    if (auto rec = manager.mount(node))
                        registry.add(*rec);
                }
                catch(const MountError& e) {
                    mylog->error("Failed mount for {}", node);
                }
            }
        }
    }
};