#pragma once

#include <memory>

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "DefaultApi.h"
#include "Config.hpp"
#include "Exceptions.hpp"

class HttpServer {
public:
    HttpServer(Facade& facade)
        : facade(facade) {}

    void start()
    {
        int port = Config::getHttpPort();

        try
        {
            Pistache::Address addr(
                Pistache::Ipv4::any(),
                Pistache::Port(port));

            endpoint =
                std::make_unique<Pistache::Http::Endpoint>(addr);
            router =
                std::make_shared<Pistache::Rest::Router>();
            auto opts =
                Pistache::Http::Endpoint::options()
                    .threads(1);

            endpoint->init(opts);
            api =
                std::make_shared<api::DefaultApi>(
                    router,
                    facade);

            api->init();
            endpoint->setHandler(router->handler());
            endpoint->serveThreaded();
            mylog->info(
                "Server started on port {}",
                port);
        }
        catch(const std::exception& e)
        {
            throw HttpServerError(e.what());
        }
    }

private:
    Facade& facade;
    std::unique_ptr<Pistache::Http::Endpoint> endpoint;
    std::shared_ptr<Pistache::Rest::Router> router;
    std::shared_ptr<api::DefaultApi> api;
};