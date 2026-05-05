#include <gtest/gtest.h>

#include "MountRegistry.hpp"
#include "DevLogger.hpp"
#include "../helpers/LoggerTestHelper.hpp"

class MountRegistryTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;

    void SetUp() override {
        logger.disable();
    }

    void TearDown() override {
        logger.restore();
    }
};

TEST_F(MountRegistryTest, AddAndGet_ReturnsValue) {
    // ARRANGE
    MountRegistry reg;
    reg.add("/dev/sda1", "/media/dlp/1234_ABCD_XYZ");
    std::string out;

    // ACT
    bool found = reg.get("/dev/sda1", out);

    // ASSERT
    EXPECT_TRUE(found);
    EXPECT_EQ(out, "/media/dlp/1234_ABCD_XYZ");
}

TEST_F(MountRegistryTest, Get_NonExisting_ReturnsFalse) {
    // ARRANGE
    MountRegistry reg;
    std::string out;

    // ACT
    bool found = reg.get("/dev/does_not_exist", out);

    // ASSERT
    EXPECT_FALSE(found);
}

TEST_F(MountRegistryTest, Add_OverwriteExistingValue) {
    // ARRANGE
    MountRegistry reg;
    reg.add("/dev/sda1", "first");
    reg.add("/dev/sda1", "second");
    std::string out;

    // ACT
    reg.get("/dev/sda1", out);

    // ASSERT
    EXPECT_EQ(out, "second");
}

TEST_F(MountRegistryTest, Remove_DeletesEntry) {
    // ARRANGE
    MountRegistry reg;
    reg.add("/dev/sda1", "mount");
    reg.remove("/dev/sda1");
    std::string out;

    // ACT
    bool found = reg.get("/dev/sda1", out);

    // ASSERT
    EXPECT_FALSE(found);
}

TEST_F(MountRegistryTest, GetAllDevNodes_ReturnsAllKeys) {
    // ARRANGE
    MountRegistry reg;
    reg.add("/dev/sda1", "m1");
    reg.add("/dev/sdb1", "m2");
    reg.add("/dev/sdc1", "m3");

    // ACT
    auto nodes = reg.getAllDevNodes();

    // ASSERT
    EXPECT_EQ(nodes.size(), 3);
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sda1"), nodes.end());
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sdb1"), nodes.end());
    EXPECT_NE(std::find(nodes.begin(), nodes.end(), "/dev/sdc1"), nodes.end());
}