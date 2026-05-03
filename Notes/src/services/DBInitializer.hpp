#pragma once

#include "DBConnection.hpp"
#include <fstream>
#include <sstream>

class DBInitializer {
private:
    static std::string loadSQL(const std::string& path) {
        std::ifstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

public:
    static void init(DBConnection& db)
    {
        std::string schemaPath = Config::getSchemaPath();
        db.execute(loadSQL(schemaPath));
    }
};