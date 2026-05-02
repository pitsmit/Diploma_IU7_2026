#pragma once

#include "LogRepository.hpp"
#include <iostream>

class LogManager {
private:
    LogRepository repo;

public:
    explicit LogManager(DBConnection& db)
        : repo(db) {}

    void setLogEvents(const std::vector<LogEventOption>& options) {
        repo.setEvents(options);
    }

    std::vector<LogEventOption> getLogEvents() {
        return repo.getEvents();
    }

    void log(const SecurityEvent& event)
    {
        std::cout
        << "[LOG] "
        << event.mountPath
        << "\n";
    }
};