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

#include <thread>
#include <chrono>

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

public:
    App()
        : db(Config::getDBPath()),
          facade(db, linms, resolver),
          #ifdef BUILD_HTTP_SERVER
          http(facade),
          #endif
          rec(facade.registry(), resolver, facade.mounts())
    {
        DBInitializer::init(db);
    }

    void run()
    {
        rec.run();
        
        EventQueue<DeviceEvent> queue;

        UdevWatcher watcher(queue);

        DeviceControlService service(
            facade.registry(),
            facade.mounts()
        );

        EventLoop loop(queue, service);

        std::jthread watcherThread([&] {
            watcher.run();
        });

        std::jthread loopThread([&] {
            loop.run();
        });

        #ifdef BUILD_HTTP_SERVER
        std::jthread httpThread([&] {
            http.start();
        });
        #endif

        while (true) {
            std::this_thread::sleep_for(
                std::chrono::seconds(10));
        }
    }
};

int main() {  
    App app;
    app.run();
}