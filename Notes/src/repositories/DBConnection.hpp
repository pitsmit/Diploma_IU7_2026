#pragma once

#include <string>
#include <sqlite3.h>
#include <stdexcept>
#include <functional>
#include "DevLogger.hpp"

class DBConnection {
private:
    sqlite3* db = nullptr;

public:
    explicit DBConnection(const std::string& dbPath) {
        if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("Failed to open SQLite database");
        }
    }

    ~DBConnection() {
        if (db) {
            sqlite3_close(db);
        }
    }

    DBConnection(const DBConnection&) = delete;
    DBConnection& operator=(const DBConnection&) = delete;

    // =========================
    // EXEC (INSERT / UPDATE / DELETE)
    // =========================
    void execute(const std::string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            std::string error = errMsg ? errMsg : "Unknown SQLite error";
            sqlite3_free(errMsg);
            throw std::runtime_error(error);
        }   
    }

    // =========================
    // QUERY (SELECT)
    // callback(row) -> bool continue?
    // =========================
    void query(
    const std::string& sql,
    std::function<void(int, char**, char**)> callback
    ) {
        char* errMsg = nullptr;

        auto trampoline = [](void* data, int cols, char** values, char** names) -> int {
            auto* cb = static_cast<std::function<void(int, char**, char**)>*>(data);
            (*cb)(cols, values, names);
            return 0;
        };

        if (sqlite3_exec(
            db,
            sql.c_str(),
            trampoline,
            &callback,
            &errMsg
        ) != SQLITE_OK) {

            std::string error = errMsg ? errMsg : "Query error";
            sqlite3_free(errMsg);
            throw std::runtime_error(error);
        }
    }
};