/*#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

struct SecurityEvent {
    EventType type;
    std::string device;
    std::string process;
    std::string timestamp;
};

class EventQueue {
private:
    std::queue<SecurityEvent> queue;
    std::mutex mtx;
    std::condition_variable cv;

public:
    void push(const SecurityEvent& event) {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(event);
        cv.notify_one();
    }

    SecurityEvent pop() {
        std::unique_lock<std::mutex> lock(mtx);

        cv.wait(lock, [&] {
            return !queue.empty();
        });

        SecurityEvent event = queue.front();
        queue.pop();
        return event;
    }
};

class Watcher {
public:
    virtual void start(EventQueue& queue) = 0;
    virtual ~Watcher() = default;
};

class UdevWatcher : public Watcher {
public:
    void start(EventQueue& queue) override {
        // mock event
        queue.push({EventType::INSERT, "usb0", "mock_process", "now"});
    }
};

class MountWatcher : public Watcher {
public:
    void start(EventQueue& queue) override {
        queue.push({EventType::READ, "mount0", "mock_process", "now"});
    }
};

class FanotifyWatcher : public Watcher {
public:
    void start(EventQueue& queue) override {
        queue.push({EventType::WRITE, "file.txt", "mock_process", "now"});
    }
};

class PolicyManager {
public:
    bool isDeviceAllowed(const std::string&) { return true; }
    bool isProcessAllowed(const std::string&) { return true; }
};

class DeviceControlService {
private:
    DeviceManager deviceManager;
    ProcessManager processManager;
    LogManager logManager;
    SettingsManager settingsManager;
    PolicyManager policyManager;

public:
    void handleEvent(const SecurityEvent& event) {

        if (!settingsManager.isEnabled(event.type))
            return;

        if (!policyManager.isDeviceAllowed(event.device))
            return;

        if (!policyManager.isProcessAllowed(event.process))
            return;

        deviceManager.handleDevice(event.device);
        processManager.handleProcess(event.process);
        logManager.log(event);
    }
};

class EventLoop {
private:
    EventQueue& queue;
    DeviceControlService service;

public:
    EventLoop(EventQueue& q) : queue(q) {}

    void run() {
        while (true) {
            SecurityEvent event = queue.pop();
            service.handleEvent(event);
            std::cout << event.device << std::endl;
        }
    }
};*/

#include "Facade.hpp"
#include "DeviceCommands.hpp"
#include "ProcessCommands.hpp"
#include "LogCommands.hpp"
#include "DBInitializer.hpp"

class App {
private:
    DBConnection db;
    Facade facade;

public:
    App()
        : db("app.db"),
          facade()
    {
        db.execute("PRAGMA foreign_keys = ON;");
        DBInitializer::init(db);
        logger->info("System start");
    }

    void run() {
        Device d;
        d.name = "USB Drive";
        d.vendorId = "1234";
        d.productId = "5678";
        d.serial = "ABCDEF123456";
        d.validTo = "2026-12-31";

        AddDeviceToWhiteListCommand addCmd(d);
        facade.execute(addCmd);

        GetWhiteListDeviceCommand getCmd;
        facade.execute(getCmd);
    }
};

int main() {
    /*EventQueue queue;

    UdevWatcher udev;
    MountWatcher mount;
    FanotifyWatcher fan;

    EventLoop loop(queue);

    // имитация запуска watcher'ов
    udev.start(queue);
    mount.start(queue);
    fan.start(queue);

    // обработка
    loop.run();

    return 0;*/  
    
    App app;
    app.run();
}