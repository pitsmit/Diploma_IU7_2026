#pragma once

#include "DBConnection.hpp"
#include "Device.hpp"
#include "DeviceInfo.hpp"
#include "MountRecord.hpp"

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

    int add(const MountRecord& d, std::optional<std::string> vld) {
        std::string sql =
            "INSERT INTO Device "
            "(vendorId, productId, serial, productName, vendorName, validTo) VALUES (" +
            sqlValue(d.info.vendorId) + "," +
            sqlValue(d.info.productId) + "," +
            sqlValue(d.info.serial) + "," +
            sqlValue(d.info.productName) + "," +
            sqlValue(d.info.vendorName) + "," +
            sqlValue(vld) + ");";
        
        db.execute(sql);
        return db.lastInsertId();
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

    int exists(const DeviceInfo& info)
    {
        if (!info.vendorId || !info.productId) {
            return 0;
        }

        int id = 0;

        std::string sql =
            "SELECT id FROM Device WHERE "
            "vendorId = '" + *info.vendorId + "' AND "
            "productId = '" + *info.productId + "'";

        if (info.serial) {
            sql += " AND serial = '" + *info.serial + "'";
        }
        else {
            sql += " AND serial IS NULL";
        }

        sql += " AND validTo >= date('now')";
        sql += " LIMIT 1;";

        db.query(
            sql,
            [&](int /*cols*/, char** values, char** /*names*/)
            {
                if (values && values[0]) {
                    id = std::stoi(values[0]);
                }
            });

        return id;
    }
};