void handleEvent(const DeviceEvent& event) {
    mylog->info("Start handle {} event with devnode {}", 
        event.type == EventType::INSERT ? "INSERT" : "REMOVE", event.devNode);
    if (event.type == EventType::INSERT) {
        auto record = mountManager_.mount(event.devNode);
        mountRegistry_.add(*record);