#pragma once

#include "DBConnection.hpp"
#include <fstream>
#include <sstream>

class DBInitializer {
private:
    static std::string loadSQL(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open schema file: " + path);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

public:
    static void init(DBConnection& db)
    {
        db.execute("PRAGMA foreign_keys = ON;");
        std::vector<std::string> schemas =
            Config::getSchemaPaths();

        for (const auto& path : schemas) {
            std::string sql = loadSQL(path);
            db.execute(sql);
        }
    }
};