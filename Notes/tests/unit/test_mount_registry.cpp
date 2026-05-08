#include <gtest/gtest.h>

#include "MountRegistry.hpp"
#include "MountRecord.hpp"
#include "DevLogger.hpp"
#include "../helpers/LoggerTestHelper.hpp"

class MountRegistryTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    MountRegistry reg;
    MountRecordBuilder builder;

    void SetUp() override
    {
        logger.disable();
    }

    void TearDown() override
    {
        logger.restore();
    }
};

TEST_F(MountRegistryTest, AddAndGet_ReturnsValue) {
    // ARRANGE
    const size_t id = 1;
    const std::string devNode = "/dev/sda1";
    const std::string mountPoint =
        "/media/dlp/1234_ABCD_XYZ";

    reg.add(
        builder
            .withId(id)
            .withDevNode(devNode)
            .withMountPoint(mountPoint)
            .build()
    );

    // ACT
    auto record = reg.getByDevNode(devNode);

    // ASSERT
    ASSERT_TRUE(record.has_value());

    EXPECT_EQ(record->id, id);
    EXPECT_EQ(record->devNode, devNode);
    EXPECT_EQ(record->mountPoint, mountPoint);
}

TEST_F(MountRegistryTest, Get_NonExisting_ReturnsNullopt) {
    // ARRANGE
    const std::string devNode =
        "/dev/does_not_exist";

    // ACT
    auto record =
        reg.getByDevNode(devNode);

    // ASSERT
    EXPECT_FALSE(record.has_value());
}

TEST_F(MountRegistryTest, Add_OverwriteExistingValue) {
    // ARRANGE
    const std::string devNode = "/dev/sda1";
    const std::string firstMount = "first";
    const std::string secondMount = "second";

    reg.add(
        builder
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint(firstMount)
            .build()
    );

    reg.add(
        builder
            .withId(2)
            .withDevNode(devNode)
            .withMountPoint(secondMount)
            .build()
    );

    // ACT
    auto record =
        reg.getByDevNode(devNode);

    // ASSERT
    ASSERT_TRUE(record.has_value());

    EXPECT_EQ(record->id, 2);
    EXPECT_EQ(record->mountPoint, secondMount);
}

TEST_F(MountRegistryTest, Remove_DeletesEntry) {
    // ARRANGE
    const std::string devNode = "/dev/sda1";
    const std::string mountPoint = "mount";

    reg.add(
        builder
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint(mountPoint)
            .build()
    );

    reg.removeByDevNode(devNode);

    // ACT
    auto record =
        reg.getByDevNode(devNode);

    // ASSERT
    EXPECT_FALSE(record.has_value());
}

TEST_F(MountRegistryTest, GetAll_ReturnsAllRecords) {
    // ARRANGE
    const std::string devNode1 = "/dev/sda1";
    const std::string devNode2 = "/dev/sdb1";
    const std::string devNode3 = "/dev/sdc1";

    reg.add(
        builder
            .withId(1)
            .withDevNode(devNode1)
            .withMountPoint("m1")
            .build()
    );

    reg.add(
        builder
            .withId(2)
            .withDevNode(devNode2)
            .withMountPoint("m2")
            .build()
    );

    reg.add(
        builder
            .withId(3)
            .withDevNode(devNode3)
            .withMountPoint("m3")
            .build()
    );

    // ACT
    auto records = reg.getAll();

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

TEST_F(MountRegistryTest, Exists_ExistingRecord_ReturnsTrue) {
    // ARRANGE
    const std::string devNode = "/dev/sda1";

    reg.add(
        builder
            .withId(1)
            .withDevNode(devNode)
            .withMountPoint("mount")
            .build()
    );

    // ACT
    bool exists =
        reg.exists(devNode);

    // ASSERT
    EXPECT_TRUE(exists);
}

TEST_F(MountRegistryTest, Exists_NonExistingRecord_ReturnsFalse) {
    // ARRANGE
    const std::string devNode = "/dev/unknown";

    // ACT
    bool exists =
        reg.exists(devNode);

    // ASSERT
    EXPECT_FALSE(exists);
}

TEST_F(MountRegistryTest, Size_ReturnsCorrectCount) {
    // ARRANGE
    reg.add(
        builder
            .withId(1)
            .withDevNode("/dev/sda1")
            .withMountPoint("m1")
            .build()
    );

    reg.add(
        builder
            .withId(2)
            .withDevNode("/dev/sdb1")
            .withMountPoint("m2")
            .build()
    );

    // ACT
    size_t count = reg.size();

    // ASSERT
    EXPECT_EQ(count, 2);
}