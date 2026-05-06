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

                DeviceInfoBuilder infoBuilder;

                if (values[1]) infoBuilder.withVendorId(values[1]);
                if (values[2]) infoBuilder.withProductId(values[2]);
                if (values[3]) infoBuilder.withSerial(values[3]);
                if (values[4]) infoBuilder.withProductName(values[4]);
                if (values[5]) infoBuilder.withVendorName(values[5]);

                DeviceBuilder deviceBuilder;

                if (values[6]) deviceBuilder.withValidTo(values[6]);

                Device device = deviceBuilder
                    .withId(std::stoull(values[0] ? values[0] : "0"))
                    .withInfo(infoBuilder.build())
                    .build();

                result.push_back(device);
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