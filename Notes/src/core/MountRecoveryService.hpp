#pragma once

#include <vector>
#include <string>
#include <optional>

#include "MountRecord.hpp"
#include "MountRegistry.hpp"
#include "MountManager.hpp"
#include "IDeviceResolver.hpp"

class MountRecoveryService {
private:
    MountRegistry &registry;
    IDeviceResolver &resolver;
    MountManager &manager;

public:
    explicit MountRecoveryService(
        MountRegistry& mr, 
        IDeviceResolver &rs,
        MountManager& man
    )
            : registry(mr),
              resolver(rs),
              manager(man)
            {}

    void run()
    {
        for (auto rec : registry.getAll())
        {
            const auto& devNode = rec.devNode;
            auto info = resolver.resolve(devNode.c_str());
            if (info) {
                const auto mountPoint = 
                    resolver.getMountPoint(devNode);
                if (mountPoint) {
                    auto md = 
                        resolver.getMountMode(*mountPoint);
                    if (md == rec.mode) {
                        if (*info == rec.info) {
                            if (mountPoint != 
                                rec.mountPoint) {
                                rec.mountPoint = *mountPoint;
                                registry.refresh(rec);
                            }
                        }
                        else {
                            auto newrec = manager.remount(rec);
                            if (newrec)
                                registry.recreate(*newrec);
                        }
                    }
                    else {
                        auto newrec = manager.remount(rec);
                        if (newrec)
                            registry.recreate(*newrec);
                    }
                }
                else {
                    auto newrec = manager.mount(devNode);
                    if (newrec)
                        registry.recreate(*newrec);
                }
            }
            else {
                registry.removeByDevNode(devNode);
                manager.unmount(rec.mountPoint);
            }
        }
    }
};