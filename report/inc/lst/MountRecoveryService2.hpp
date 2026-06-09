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
        if (*md != desired)
            manager.remount(rec);
        if (*mountPoint != rec.mountPoint) {
            rec.mountPoint = *mountPoint;
            some_changes = true;
        }
        if (some_changes)
            registry.refresh(rec);
    }
    void run(){
        auto regs = registry.getAll();
        auto devNodes = resolver.getUsbDevNodes();
        std::unordered_map<std::string, MountRecord> regMap;
        for (const auto& reg : regs)
            regMap[reg.devNode] = reg;