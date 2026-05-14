#include <gtest/gtest.h>

#include "MountRegistry.hpp"
#include "MountRecord.hpp"
#include "DevLogger.hpp"
#include "DeviceInfo.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"

class MountRegistryTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    std::unique_ptr<MountRegistry> reg;
    DataBaseTestHelper dbHelper;

    void SetUp() override
    {
        logger.disable();
        dbHelper.create();
        reg = std::make_unique<MountRegistry>(dbHelper.get_db());
    }

    void TearDown() override
    {
        logger.restore();
        dbHelper.reset();
        reg.reset();
    }
};

TEST_F(MountRegistryTest, AddAndGet_ReturnsValue) {
    // ARRANGE
    const size_t id = 1;
    const std::string devNode = "/dev/sda1";
    const std::string mountPoint =
        "/media/dlp/1234_ABCD_XYZ";
    const std::string vendorId = "1234";
    const std::string productId = "ABCD";
    const std::string serial = "ACXDIFTGX6459KOD";

    const DeviceInfo info = 
            DeviceInfoBuilder()
            .withProductId(productId)
            .withVendorId(vendorId)
            .withSerial(serial)
            .build();

    reg->add(
        MountRecordBuilder()
            .withId(id)
            .withDevNode(devNode)
            .withMountPoint(mountPoint)
            .withInfo(info)
            .withMode(MODE::RO)
            .build()
    );

    // ACT
    auto mntpt = reg->getMountPointByDevNode(devNode);

    // ASSERT
    ASSERT_TRUE(mntpt.has_value());
    EXPECT_EQ(*mntpt, mountPoint);
}

TEST_F(MountRegistryTest, Get_NonExisting_ReturnsNullopt) {
    // ARRANGE
    const std::string devNode =
        "/dev/does_not_exist";

    // ACT
    auto mntpt = reg->getMountPointByDevNode(devNode);

    // ASSERT
    EXPECT_FALSE(mntpt.has_value());
}

TEST_F(MountRegistryTest, Add_OverwriteExistingValue) {
    // ARRANGE
    const std::string devNode = "/dev/sda1";
    const std::string firstMount = "first";
    const std::string secondMount = "second";
    const std::string vendorId = "1234";
    const std::string productId = "ABCD";
    const std::string serial = "ACXDIFTGX6459KOD";
    const DeviceInfo info = 
            DeviceInfoBuilder()
            .withProductId(productId)
            .withVendorId(vendorId)
            .withSerial(serial)
            .build();

    reg->add(
        MountRecordBuilder()
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint(firstMount)
            .withInfo(info)
            .withMode(MODE::RO)
            .build()
    );

    reg->refresh(
        MountRecordBuilder()
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint(secondMount)
            .withInfo(info)
            .withMode(MODE::RO)
            .build()
    );

    // ACT
    auto mntpt =
        reg->getMountPointByDevNode(devNode);

    // ASSERT
    ASSERT_TRUE(mntpt.has_value());
    EXPECT_EQ(*mntpt, secondMount);
}

TEST_F(MountRegistryTest, Remove_DeletesEntry) {
    // ARRANGE
    const std::string devNode = "/dev/sda1";
    const std::string mountPoint = "mount";
    const std::string vendorId = "1234";
    const std::string productId = "ABCD";
    const std::string serial = "ACXDIFTGX6459KOD";
    const DeviceInfo info = 
            DeviceInfoBuilder()
            .withProductId(productId)
            .withVendorId(vendorId)
            .withSerial(serial)
            .build();

    reg->add(
        MountRecordBuilder()
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint(mountPoint)
            .withInfo(info)
            .withMode(MODE::RO)
            .build()
    );

    reg->removeByDevNode(devNode);

    // ACT
    auto mntpt =
        reg->getMountPointByDevNode(devNode);

    // ASSERT
    EXPECT_FALSE(mntpt.has_value());
}

TEST_F(MountRegistryTest, GetAll_ReturnsAllRecords) {
    // ARRANGE
    const std::string devNode1 = "/dev/sda1";
    const std::string devNode2 = "/dev/sdb1";
    const std::string devNode3 = "/dev/sdc1";

    const DeviceInfo info1 = 
            DeviceInfoBuilder()
            .withProductId("1234")
            .withVendorId("ABCD")
            .withSerial("ACXDIFTGX6459KOD")
            .build();

    const DeviceInfo info2 = 
            DeviceInfoBuilder()
            .withProductId("1244")
            .withVendorId("ABCD")
            .withSerial("ACXDIFTGX6459KRD")
            .build();

    const DeviceInfo info3 = 
            DeviceInfoBuilder()
            .withProductId("1254")
            .withVendorId("ABCD")
            .withSerial("ACXDIFTP86459KOD")
            .build();

    reg->add(
        MountRecordBuilder()
            .withId(1)
            .withDevNode(devNode1)
            .withMountPoint("m1")
            .withInfo(info1)
            .withMode(MODE::RO)
            .build()
    );

    reg->add(
        MountRecordBuilder()
            .withId(2)
            .withDevNode(devNode2)
            .withMountPoint("m2")
            .withInfo(info2)
            .withMode(MODE::RO)
            .build()
    );

    reg->add(
        MountRecordBuilder()
            .withId(3)
            .withDevNode(devNode3)
            .withMountPoint("m3")
            .withInfo(info3)
            .withMode(MODE::RO)
            .build()
    );

    // ACT
    auto records = reg->getAll();

    // ASSERT
    EXPECT_EQ(records.size(), 3);

    EXPECT_TRUE(
        std::any_of(
            records.begin(),
            records.end(),
            [&](const MountRecord& r) {
                return r.devNode == devNode1;
            }
        )
    );

    EXPECT_TRUE(
        std::any_of(
            records.begin(),
            records.end(),
            [&](const MountRecord& r) {
                return r.devNode == devNode2;
            }
        )
    );

    EXPECT_TRUE(
        std::any_of(
            records.begin(),
            records.end(),
            [&](const MountRecord& r) {
                return r.devNode == devNode3;
            }
        )
    );
}