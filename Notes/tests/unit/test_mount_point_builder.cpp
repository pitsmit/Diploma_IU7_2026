#include <gtest/gtest.h>
#include <filesystem>

#include "MountPointBuilder.hpp"

TEST(MountPointBuilderTest, Build_AllFieldsPresent) {
    // ARRANGE
    DeviceInfo dev{
        .vendorId = "1234",
        .productId = "ABCD",
        .serial = "ABCDEF123456"
    };

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, "/media/dlp/1234_ABCD_ABCDEF123456");
}

TEST(MountPointBuilderTest, Build_NoSerial) {
    // ARRANGE
    DeviceInfo dev{
        .vendorId = "1111",
        .productId = "2222",
        .serial = std::nullopt
    };

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, "/media/dlp/1111_2222_noserial");
}

TEST(MountPointBuilderTest, Build_MissingVendorAndProduct) {
    // ARRANGE
    DeviceInfo dev{
        .vendorId = std::nullopt,
        .productId = std::nullopt,
        .serial = std::nullopt
    };

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, "/media/dlp/unknown_unknown_noserial");
}

TEST(MountPointBuilderTest, EnsureExists_CreatesDirectory) {
    // ARRANGE
    std::string path = "/tmp/test_mount_point_builder_dir";
    std::filesystem::remove_all(path);

    // ACT
    MountPointBuilder::ensureExists(path);

    // ASSERT
    EXPECT_TRUE(std::filesystem::exists(path));
}

TEST(MountPointBuilderTest, EnsureExists_Idempotent) {
    // ARRANGE
    std::string path = "/tmp/test_mount_point_builder_dir2";
    std::filesystem::remove_all(path);

    // ACT
    MountPointBuilder::ensureExists(path);
    MountPointBuilder::ensureExists(path);

    // ASSERT
    EXPECT_TRUE(std::filesystem::exists(path));
}