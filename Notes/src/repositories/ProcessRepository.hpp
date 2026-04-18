#pragma once

#include "DBConnection.hpp"
#include "Process.hpp"
#include <vector>
#include <string>

class ProcessRepository {
private:
    DBConnection& db;

public:
    explicit ProcessRepository(DBConnection& connection)
        : db(connection) {}

    void add(const Process& process) {
        std::string sql =
            "INSERT INTO Process (name, path, validTo) VALUES ('" +
            process.name + "','" +
            process.path + "','" +
            process.validTo + "');";

        db.execute(sql);
    }

    void remove(size_t id) {
        db.execute(
            "DELETE FROM Process WHERE id = " + std::to_string(id) + ";"
        );
    }

    void updateValidTo(size_t id, const std::string& validTo) {
        db.execute(
            "UPDATE Process SET validTo = '" + validTo +
            "' WHERE id = " + std::to_string(id) + ";"
        );
    }

    std::vector<Process> getAll() {
        std::vector<Process> result;

        db.query("SELECT id, name, path, validTo FROM Process;",
            [&](int cols, char** values, char** names) {

                Process p;

                p.id = std::stoull(values[0] ? values[0] : "0");
                p.name = values[1] ? values[1] : "";
                p.path = values[2] ? values[2] : "";
                p.validTo = values[3] ? values[3] : "";

                result.push_back(p);
            });

        return result;
    }
};