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
#include "MountRecord.hpp"
#include "UdevDeviceResolver.hpp"
#include "MountManager.hpp"

#include "../helpers/DataBaseTestHelper.hpp"
#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/MountHelpers.hpp"
#include "../mocks/MockDeviceResolver.hpp"

class RealMountTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;
    std::string devNode;
    LinuxMountSystem sys;
    std::unique_ptr<MountRegistry> registry;
    std::unique_ptr<DeviceControlService> svc;
    std::unique_ptr<MountUtils> utils;
    std::unique_ptr<PolicyManager> policy;
    std::unique_ptr<MockDeviceResolver> resolver;
    std::unique_ptr<MountManager> mntman;

    std::unique_ptr<DeviceInfo> info;
    std::unique_ptr<DeviceEvent> event;
    std::unique_ptr<MountRecord> mount_rec;

    void SetUp() override {
        logger.disable();
        dbHelper.create();
        devNode = createLoopFs();

        utils = std::make_unique<MountUtils>(sys);
        policy = std::make_unique<PolicyManager>(dbHelper.get_db());
        resolver = std::make_unique<MockDeviceResolver>();
        mntman = std::make_unique<MountManager>(*policy, *utils, *resolver);
        registry = std::make_unique<MountRegistry>(dbHelper.get_db());

        svc = std::make_unique<DeviceControlService>(
            *registry,
            *mntman
        );

        info = std::make_unique<DeviceInfo>(
            DeviceInfoBuilder()
                .withVendorId("1234")
                .withProductId("5678")
                .build()
        );

        resolver->setResult(*info);

        event = std::make_unique<DeviceEvent>(
            DeviceEventBuilder()
                .withType(EventType::INSERT)
                .withDevNode(devNode.c_str())
                .build()
        );

        mount_rec = std::make_unique<MountRecord>(
            MountRecordBuilder()
                .withInfo(*info)
                .withDevNode(devNode)
                .withMountPoint("/dev/test")
                .build()
        );


    }

    void TearDown() override {
        svc->handleEvent(DeviceEvent{EventType::REMOVE, devNode.c_str()});

        mount_rec.reset();
        event.reset();
        info.reset();
        svc.reset();
        registry.reset();
        mntman.reset();
        resolver.reset();
        policy.reset();
        utils.reset();
        dbHelper.reset();
        logger.restore();
    }
};

TEST_F(RealMountTest, MountLoopDeviceReadOnly)
{
    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mount_rec = registry->getByDevNode(devNode);

    ASSERT_TRUE(mount_rec.has_value());
    ASSERT_TRUE(isMounted(mount_rec->mountPoint));
    ASSERT_TRUE(isMountedAs(mount_rec->mountPoint, "ro"));
}

TEST_F(RealMountTest, MountLoopDeviceReadWrite)
{
    // ARRANGE
    std::string validTo = "2099-01-01";
    dbHelper.get_repo().add(*mount_rec, validTo);

    // ACT
    svc->handleEvent(*event);

    // ASSERT
    auto mnt_r = registry->getByDevNode(devNode);
    ASSERT_TRUE(mnt_r.has_value());
    ASSERT_TRUE(isMounted(mnt_r->mountPoint));
    ASSERT_TRUE(isMountedAs(mnt_r->mountPoint, "rw"));
}