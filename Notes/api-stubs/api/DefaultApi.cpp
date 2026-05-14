#include "DefaultApi.h"
#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "DeviceInfo.hpp"
#include "JsonUtils.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace api
{

DefaultApi::DefaultApi(
    const std::shared_ptr<Pistache::Rest::Router>& rtr,
    Facade& f)
    : ApiBase(rtr),
      facade(f)
{}

void DefaultApi::init()
{
    setupRoutes();
}

void DefaultApi::setupRoutes()
{
    using namespace Pistache::Rest;

    Routes::Post(*router, "/api/v1/whitelist",
        Routes::bind(&DefaultApi::add_device_to_white_list_handler, this));

    Routes::Delete(*router, "/api/v1/whitelist/:id",
        Routes::bind(&DefaultApi::delete_device_from_white_list_handler, this));

    Routes::Patch(*router, "/api/v1/whitelist/:id",
        Routes::bind(&DefaultApi::patch_valid_to_device_handler, this));

    Routes::Get(*router, "/api/v1/whitelist",
        Routes::bind(&DefaultApi::get_usb_white_list_handler, this));

    Routes::Get(*router, "/api/v1/list",
        Routes::bind(&DefaultApi::get_current_connected_devices_list_handler, this));

    #ifdef ENABLE_TEST_API
    Routes::Post(*router, "/api/test/seed/whitelist",
        Routes::bind(&DefaultApi::seed_whitelist_handler, this));
    #endif
}

void DefaultApi::add_device_to_white_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        json body = json::parse(request.body());

        MountRecord record = body.get<MountRecord>();
        std::optional<std::string> validTo;

        if (body.contains("validTo") && !body["validTo"].is_null()) {
            validTo = body["validTo"].get<std::string>();
        }

        AddDeviceToWhiteListCommand command(record, validTo);

        facade.execute(command);

        json result = {
            { "id", command.id }
        };

        response.send(
            Pistache::Http::Code::Created,
            result.dump(),
            MIME(Application, Json)
        );
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void DefaultApi::delete_device_from_white_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        size_t id = std::stoul(request.param(":id").as<std::string>());

        DeleteDeviceFromWhiteListCommand command(id);

        facade.execute(command);

        response.send(Pistache::Http::Code::No_Content);
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void DefaultApi::patch_valid_to_device_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        size_t id = std::stoul(request.param(":id").as<std::string>());

        json body = json::parse(request.body());

        std::optional<std::string> validTo;

        if (body.contains("validTo") && !body["validTo"].is_null()) {
            validTo = body["validTo"].get<std::string>();
        }

        PatchValidToDeviceCommand command(id, validTo);

        facade.execute(command);

        response.send(Pistache::Http::Code::Ok);
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void DefaultApi::get_usb_white_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        GetWhiteListDeviceCommand command;

        facade.execute(command);

        json j = command.list;

        response.send(Pistache::Http::Code::Ok, j.dump());
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

void DefaultApi::get_current_connected_devices_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        GetCurrentConnectedDevicesCommand command;

        facade.execute(command);

        json j = command.records;

        response.send(Pistache::Http::Code::Ok, j.dump());
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}

#ifdef ENABLE_TEST_API
void DefaultApi::seed_whitelist_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        json body = json::parse(request.body());

        DeviceInfo info = body.get<DeviceInfo>();

        std::optional<std::string> validTo;

        if (body.contains("validTo") && !body["validTo"].is_null()) {
            validTo = body["validTo"].get<std::string>();
        }

        facade.devices().addToWhitelist(info, validTo);

        response.send(Pistache::Http::Code::Created, "seeded");
    }
    catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    }
}
#endif

}