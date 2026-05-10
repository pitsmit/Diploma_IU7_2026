#include <gtest/gtest.h>
#include <iostream>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "MountRecord.hpp"
#include "DeviceInfo.hpp"
#include "UdevDeviceResolver.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class DeleteDeviceCommandTest : public ::testing::Test {
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
        dbHelper.reset();
        logger.restore();
    }
};

TEST_F(DeleteDeviceCommandTest, DeleteOk) {
    // ARRANGE
    DeviceInfo info = 
            DeviceInfoBuilder()
            .withVendorId("1234")
            .withProductId("ABCD")
            .build();

    MountRecord d = 
            MountRecordBuilder()
            .withDevNode("/dev/test")
            .withMountPoint("/media/test")
            .withInfo(info)
            .build();
        
    std::string validTo = "2099-01-01";
    d.id = facade->devices().addToWhitelist(d, validTo);
    facade->registry().add(d);
    DeleteDeviceFromWhiteListCommand cmd(d.id);

    // ACT
    facade->execute(cmd);

    // ASSERT
    auto devices = facade->devices().getWhitelist();
    auto it = std::find_if(devices.begin(), devices.end(),
        [&](const auto& dev) {
            return dev.id == d.id;
        });
    EXPECT_EQ(it, devices.end());
    EXPECT_FALSE(facade->registry().getById(d.id).has_value());
}