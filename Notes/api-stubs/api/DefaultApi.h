#pragma once

#include "ApiBase.h"
#include "Facade.hpp"
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

    // HTTP handlers
    void add_device_to_white_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void get_usb_white_list_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
    void default_handler(const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response);
};

}