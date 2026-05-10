#include <gtest/gtest.h>
#include <string>
#include <filesystem>

#include "MountPointBuilder.hpp"

const std::string &base = "/media/dlp/";

TEST(MountPointBuilderTest, Build_AllFieldsPresent) {
    // ARRANGE
    const std::string &vendorId = "1234";
    const std::string &productId = "7856";
    const std::string &serial = "ACDC456IRHX";
    const std::string &productName = "Galaxy";
    const std::string &vendorName = "Samsung";

    DeviceInfo dev = DeviceInfoBuilder()
                        .withVendorId(vendorId)
                        .withProductId(productId)
                        .withSerial(serial)
                        .withProductName(productName)
                        .withVendorName(vendorName)
                        .build();
    
    const std::string &expected = base + vendorId + "_" + productId + "_" + serial;

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, expected);
}

TEST(MountPointBuilderTest, Build_NoSerial) {
    // ARRANGE
    const std::string &vendorId = "1234";
    const std::string &productId = "7856";

    DeviceInfo dev = DeviceInfoBuilder()
                        .withVendorId(vendorId)
                        .withProductId(productId)
                        .build();

    const std::string &expected = base + vendorId + "_" + productId + "_noserial";

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, expected);
}

TEST(MountPointBuilderTest, Build_MissingVendorAndProduct) {
    // ARRANGE
    DeviceInfo dev = DeviceInfoBuilder().build();
    const std::string &expected = base + "unknown_unknown_noserial";

    // ACT
    std::string result = MountPointBuilder::build(dev);

    // ASSERT
    EXPECT_EQ(result, expected);
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