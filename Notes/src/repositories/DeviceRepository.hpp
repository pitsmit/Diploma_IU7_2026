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

    int ensureDeviceInfo(const DeviceInfo& info)
    {
        std::string sql =
            "SELECT id FROM DeviceInfo WHERE "
            "vendorId = " + sqlValue(info.vendorId) + " AND "
            "productId = " + sqlValue(info.productId) + " AND "
            "serial = " + sqlValue(info.serial);

        sql += " LIMIT 1;";

        int id = 0;

        db.query(sql,
            [&](int /*cols*/, char** values, char** /*names*/) {
                if (values && values[0]) {
                    id = std::stoi(values[0]);
                }
            });

        if (id != 0)
            return id;

        std::string insert =
            "INSERT INTO DeviceInfo "
            "(vendorId, productId, serial, productName, vendorName) VALUES (" +
            sqlValue(info.vendorId) + "," +
            sqlValue(info.productId) + "," +
            sqlValue(info.serial) + "," +
            sqlValue(info.productName) + "," +
            sqlValue(info.vendorName) + ");";

        db.execute(insert);

        return db.lastInsertId();
    }

public:
    explicit DeviceRepository(DBConnection& connection)
        : db(connection) {}

    int add(const DeviceInfo& dev) {
        int deviceInfoId = ensureDeviceInfo(dev);
        std::string sql =
            "INSERT INTO Device "
            "(deviceInfoId, validTo) VALUES (" +
            std::to_string(deviceInfoId) + "," +
            "NULL);";

        db.execute(sql);
        return db.lastInsertId();
    }

    std::vector<Device> getAll() {
        std::vector<Device> result;

        db.query(
            "SELECT d.id, di.vendorId, di.productId, di.serial, "
            "di.productName, di.vendorName, d.validTo "
            "FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id;",
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

    int findActiveId(const DeviceInfo& info)
    {
        int id = 0;

        std::string sql =
            "SELECT d.id FROM Device d "
            "JOIN DeviceInfo di ON d.deviceInfoId = di.id "
            "WHERE di.vendorId = " + sqlValue(info.vendorId) + " AND "
            "di.productId = " + sqlValue(info.productId) + " AND "
            "di.serial = " + sqlValue(info.serial);

        sql += " AND (d.validTo IS NULL "
            "OR d.validTo >= date('now'))";
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