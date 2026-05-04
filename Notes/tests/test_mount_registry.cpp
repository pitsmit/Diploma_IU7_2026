#include <gtest/gtest.h>
#include "MountRegistry.hpp"
#include "DevLogger.hpp"

TEST(MountRegistryTest, AddAndGet_ReturnsValue) {
    auto mock = std::make_shared<spdlog::logger>("test", std::make_shared<spdlog::sinks::sink>());
    DevLogger::instance().setLogger(mock);
    MountRegistry reg;

    reg.add("/dev/sda1", "/media/dlp/1234_ABCD_XYZ");

    std::string out;
    bool found = reg.get("/dev/sda1", out);

    EXPECT_TRUE(found);
    EXPECT_EQ(out, "/media/dlp/1234_ABCD_XYZ");
}

TEST(MountRegistryTest, Get_NonExisting_ReturnsFalse) {
    MountRegistry reg;

    std::string out;
    bool found = reg.get("/dev/does_not_exist", out);

    EXPECT_FALSE(found);
}

TEST(MountRegistryTest, Add_OverwriteExistingValue) {
    MountRegistry reg;

    reg.add("/dev/sda1", "first");
    reg.add("/dev/sda1", "second");

    std::string out;
    reg.get("/dev/sda1", out);

    EXPECT_EQ(out, "second");
}

TEST(MountRegistryTest, Remove_DeletesEntry) {
    MountRegistry reg;

    reg.add("/dev/sda1", "mount");
    reg.remove("/dev/sda1");

    std::string out;
    bool found = reg.get("/dev/sda1", out);

    EXPECT_FALSE(found);
}

TEST(MountRegistryTest, GetAllDevNodes_ReturnsAllKeys) {
    MountRegistry reg;

    reg.add("/dev/sda1", "m1");
    reg.add("/dev/sdb1", "m2");
    reg.add("/dev/sdc1", "m3");

    auto nodes = reg.getAllDevNodes();

    EXPECT_EQ(nodes.size(), 3);
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sda1"), nodes.end());
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sdb1"), nodes.end());
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sdc1"), nodes.end());
}