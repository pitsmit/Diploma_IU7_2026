#include <gtest/gtest.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "../helpers/E2EHelper.hpp"

using json = nlohmann::json;

TEST(GetWhiteListE2E, ReturnsWhiteList)
{
    // ARRANGE
    const std::string vendorId = "1234";
    const std::string productId = "ABCD";

    json body = {
        {"id", 1},
        {"devNode", "/dev/sdb1"},
        {"mountPoint", "/mnt/usb"},
        {
            "info",
            {
                {"vendorId", vendorId},
                {"productId", productId},
                {"serial", "123456"},
                {"vendorName", "SanDisk"},
                {"productName", "Ultra"}
            }
        },
        {"mode", "RW"},
        {"validTo", "2027-01-01"}
    };

    long postCode = 0;

    request(
        "POST",
        "http://app:8080/api/test/seed/whitelist",
        body.dump(),
        postCode);

    ASSERT_EQ(postCode, 201);
    long getCode = 0;

    // ACT
    std::string response = request(
        "GET",
        "http://app:8080/api/v1/whitelist",
        "",
        getCode);

    // ASSERT
    ASSERT_EQ(getCode, 200);

    json result = json::parse(response);

    ASSERT_TRUE(result.is_array());
    ASSERT_EQ(result.size(), 1);

    EXPECT_EQ(result[0]["info"]["vendorId"], vendorId);
    EXPECT_EQ(result[0]["info"]["productId"], productId);
}