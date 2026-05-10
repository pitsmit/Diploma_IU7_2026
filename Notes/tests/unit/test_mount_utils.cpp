#include <string>
#include <gtest/gtest.h>

#include "MountUtils.hpp"
#include "Exceptions.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class MountUtilsTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    MockMountSystem mock;
    std::unique_ptr<MountUtils> utils;
    const std::string mountPath = "/mnt/test";
    const std::string devNode = "/dev/sda1";

    void SetUp() override {
        logger.disable();
        utils = std::make_unique<MountUtils>(mock);
    }

    void TearDown() override {
        utils.reset();
        logger.restore();
    }
};

TEST_F(MountUtilsTest, Mount_ReadWrite_Success) {
    // ACT
    utils->mountDevice(devNode, mountPath, false);

    // ASSERT
    EXPECT_TRUE(mock.syncCalled);
    EXPECT_TRUE(mock.mountCalled);
    EXPECT_EQ(mock.lastDev, devNode);
    EXPECT_EQ(mock.lastTarget, mountPath);
    EXPECT_EQ(mock.lastFs, "ext4");
    EXPECT_EQ(mock.lastOpts, "rw,umask=0000");
}

TEST_F(MountUtilsTest, Mount_ReadOnly_Success) {
    // ACT
    utils->mountDevice(devNode, mountPath, true);

    // ASSERT
    EXPECT_EQ(mock.lastOpts, "ro,umask=0000");
}

TEST_F(MountUtilsTest, Mount_ThrowsOnUnknownFs) {
    // ARRANGE
    mock.fsType = "";

    // ACT && ASSERT
    EXPECT_THROW(
        utils->mountDevice(devNode, mountPath, false),
        UnknownFsError
    );
}

TEST_F(MountUtilsTest, Mount_Failure_ReturnsError) {
    // ARRANGE
    mock.mountResult = -1;

    // ACT && ASSERT
    EXPECT_THROW(
        utils->mountDevice(devNode, mountPath, false),
        MountError
    );
}

TEST_F(MountUtilsTest, Unmount_Success) {
    // ARRANGE
    mock.umountResult = 0;

    // ACT
    utils->handleUnmount(mountPath);

    // ASSERT
    EXPECT_TRUE(mock.syncCalled);
    EXPECT_TRUE(mock.umountCalled);
    EXPECT_EQ(mock.lastTarget, mountPath);
}

TEST_F(MountUtilsTest, Unmount_Failure) {
    // ARRANGE
    mock.umountResult = -1;

    // ACT && ASSERT
    EXPECT_THROW(
        utils->handleUnmount(mountPath),
        UnMountError
    );
}