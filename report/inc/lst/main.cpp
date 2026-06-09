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