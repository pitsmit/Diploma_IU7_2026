#pragma once

#include "ApiBase.h"
#include "Facade.hpp"
#include "JsonUtils.hpp"

#include <pistache/router.h>
#include <pistache/http.h>
#include <memory>

namespace api
{

class DefaultApi : public ApiBase {
public:
    explicit DefaultApi(const std::shared_ptr<Pistache::Rest::Router>& rtr, Facade& f);
    void init() override;

    static const std::string base;

private:
    void setupRoutes();
    Facade& facade;

    void add_device_to_white_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void delete_device_from_white_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void patch_valid_to_device_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void get_usb_white_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void get_current_connected_devices_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);

    #ifdef ENABLE_TEST_API
    void seed_whitelist_handler(
        const Pistache::Rest::Request&,
        Pistache::Http::ResponseWriter);
    #endif
};

}