        std::unordered_set<std::string> devSet(
            devNodes.begin(),
            devNodes.end());
        for (const auto& reg : regs) {
            if (!devSet.contains(reg.devNode)) {
                registry.removeByDevNode(reg.devNode);
                manager.unmount(reg.mountPoint);
            }
        }
        for (const auto& node : devNodes) {
            auto it = regMap.find(node);
            if (it != regMap.end())
                actualize(it->second);
            else {
                auto rec = manager.mount(node);
                if (rec) registry.add(*rec);
            }
        }
    }
};