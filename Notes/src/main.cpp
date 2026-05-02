#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "ProcessCommands.hpp"
#include "LogCommands.hpp"
#include "DBInitializer.hpp"
#include "HttpServer.hpp"
#include <thread>
#include <chrono>

#include <thread>

#include "EventQueue.hpp"
#include "Watchers.hpp"
#include "EventLoop.hpp"

class App {
private:
    DBConnection db;
    Facade facade;
    //HttpServer http;

public:
    App()
        : db("app.db"),
          facade()
          //http(facade)
    {
        db.execute("PRAGMA foreign_keys = ON;");
        DBInitializer::init(db);
        mylog->info("System start");
    }

    void run() {
        //http.start();

        //while (true) {
        //    std::this_thread::sleep_for(std::chrono::seconds(10));
        //}

        EventQueue<SecurityEvent> queue;

        MountWatcher watcher(queue);

        DeviceControlService service(
            facade.devices(),
            facade.processes(),
            facade.logs(),
            facade.policies()
        );

        std::jthread watcherThread([&] {
            watcher.run();
        });

        EventLoop loop(queue, service);

        loop.run();

        spdlog::shutdown();

        watcherThread.join();
    }
};

int main() {  
    App app;
    app.run();
}