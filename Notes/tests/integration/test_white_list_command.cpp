#include <gtest/gtest.h>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "UdevDeviceResolver.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class WhiteListDeviceCommandTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;
    UdevDeviceResolver resolver;
    MockMountSystem mock;
    std::unique_ptr<Facade> facade;

    void SetUp() override {
        logger.disable();
        dbHelper.create();
        facade = std::make_unique<Facade>(
                    dbHelper.get_db(), 
                    mock, 
                    resolver);
    }

    void TearDown() override {
        facade.reset();
        logger.restore();
        dbHelper.reset();
    }
};

TEST_F(WhiteListDeviceCommandTest, ReturnsWhitelistFill) {
    // ARRANGE
    const std::string vendorId = "1234";
    const std::string productId = "ABCD";

    const DeviceInfo info = 
            DeviceInfoBuilder()
            .withProductId(productId)
            .withVendorId(vendorId)
            .withSerial("ACXDIFTGX6459KOD")
            .build();            
    facade->devices().addToWhitelist(info);
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 1);
    EXPECT_EQ(*cmd.list[0].info.vendorId, vendorId);
}

TEST_F(WhiteListDeviceCommandTest, ReturnsEmptyWhiteList) {
    // ARRANGE
    GetWhiteListDeviceCommand cmd;

    // ACT
    facade->execute(cmd);

    // ASSERT
    ASSERT_EQ(cmd.list.size(), 0);
}