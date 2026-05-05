#include <gtest/gtest.h>

#include "Device.hpp"
#include "DeviceInfo.hpp"

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
    Device d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.info.serial = "ABCDEF123456";
    d.validTo = "2099-01-01";
    dbHelper.get_repo().add(d);

    // ACT
    auto all = dbHelper.get_repo().getAll();

    // ASSERT
    ASSERT_EQ(all.size(), 1);
    EXPECT_EQ(*all[0].info.vendorId, "1234");
}

TEST_F(DeviceRepositoryTest, Exists_ReturnsTrue) {
    // ARRANGE
    Device d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.validTo = "2099-01-01";
    dbHelper.get_repo().add(d);

    // ACT
    bool r = dbHelper.get_repo().exists(d.info);

    // ASSERT
    EXPECT_TRUE(r);
}

TEST_F(DeviceRepositoryTest, Exists_Expired_ReturnsFalse) {
    // ARRANGE
    Device d;
    d.info.vendorId = "1234";
    d.info.productId = "ABCD";
    d.validTo = "2000-01-01";
    dbHelper.get_repo().add(d);

    // ACT
    bool r = dbHelper.get_repo().exists(d.info);

    // ASSERT
    EXPECT_FALSE(r);
}