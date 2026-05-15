#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Exceptions.hpp"

class Config {
private:
    static const std::unordered_map<std::string, std::string>& getCache()
    {
        static std::unordered_map<std::string, std::string> cache;

        static bool initialized = [] {
            std::ifstream file("config.txt");
            if (!file.is_open()) {
                throw FileException("config.txt not found");
            }

            std::string line;
            while (std::getline(file, line)) {
                if (line.empty() || line[0] == '#') continue;

                auto pos = line.find('=');
                if (pos == std::string::npos) continue;

                cache[line.substr(0, pos)] = line.substr(pos + 1);
            }

            return true;
        }();

        (void)initialized;
        return cache;
    }

    static std::string get(const std::string& key)
    {
        const auto& cache = getCache();

        auto it = cache.find(key);
        if (it == cache.end()) {
            throw FileException(("Missing config key: " + key).c_str());
        }

        return it->second;
    }

public:
    static std::vector<std::string> getSchemaPaths()
    {
        std::string v = get("db.schema");

        std::stringstream ss(v);
        std::string item;
        std::vector<std::string> result;

        while (std::getline(ss, item, ';')) {
            if (!item.empty())
                result.push_back(item);
        }

        return result;
    }

    static std::string getDBPath()  { return get("db.path"); }
    static std::string getLogFile() { return get("log.file"); }
    static std::string getLogLevel(){ return get("log.level"); }
    static int getHttpPort() { return std::stoi(get("http.port")); }
    static int getWebSocketPort() { return std::stoi(get("websocket.port")); }
};