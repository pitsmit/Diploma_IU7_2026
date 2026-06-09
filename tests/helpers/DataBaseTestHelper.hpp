#pragma once

#include <memory>

#include "DBInitializer.hpp"
#include "DBConnection.hpp"
#include "DeviceRepository.hpp"


class DataBaseTestHelper {
private:
    std::unique_ptr<DBConnection> db;
    std::unique_ptr<DeviceRepository> repo;

public:
    void create() {
        db = std::make_unique<DBConnection>(":memory:");
        DBInitializer::init(*db);
        repo = std::make_unique<DeviceRepository>(*db);
    }

    void reset() {
        repo.reset();
        db.reset();
    }

    DeviceRepository& get_repo() { return *repo; }
    DBConnection& get_db() { return *db; }
};