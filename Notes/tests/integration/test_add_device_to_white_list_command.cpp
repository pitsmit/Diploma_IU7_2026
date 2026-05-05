#include <gtest/gtest.h>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "Device.hpp"
#include "DeviceInfo.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class AddDeviceToWhiteListCommandTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;

    MockMountSystem mock;
    std::unique_ptr<Facade> facade;

    void SetUp() override {
        logger.disable();
        dbHelper.create();

        facade = std::make_unique<Facade>(dbHelper.get_db(), mock);
    }

    void TearDown() override {
        facade.reset();
        logger.restore();
        dbHelper.reset();
    }
};

TEST_F(AddDeviceToWhiteListCommandTest, CorrectAdd) {
    // ARRANGE
    Device d;
    d.devNode = "/dev/sda1";
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.validTo = "2099-01-01";
    facade->registry().add("/dev/sda1", "/mnt/test");
    AddDeviceToWhiteListCommand cmd(d);

    // ACT
    facade->execute(cmd);

    // ASSERT
    auto all = facade->devices().getWhitelist();
    ASSERT_EQ(all.size(), 1);
    EXPECT_TRUE(mock.syncCalled);
    EXPECT_TRUE(mock.umountCalled);
    EXPECT_TRUE(mock.mountCalled);
    EXPECT_EQ(mock.lastTarget, "/mnt/test");
}