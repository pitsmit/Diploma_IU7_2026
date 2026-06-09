#include <string>
#include <unordered_map>
#include <fstream>

class ConfigFileBuilder {
private:
    std::unordered_map<std::string, std::string> data;

public:
    ConfigFileBuilder& add(
        const std::string& key,
        const std::string& value)
    {
        data[key] = value;
        return *this;
    }

    void build(const std::string& path = "config.txt")
    {
        std::ofstream f(path, std::ios::trunc);

        for (const auto& [k, v] : data) {
            f << k << "=" << v << "\n";
        }
    }
};