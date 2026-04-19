#pragma once

#include "DBConnection.hpp"
#include "LogEventOption.hpp"
#include "EventType.hpp"
#include <vector>

class LogRepository {
private:
    DBConnection& db;

public:
    explicit LogRepository(DBConnection& connection)
        : db(connection) {}

    void setEvents(const std::vector<LogEventOption>& options) {
        for (const auto& opt : options) {
            std::string sql =
                "UPDATE Event SET isJournaling = " +
                std::to_string(opt.enabled ? 1 : 0) +
                " WHERE name = '" + toString(opt.type) + "';";

            db.execute(sql);
        }
    }

    std::vector<LogEventOption> getEvents() {
        std::vector<LogEventOption> result;

        db.query(
            "SELECT name, isJournaling FROM Event;",
            [&](int cols, char** values, char** names) {

                std::string name = values[0] ? values[0] : "";
                bool enabled = values[1] ? std::stoi(values[1]) : 0;

                LogEventOption opt;
                opt.type = fromString(name);
                opt.enabled = enabled;

                result.push_back(opt);
            });

        return result;
    }
};