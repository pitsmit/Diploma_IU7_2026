#include "DefaultApi.h"
#include "Facade.hpp"
#include "DeviceCommands.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace api
{

const std::string DefaultApi::base = "";

DefaultApi::DefaultApi(
    const std::shared_ptr<Pistache::Rest::Router>& rtr,
    Facade& f)
    : ApiBase(rtr),
      facade(f)
{}

void DefaultApi::init() {
    setupRoutes();
}

void DefaultApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Post(*router, base + "/api/v1/whitelist/usbdev",
        Routes::bind(&DefaultApi::add_device_to_white_list_handler, this));

    Routes::Get(*router, base + "/api/v1/whitelist/usbdev",
        Routes::bind(&DefaultApi::get_usb_white_list_handler, this));

    router->addCustomHandler(
        Routes::bind(&DefaultApi::default_handler, this)
    );
}

void DefaultApi::add_device_to_white_list_handler(
    const Pistache::Rest::Request&,
    Pistache::Http::ResponseWriter response)
{
    response.send(Pistache::Http::Code::Ok, "device added");
}

void DefaultApi::get_usb_white_list_handler(
    const Pistache::Rest::Request&,
    Pistache::Http::ResponseWriter response)
{
    GetWhiteListDeviceCommand command;

    facade.execute(command);

    json j = command.list;

    response.send(
        Pistache::Http::Code::Ok,
        j.dump()
    );
}

void DefaultApi::default_handler(
    const Pistache::Rest::Request&,
    Pistache::Http::ResponseWriter response)
{
    response.send(Pistache::Http::Code::Not_Found, "not found");
}

}