#pragma once

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "DefaultApi.h"
#include "Config.hpp"

class HttpServer {
public:
    HttpServer(Facade& facade)
        : facade(facade) {}

    void start()
    {
        int port = Config::getHttpPort();

        Pistache::Address addr(
            Pistache::Ipv4::any(),
            Pistache::Port(port));

        endpoint = std::make_unique<Pistache::Http::Endpoint>(addr);
        auto router = std::make_shared<Pistache::Rest::Router>();
        auto opts = Pistache::Http::Endpoint::options().threads(1);
        endpoint->init(opts);
        auto api = std::make_shared<api::DefaultApi>(router, facade);
        api->init();

        endpoint->setHandler(router->handler());
        endpoint->serveThreaded();
        mylog->info("Server started on port {}", port);
    }

private:
    Facade& facade;
    std::unique_ptr<Pistache::Http::Endpoint> endpoint;
};