#pragma once

#include "DBConnection.hpp"
#include "Device.hpp"
#include <vector>
#include <string>

class DeviceRepository {
private:
    DBConnection& db;

public:
    explicit DeviceRepository(DBConnection& connection)
        : db(connection) {}

    void add(const Device& device) {
        std::string sql =
            "INSERT INTO Device (name, vendorId, productId, serial, validTo) VALUES ('" +
            device.name + "','" +
            device.vendorId + "','" +
            device.productId + "','" +
            device.serial + "','" +
            device.validTo + "');";

        db.execute(sql);
    }

    std::vector<Device> getAll() {
        std::vector<Device> result;

        db.query(
            "SELECT id, name, vendorId, productId, serial, validTo FROM Device;",
            [&](int cols, char** values, char** names) {

                Device d;

                d.id = std::stoull(values[0] ? values[0] : "0");
                d.name = values[1] ? values[1] : "";
                d.vendorId = values[2] ? values[2] : "";
                d.productId = values[3] ? values[3] : "";
                d.serial = values[4] ? values[4] : "";
                d.validTo = values[5] ? values[5] : "";

                result.push_back(d);
            });

        return result;
    }

    void remove(size_t id) {
        db.execute(
            "DELETE FROM Device WHERE id = " + std::to_string(id) + ";"
        );
    }

    void updateValidTo(size_t id, const std::string& validTo) {
        db.execute(
            "UPDATE Device SET validTo = '" + validTo +
            "' WHERE id = " + std::to_string(id) + ";"
        );
    }
};