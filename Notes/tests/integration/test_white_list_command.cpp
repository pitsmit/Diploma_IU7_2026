#include <gtest/gtest.h>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "MountRecord.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class WhiteListDeviceCommandTest : public ::testing::Test {
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

TEST_F(WhiteListDeviceCommandTest, ReturnsWhitelistFill) {
    // ARRANGE
    MountRecord d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    std::string validTo = "2099-01-01";
    facade->devices().addToWhitelist(d, validTo);
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 1);
    EXPECT_EQ(*cmd.list[0].info.vendorId, "1234");
}

TEST_F(WhiteListDeviceCommandTest, ReturnsEmptyWhiteList) {
    // ARRANGE
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 0);
}