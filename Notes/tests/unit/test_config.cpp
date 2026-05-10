#include <gtest/gtest.h>

#include <fstream>
#include <cstdio>
#include <string>
#include <unordered_map>

#include "Config.hpp"

class ConfigFileBuilder {
private:
    std::unordered_map<std::string, std::string> data;

public:
    ConfigFileBuilder& add(const std::string& key, const std::string& value) {
        data[key] = value;
        return *this;
    }

    void build(const std::string& path = "config.txt") {
        std::ofstream f(path);

        for (const auto& [k, v] : data) {
            f << k << "=" << v << "\n";
        }
    }
};

TEST(ConfigTest, MissingConfigFile)
{
    // ARRANGE
    std::rename("config.txt", "config.txt.bak");

    // ASSERT
    EXPECT_THROW(Config::getHttpPort(), FileException);
    std::rename("config.txt.bak", "config.txt");
}

TEST(ConfigTest, MissingHttpPortKey)
{
    // ARRANGE
    ConfigFileBuilder()
        .add("db.path", "/tmp/db")
        .add("log.file", "/tmp/log")
        .build();

    // ASSERT
    EXPECT_THROW(Config::getHttpPort(), FileException);
}

TEST(ConfigTest, ValidConfigReturnsHttpPort)
{
    // ARRANGE
    ConfigFileBuilder()
        .add("http.port", "8080")
        .add("db.path", "/tmp/db")
        .add("log.file", "/tmp/log")
        .build();

    // ACT
    int port = Config::getHttpPort();

    // ASSERT
    EXPECT_EQ(port, 8080);
}