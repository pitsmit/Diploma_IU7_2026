#include <gtest/gtest.h>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "Device.hpp"
#include "DeviceInfo.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class GetWhiteListDeviceCommandTest : public ::testing::Test {
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

TEST_F(GetWhiteListDeviceCommandTest, ReturnsWhitelistFill) {
    // ARRANGE
    Device d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.validTo = "2099-01-01";
    facade->devices().addToWhitelist(d);
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 1);
    EXPECT_EQ(*cmd.list[0].info.vendorId, "1234");
}

TEST_F(GetWhiteListDeviceCommandTest, ReturnsEmptyWhiteList) {
    // ARRANGE
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 0);
}