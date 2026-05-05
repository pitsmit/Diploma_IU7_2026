#pragma once

#include "DBConnection.hpp"
#include "Device.hpp"
#include "DeviceInfo.hpp"

#include <vector>
#include <string>

class DeviceRepository {
private:
    DBConnection& db;

    static std::string sqlValue(const std::optional<std::string>& field) {
        return field.has_value()
            ? "'" + *field + "'"
            : "NULL";
    }

public:
    explicit DeviceRepository(DBConnection& connection)
        : db(connection) {}

    void add(const Device& device) {
        std::string sql =
            "INSERT INTO Device "
            "(vendorId, productId, serial, productName, vendorName, validTo) VALUES (" +
            sqlValue(device.info.vendorId) + "," +
            sqlValue(device.info.productId) + "," +
            sqlValue(device.info.serial) + "," +
            sqlValue(device.info.productName) + "," +
            sqlValue(device.info.vendorName) + "," +
            sqlValue(device.validTo) + ");";
        
        db.execute(sql);
    }

    std::vector<Device> getAll() {
        std::vector<Device> result;

        db.query(
            "SELECT id, vendorId, productId, serial, productName, vendorName, validTo FROM Device;",
            [&](int /*cols*/, char** values, char** /*names*/) {

                Device d;

                d.id = std::stoull(values[0] ? values[0] : "0");
                d.info.vendorId = values[1] ? values[1] : "";
                d.info.productId = values[2] ? values[2] : "";
                d.info.serial = values[3] ? values[3] : "";
                d.info.productName = values[4] ? values[4] : "";
                d.info.vendorName = values[5] ? values[5] : "";
                d.validTo = values[6] ? values[6] : "";

                result.push_back(d);
            });

        return result;
    }

    void remove(size_t id) {
        db.execute(
            "DELETE FROM Device WHERE id = " + std::to_string(id) + ";"
        );
    }

    void updateValidTo(size_t id, std::optional<std::string> validTo) {
        db.execute(
            "UPDATE Device SET validTo = " + sqlValue(validTo) +
            " WHERE id = " + std::to_string(id) + ";"
        );
    }

    bool exists(const DeviceInfo& info) {
        if (!info.vendorId || !info.productId) {
            return false;
        }

        bool found = false;

        std::string sql =
            "SELECT 1 FROM Device WHERE "
            "vendorId = '" + *info.vendorId + "' AND "
            "productId = '" + *info.productId + "'";

        if (info.serial) {
            sql += " AND serial = '" + *info.serial + "'";
        } else {
            sql += " AND serial IS NULL";
        }

        sql += " AND validTo >= date('now')";
        sql += " LIMIT 1;";

        db.query(
            sql,
            [&](int /*cols*/, char** /*values*/, char** /*names*/) {
                found = true;
            });

        return found;
    }
};