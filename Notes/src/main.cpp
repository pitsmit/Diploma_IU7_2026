#include "Facade.hpp"
#include "DBInitializer.hpp"
#include "HttpServer.hpp"
#include "EventQueue.hpp"
#include "Watcher.hpp"
#include "EventLoop.hpp"
#include "Config.hpp"
#include "LinuxMountSystem.hpp"
#include "UdevDeviceResolver.hpp"

#include <thread>
#include <chrono>

class App {
private:
    DBConnection db;
    Facade facade;
    HttpServer http;
    LinuxMountSystem linms;

public:
    App()
        : db(Config::getDBPath()),
          facade(db, linms),
          http(facade)
    {
        DBInitializer::init(db);
    }

    void run()
    {
        EventQueue<DeviceEvent> queue;
        UdevDeviceResolver resolver;

        UdevWatcher watcher(queue);

        DeviceControlService service(
            facade.policies(),
            facade.registry(),
            facade.utils(),
            resolver
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