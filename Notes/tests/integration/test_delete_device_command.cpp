#include <gtest/gtest.h>

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "MountRecord.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"
#include "../mocks/MockMountSystem.hpp"

class DeleteDeviceCommandTest : public ::testing::Test {
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

TEST_F(DeleteDeviceCommandTest, DeleteOk) {
    // ARRANGE
    MountRecord d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.devNode = "/dev/test";
    d.mountPoint = "/media/test";
    std::string validTo = "2099-01-01";
    int id = facade->devices().addToWhitelist(d, validTo);
    DeleteDeviceFromWhiteListCommand cmd(id);

    // ACT
    facade->execute(cmd);

    // ASSERT
    auto devices = facade->devices().getWhitelist();
    auto it = std::find_if(devices.begin(), devices.end(),
        [&](const auto& dev) {
            return dev.id == id;
        });
    EXPECT_EQ(it, devices.end());
    EXPECT_FALSE(facade->registry().getById(id).has_value());
}