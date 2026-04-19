#pragma once

#include "LogRepository.hpp"

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
};