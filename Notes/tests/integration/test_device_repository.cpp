#include <gtest/gtest.h>

#include "Device.hpp"
#include "DeviceInfo.hpp"
#include "MountRecord.hpp"

#include "../helpers/LoggerTestHelper.hpp"
#include "../helpers/DataBaseTestHelper.hpp"

class DeviceRepositoryTest : public ::testing::Test {
protected:
    LoggerTestHelper logger;
    DataBaseTestHelper dbHelper;

    void SetUp() override {
        logger.disable();
        dbHelper.create();
    }

    void TearDown() override {
        logger.restore();
        dbHelper.reset();
    }
};


TEST_F(DeviceRepositoryTest, AddAndGetAll) {
    // ARRANGE
    MountRecord r;
    r.info.vendorId = "1234";
    r.info.productId = "ABCD";
    r.info.serial = "ABCDEF123456";
    std::string validTo = "2099-01-01";
    dbHelper.get_repo().add(r, validTo);

    // ACT
    auto all = dbHelper.get_repo().getAll();

    // ASSERT
    ASSERT_EQ(all.size(), 1);
    EXPECT_EQ(*all[0].info.vendorId, "1234");
}

TEST_F(DeviceRepositoryTest, Exists_ReturnsTrue) {
    // ARRANGE
    MountRecord r;
    r.info.vendorId = "1234";
    r.info.productId = "ABCD";
    std::string validTo = "2099-01-01";
    dbHelper.get_repo().add(r, validTo);

    // ACT
    auto id = dbHelper.get_repo().exists(r.info);

    // ASSERT
    EXPECT_TRUE(id);
}

TEST_F(DeviceRepositoryTest, Exists_Expired_ReturnsFalse) {
    // ARRANGE
    MountRecord r;
    r.info.vendorId = "1234";
    r.info.productId = "ABCD";
    std::string validTo = "2005-01-01";
    dbHelper.get_repo().add(r, validTo);

    // ACT
    auto id = dbHelper.get_repo().exists(r.info);

    // ASSERT
    EXPECT_FALSE(id);
}