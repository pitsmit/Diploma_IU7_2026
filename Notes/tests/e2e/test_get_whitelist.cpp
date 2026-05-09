#include <gtest/gtest.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

static size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    void* userp)
{
    ((std::string*)userp)->append(
        (char*)contents,
        size * nmemb);

    return size * nmemb;
}

TEST(GetWhiteListE2E, ReturnsEmptyList)
{
    CURL* curl = curl_easy_init();

    ASSERT_NE(curl, nullptr);

    std::string response;

    curl_easy_setopt(
        curl,
        CURLOPT_URL,
        "http://app:8080/api/v1/whitelist/");

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback);

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &response);

    CURLcode res = curl_easy_perform(curl);

    ASSERT_EQ(res, CURLE_OK);

    long httpCode = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &httpCode);

    EXPECT_EQ(httpCode, 200);

    auto json = nlohmann::json::parse(response);

    ASSERT_TRUE(json.is_array());

    curl_easy_cleanup(curl);
}