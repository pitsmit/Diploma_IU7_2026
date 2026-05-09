#include "Facade.hpp"
#include "DBInitializer.hpp"
#include "HttpServer.hpp"
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
    HttpServer http;
    LinuxMountSystem linms;
    UdevDeviceResolver resolver;
    MountRecoveryService rec;

public:
    App()
        : db(Config::getDBPath()),
          facade(db, linms, resolver),
          http(facade),
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

        std::jthread httpThread([&] {
            http.start();
        });

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