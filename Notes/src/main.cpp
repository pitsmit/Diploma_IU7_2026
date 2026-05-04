#include "Facade.hpp"
#include "DBInitializer.hpp"
#include "HttpServer.hpp"
#include "EventQueue.hpp"
#include "Watcher.hpp"
#include "EventLoop.hpp"
#include "Config.hpp"

#include <thread>
#include <chrono>

class App {
private:
    DBConnection db;
    Facade facade;
    HttpServer http;

public:
    App()
        : db(Config::getDBPath()),
          facade(),
          http(facade)
    {
        DBInitializer::init(db);
    }

    void run()
    {
        EventQueue<DeviceEvent> queue;

        UdevWatcher watcher(queue);

        DeviceControlService service(
            facade.devices(),
            facade.policies(),
            facade.registry()
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