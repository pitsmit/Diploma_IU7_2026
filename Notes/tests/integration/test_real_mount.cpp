#include <gtest/gtest.h>

#include <cstdlib>
#include <filesystem>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "DeviceControlService.hpp"
#include "LinuxMountSystem.hpp"
#include "DeviceInfo.hpp"
#include "DeviceManager.hpp"
#include "DeviceEvent.hpp"
#include "MountRegistry.hpp"
#include "MountUtils.hpp"
#include "UdevDeviceResolver.hpp"
#include "MountService.hpp"
#include "DeviceEventNotifier.hpp"

#include "../helpers/DataBaseTestHelper.hpp"
#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/MountHelpers.hpp"
#include "../mocks/MockDeviceResolver.hpp"
#include "../mocks/MockWebSocketServer.hpp"

class RealMountTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;
    LinuxMountSystem sys;
    std::unique_ptr<MountRegistryManager> registry;
    std::unique_ptr<DeviceControlService> svc;
    std::unique_ptr<MountUtils> utils;
    std::unique_ptr<DeviceManager> devicemanager;
    std::unique_ptr<MockDeviceResolver> resolver;
    std::unique_ptr<MountService> mntser;
    std::unique_ptr<MockWebSocketServer> sct_server;
    std::unique_ptr<DeviceEventNotifier> notifier;

    std::unique_ptr<DeviceInfo> info;

    void SetUp() override {
        logger.disable();
        dbHelper.create();

        utils = std::make_unique<MountUtils>(sys);
        devicemanager = std::make_unique<DeviceManager>(dbHelper.get_db());
        resolver = std::make_unique<MockDeviceResolver>();
        mntser = std::make_unique<MountService>(*devicemanager, *utils, *resolver);
        registry = std::make_unique<MountRegistryManager>(dbHelper.get_db());
        sct_server = std::make_unique<MockWebSocketServer>(1);
        notifier = std::make_unique<DeviceEventNotifier>(*sct_server);

        svc = std::make_unique<DeviceControlService>(
            *registry,
            *mntser,
            *notifier
        );

        info = std::make_unique<DeviceInfo>(
            DeviceInfoBuilder()
                .withVendorId("1234")
                .withProductId("5678")
                .withSerial("ACXDIFTGX6459KOD")
                .build()
        );

        resolver->setResult(*info);
    }

    void TearDown() override {
        info.reset();
        svc.reset();
        notifier.reset();
        sct_server.reset();
        registry.reset();
        mntser.reset();
        resolver.reset();
        devicemanager.reset();
        utils.reset();
        dbHelper.reset();
        logger.restore();
    }
};

TEST_F(RealMountTest, MountLoopDeviceReadOnlyVFAT)
{
    // ARRANGE
    std::string devNode = createLoopFs("vfat");
    std::unique_ptr<DeviceEvent> event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode.c_str())
                .build());

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mount_path = registry->getMountPointByDevNode(devNode);

    ASSERT_TRUE(isMounted(*mount_path));
    ASSERT_TRUE(isMountedAs(*mount_path, "ro"));

    int status = simulate_write(*mount_path);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_NE(WEXITSTATUS(status), 0);

    svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode.c_str()});
}

TEST_F(RealMountTest, MountLoopDeviceReadWriteVFAT)
{
    // ARRANGE
    dbHelper.get_repo().add(*info);
    std::string devNode = createLoopFs("vfat");
    std::unique_ptr<DeviceEvent> event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode.c_str())
                .build());

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mount_path = registry->getMountPointByDevNode(devNode);
    ASSERT_TRUE(isMounted(*mount_path));
    ASSERT_TRUE(isMountedAs(*mount_path, "rw"));

    int status = simulate_write(*mount_path);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_EQ(WEXITSTATUS(status), 0);

    svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode.c_str()});
}

TEST_F(RealMountTest, MountLoopDeviceReadOnlyEXT4)
{
    // ARRANGE
    std::string devNode = createLoopFs("ext4");
    std::unique_ptr<DeviceEvent> event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode.c_str())
                .build());

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mount_path = registry->getMountPointByDevNode(devNode);

    ASSERT_TRUE(isMounted(*mount_path));
    ASSERT_TRUE(isMountedAs(*mount_path, "ro"));

    int status = simulate_write(*mount_path);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_NE(WEXITSTATUS(status), 0);

    svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode.c_str()});
}

TEST_F(RealMountTest, MountLoopDeviceReadWriteEXT4)
{
    // ARRANGE
    dbHelper.get_repo().add(*info);
    std::string devNode = createLoopFs("ext4");
    std::unique_ptr<DeviceEvent> event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode.c_str())
                .build());

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mount_path = registry->getMountPointByDevNode(devNode);
    ASSERT_TRUE(isMounted(*mount_path));
    ASSERT_TRUE(isMountedAs(*mount_path, "rw"));

    int status = simulate_write(*mount_path);
    ASSERT_TRUE(WIFEXITED(status));
    ASSERT_EQ(WEXITSTATUS(status), 0);

    svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode.c_str()});
}