#include "Facade.hpp"
#include "DBInitializer.hpp"
#ifdef BUILD_HTTP_SERVER
#include "HttpServer.hpp"
#endif
#include "EventQueue.hpp"
#include "Watcher.hpp"
#include "EventLoop.hpp"
#include "Config.hpp"
#include "LinuxMountSystem.hpp"
#include "UdevDeviceResolver.hpp"
#include "MountRecoveryService.hpp"
#include "WebSocketServer.hpp"
#include "DeviceEventNotifier.hpp"

#include <thread>
#include <chrono>
#include <iostream>

class App {
private:
    DBConnection db;
    Facade facade;
    #ifdef BUILD_HTTP_SERVER
    HttpServer http;
    #endif
    LinuxMountSystem linms;
    UdevDeviceResolver resolver;
    MountRecoveryService rec;
    WebSocketServer ws;
    DeviceEventNotifier notifier;
    EventQueue<DeviceEvent> queue;
    UdevWatcher watcher;
    DeviceControlService service;
    EventLoop loop;

public:
    App()
        : db(Config::getDBPath()),
          ws(Config::getWebSocketPort()),
          notifier(ws),
          service(facade.registry(), facade.mounts(), notifier),
          loop(queue, service),
          watcher(queue),
          facade(db, linms, resolver),
          #ifdef BUILD_HTTP_SERVER
          http(facade),
          #endif
          rec(facade.registry(), resolver, facade.mounts(), facade.devices())
        {
            DBInitializer::init(db);
            rec.run();
            ws.start();
        }

    void run()
    {
        std::jthread watcherThread([&] {
            try {
                watcher.run();
            }
            catch (const std::exception& e) {
                std::cerr << "watcher thread: "
                        << e.what()
                        << std::endl;
            }
        });

        std::jthread loopThread([&] {
            try {
                loop.run();
            }
            catch (const std::exception& e) {
                std::cerr << "loop thread: "
                        << e.what()
                        << std::endl;
            }
        });

    #ifdef BUILD_HTTP_SERVER
        std::jthread httpThread([&] {
            try {
                http.start();
            }
            catch (const std::exception& e) {
                std::cerr << "http thread: "
                        << e.what()
                        << std::endl;
            }
        });
    #endif

        for (;;) {
            std::this_thread::sleep_for(
                std::chrono::seconds(10));
        }
    }
};

int main() { App().run(); }