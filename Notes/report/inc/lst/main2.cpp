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
          rec(facade.registry(), resolver, facade.mounts(), facade.devices()) {
            DBInitializer::init(db);
            rec.run();
            ws.start();
        }
    void run() {
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