#pragma once
#include <unordered_map>
#include <string>
#include <mutex>

class MountRegistry {
private:
    std::unordered_map<std::string, std::string> map_;
    std::mutex mtx_;

public:
    void add(
        const std::string& devNode, 
        const std::string& mountPoint)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        map_[devNode] = mountPoint;
    }

    bool get(const std::string& devNode, std::string& out)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = map_.find(devNode);
        if (it == map_.end())
            return false;
        out = it->second;
        return true;
    }

    void remove(const std::string& devNode)
    {
        std::lock_guard<std::mutex> lock(mtx_);
        map_.erase(devNode);
    }
};