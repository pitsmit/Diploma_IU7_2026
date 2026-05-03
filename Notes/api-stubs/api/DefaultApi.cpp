#include "DefaultApi.h"
#include "Facade.hpp"
#include "DeviceCommands.hpp"

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

void DefaultApi::init() {
    setupRoutes();
}

void DefaultApi::setupRoutes() {
    using namespace Pistache::Rest;

    Routes::Post(*router, "/api/v1/whitelist/usbdev",
        Routes::bind(&DefaultApi::add_device_to_white_list_handler, this));
    
    Routes::Delete(*router, "/api/v1/whitelist/usbdev/:id",
        Routes::bind(&DefaultApi::delete_device_from_white_list_handler, this));

    Routes::Patch(*router, "/api/v1/whitelist/usbdev/:id",
        Routes::bind(&DefaultApi::patch_valid_to_device_handler, this));

    Routes::Get(*router, "/api/v1/whitelist/usbdev",
        Routes::bind(&DefaultApi::get_usb_white_list_handler, this));
    
    Routes::Get(*router, "/api/v1/list/usbdev",
        Routes::bind(&DefaultApi::get_current_connected_devices_list_handler, this));
}

void DefaultApi::add_device_to_white_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        auto body = request.body();
        std::cout << "Request body: " << body << std::endl;
        if (body.empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Empty request body");
            return;
        }

        nlohmann::json json_body = nlohmann::json::parse(body);
        
        Device d;
        from_json(json_body, d);
        
        AddDeviceToWhiteListCommand command(d);
        facade.execute(command);
        
        response.send(Pistache::Http::Code::Created, "Device added to white list");
        
    } catch (const nlohmann::json::parse_error& e) {
        response.send(Pistache::Http::Code::Bad_Request, 
                     "Invalid JSON format: " + std::string(e.what()));
    } catch (const std::bad_alloc& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, 
                     "Memory allocation error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, 
                     "Error: " + std::string(e.what()));
    }
}

void DefaultApi::delete_device_from_white_list_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        auto id_param = request.param(":id");
        if (id_param.as<std::string>().empty()) {
            response.send(Pistache::Http::Code::Bad_Request, "Missing device ID");
            return;
        }
        
        size_t device_id;
        try {
            device_id = std::stoul(id_param.as<std::string>());
        } catch (const std::exception& e) {
            response.send(Pistache::Http::Code::Bad_Request, "Invalid device ID format");
            return;
        }
        
        DeleteDeviceFromWhiteListCommand command(device_id);
        facade.execute(command);
        
        response.send(Pistache::Http::Code::No_Content);
        
    } catch (const std::exception& e) {
        response.send(Pistache::Http::Code::Internal_Server_Error, 
                     "Error deleting device: " + std::string(e.what()));
    }
}

void DefaultApi::patch_valid_to_device_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response)
{
    try {
        // -------- Path param --------
        auto id_param = request.param(":id");

        if (id_param.as<std::string>().empty()) {
            response.send(
                Pistache::Http::Code::Bad_Request,
                "Missing device ID"
            );
            return;
        }

        size_t device_id;

        try {
            device_id = std::stoul(id_param.as<std::string>());
        } catch (const std::exception&) {
            response.send(
                Pistache::Http::Code::Bad_Request,
                "Invalid device ID format"
            );
            return;
        }

        // -------- Body --------
        if (request.body().empty()) {
            response.send(
                Pistache::Http::Code::Bad_Request,
                "Request body is empty"
            );
            return;
        }

        nlohmann::json body;

        try {
            body = nlohmann::json::parse(request.body());
        } catch (const std::exception&) {
            response.send(
                Pistache::Http::Code::Bad_Request,
                "Invalid JSON"
            );
            return;
        }

        if (!body.contains("validTo")) {
            response.send(
                Pistache::Http::Code::Bad_Request,
                "Missing field: validTo"
            );
            return;
        }

        std::string valid_to;

        if (!body["validTo"].is_null()) {
            if (!body["validTo"].is_string()) {
                response.send(
                    Pistache::Http::Code::Bad_Request,
                    "validTo must be string or null"
                );
                return;
            }

            valid_to = body["validTo"].get<std::string>();

            // Простая проверка YYYY-MM-DD
            static const std::regex date_regex(
                R"(^\d{4}-\d{2}-\d{2}$)"
            );

            if (!std::regex_match(valid_to, date_regex)) {
                response.send(
                    Pistache::Http::Code::Bad_Request,
                    "Invalid date format. Expected YYYY-MM-DD"
                );
                return;
            }
        }

        // -------- Execute command --------
        PatchValidToDeviceCommand command(device_id, valid_to);

        facade.execute(command);

        response.send(Pistache::Http::Code::Ok);

    } catch (const std::exception& e) {
        response.send(
            Pistache::Http::Code::Internal_Server_Error,
            "Error updating device validTo: " + std::string(e.what())
        );
    }
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

void DefaultApi::get_current_connected_devices_list_handler(
    const Pistache::Rest::Request&,
    Pistache::Http::ResponseWriter response)
{
    GetCurrentConnectedDevicesCommand command;

    facade.execute(command);

    json j = command.devices;

    response.send(
        Pistache::Http::Code::Ok,
        j.dump()
    );
}

}