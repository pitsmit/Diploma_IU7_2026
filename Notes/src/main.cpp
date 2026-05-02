#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "DBInitializer.hpp"
#include "HttpServer.hpp"
#include <thread>
#include <chrono>

#include <thread>

#include "EventQueue.hpp"
#include "Watcher.hpp"
#include "EventLoop.hpp"

class App {
private:
    DBConnection db;
    Facade facade;
    HttpServer http;

public:
    App()
        : db("app.db"),
          facade(),
          http(facade)
    {
        DBInitializer::init(db);
        mylog->info("System start");
    }

    void run()
    {
        EventQueue<MountEvent> queue;

        MountWatcher watcher(queue);

        DeviceControlService service(
            facade.devices(),
            facade.policies()
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