#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <string>

#include "DeviceControlService.hpp"
#include "LinuxMountSystem.hpp"
#include "PolicyManager.hpp"
#include "DeviceInfo.hpp"
#include "DeviceManager.hpp"
#include "DeviceEvent.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"

#include "../helpers/DataBaseTestHelper.hpp"
#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/MountHelpers.hpp"

class RealMountTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;
    std::string devNode;
    LinuxMountSystem sys;
    MountRegistry registry;
    std::unique_ptr<DeviceControlService> svc;
    std::unique_ptr<MountUtils> utils;
    std::unique_ptr<PolicyManager> policy;

    std::unique_ptr<DeviceInfo> info;
    std::unique_ptr<DeviceEvent> event;

    void SetUp() override {
        logger.disable();
        dbHelper.create();
        devNode = createLoopFs();

        utils = std::make_unique<MountUtils>(sys);
        policy = std::make_unique<PolicyManager>(dbHelper.get_db());

        svc = std::make_unique<DeviceControlService>(
            *policy,
            registry,
            *utils
        );

        info = std::make_unique<DeviceInfo>(
            DeviceInfoBuilder()
                .withVendorId("1234")
                .withProductId("5678")
                .build()
        );

        event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode)
                .withDeviceInfo(*info)
                .build()
        );
    }

    void TearDown() override {
        svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode});
        system("losetup -D");

        svc.reset();
        utils.reset();
        policy.reset();
        logger.restore();
        dbHelper.reset();
        event.reset();
        info.reset();
    }
};

TEST_F(RealMountTest, MountLoopDeviceReadOnly)
{
    // ACT
    svc->handleEvent(*event);

    // ASSERT
    std::string mountPoint;
    registry.get(devNode, mountPoint);

    ASSERT_TRUE(isMounted(mountPoint));
    ASSERT_TRUE(isMountedAs(mountPoint, "ro"));
}

TEST_F(RealMountTest, MountLoopDeviceReadWrite)
{
    // ARRANGE
    auto device = DeviceBuilder()
        .withInfo(*info)
        .withValidTo("2099-01-01")
        .build();

    dbHelper.get_repo().add(device);

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    std::string mountPoint;
    registry.get(devNode, mountPoint);

    ASSERT_TRUE(isMounted(mountPoint));
    ASSERT_TRUE(isMountedAs(mountPoint, "rw"));
}