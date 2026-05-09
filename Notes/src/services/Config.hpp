#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

class Config {
private:
    static inline const std::string configFile = "config.txt";
    static inline std::unordered_map<std::string, std::string> cache;

    static void load()
    {
        if (!cache.empty()) {
            return;
        }

        std::ifstream file(configFile);
        if (!file.is_open()) {
            return;
        }

        std::string line;

        while (std::getline(file, line)) {

            if (line.empty() || line[0] == '#') {
                continue;
            }

            auto pos = line.find('=');
            if (pos == std::string::npos) {
                continue;
            }

            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            cache[key] = value;
        }
    }

    static std::string get(const std::string& key)
    {
        load();

        auto it = cache.find(key);
        if (it == cache.end()) {
            return "";
        }

        return it->second;
    }

public:
    static int getHttpPort()
    {
        std::string v = get("http.port");
        return v.empty() ? 8080 : std::stoi(v);
    }

    static std::vector<std::string> getSchemaPaths()
    {
        std::string v = get("db.schema");

        if (v.empty()) {
            return {
                "sql/schema/001_device_info.sql"
            };
        }

        std::vector<std::string> result;
        std::stringstream ss(v);
        std::string item;

        while (std::getline(ss, item, ';')) {
            if (!item.empty())
                result.push_back(item);
        }

        return result;
    }

    static std::string getDBPath()
    {
        std::string v = get("db.path");
        return v.empty()
            ? "app.db"
            : v;
    }

    static std::string getLogFile()
    {
        std::string v = get("log.file");
        return v.empty() 
            ? "logs.txt" 
            : v;
    }

    static std::string getLogLevel()
    {
        std::string v = get("log.level");
        return v.empty() 
            ? "info" 
            : v;
    }
};