#pragma once

#include "DBConnection.hpp"
#include "Device.hpp"
#include "DeviceInfo.hpp"
#include "MountRecord.hpp"
#include "DevLogger.hpp"

#include <vector>
#include <string>
#include <optional>

class MountRepository {
private:
    DBConnection& db;

    static std::string sqlValue(const std::optional<std::string>& field) {
        return field.has_value()
            ? "'" + *field + "'"
            : "NULL";
    }

    static std::string modeToStr(MODE m)
    {
        return m == MODE::RW ? "RW" : "RO";
    }

    static MODE parseMode(const char* v)
    {
        if (!v) return MODE::RO;
        return std::string(v) == "RW" ? MODE::RW : MODE::RO;
    }

    int ensureDeviceInfo(const DeviceInfo& info)
    {
        std::string sql =
            "SELECT id FROM DeviceInfo WHERE "
            "vendorId = " + sqlValue(info.vendorId) + " AND "
            "productId = " + sqlValue(info.productId) + " AND "
            "serial = " + sqlValue(info.serial) + " LIMIT 1;";

        int id = 0;

        db.query(sql,
            [&](int, char** values, char**) {
                if (values && values[0])
                    id = std::stoi(values[0]);
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
    explicit MountRepository(DBConnection& connection)
        : db(connection) {}

    void add(const MountRecord& record)
    {
        int deviceInfoId = ensureDeviceInfo(record.info);

        std::string sql =
            "INSERT INTO MountRecord "
            "(deviceInfoId, devNode, mountPoint, mode) VALUES (" +
            std::to_string(deviceInfoId) + "," +
            sqlValue(record.devNode) + "," +
            sqlValue(record.mountPoint) + "," +
            sqlValue(modeToStr(record.mode)) +
            ");";

        db.execute(sql);
    }

    void update(const MountRecord& record)
    {
        std::string findSql =
            "SELECT id FROM MountRecord WHERE devNode = " +
            sqlValue(record.devNode) + " LIMIT 1;";
        int mountId = 0;
        db.query(findSql,
            [&](int, char** v, char**) {
                if (v && v[0])
                    mountId = std::stoi(v[0]);
            });
        if (mountId == 0) {
            return;
        }

        int deviceInfoId = ensureDeviceInfo(record.info);

        std::string sql =
            "UPDATE MountRecord SET "
            "deviceInfoId = " + std::to_string(deviceInfoId) + ", "
            "mountPoint = " + sqlValue(record.mountPoint) + ", "
            "mode = " + sqlValue(modeToStr(record.mode)) +
            " WHERE devNode = " + sqlValue(record.devNode) + ";";

        db.execute(sql);
    }

    std::optional<MountRecord> getByDevNode(const std::string& devNode)
    {
        std::optional<MountRecord> result;

        std::string sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM MountRecord mr "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
            "WHERE mr.devNode = " + sqlValue(devNode) + " LIMIT 1;";

        db.query(sql,
            [&](int, char** v, char**) {
                if (!v || !v[0]) return;

                DeviceInfoBuilder infoBuilder;

                if (v[5]) infoBuilder.withVendorId(v[5]);
                if (v[6]) infoBuilder.withProductId(v[6]);
                if (v[7]) infoBuilder.withSerial(v[7]);
                if (v[8]) infoBuilder.withVendorName(v[8]);
                if (v[9]) infoBuilder.withProductName(v[9]);

                MountRecord record = MountRecordBuilder()
                    .withId(std::stoull(v[0]))
                    .withDevNode(v[2] ? v[2] : "")
                    .withMountPoint(v[3] ? v[3] : "")
                    .withMode(parseMode(v[4]))
                    .withInfo(infoBuilder.build())
                    .build();

                result = record;
            });

        return result;
    }

    std::optional<MountRecord> getById(const size_t& id)
    {
        std::optional<MountRecord> result;

        std::string sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM Device d "
            "JOIN MountRecord mr ON mr.deviceInfoId = d.deviceInfoId "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id "
            "WHERE d.id = " + std::to_string(id) + " LIMIT 1;";

        db.query(sql,
            [&](int, char** v, char**) {
                if (!v || !v[0]) return;

                DeviceInfoBuilder infoBuilder;

                if (v[5]) infoBuilder.withVendorId(v[5]);
                if (v[6]) infoBuilder.withProductId(v[6]);
                if (v[7]) infoBuilder.withSerial(v[7]);
                if (v[8]) infoBuilder.withVendorName(v[8]);
                if (v[9]) infoBuilder.withProductName(v[9]);

                MountRecord record = MountRecordBuilder()
                    .withId(std::stoull(v[0]))
                    .withDevNode(v[2] ? v[2] : "")
                    .withMountPoint(v[3] ? v[3] : "")
                    .withMode(parseMode(v[4]))
                    .withInfo(infoBuilder.build())
                    .build();

                result = record;
            });

        return result;
    }

    void removeByDevNode(const std::string& devNode)
    {
        std::string sql =
            "DELETE FROM MountRecord WHERE devNode = " + sqlValue(devNode) + ";";

        db.execute(sql);
    }

    std::vector<MountRecord> getAll()
    {
        std::vector<MountRecord> result;

        std::string sql =
            "SELECT mr.id, mr.deviceInfoId, mr.devNode, mr.mountPoint, mr.mode, "
            "di.vendorId, di.productId, di.serial, di.vendorName, di.productName "
            "FROM MountRecord mr "
            "JOIN DeviceInfo di ON mr.deviceInfoId = di.id;";

        db.query(sql,
            [&](int, char** v, char**) {
                if (!v || !v[0]) return;

                DeviceInfoBuilder infoBuilder;

                if (v[5]) infoBuilder.withVendorId(v[5]);
                if (v[6]) infoBuilder.withProductId(v[6]);
                if (v[7]) infoBuilder.withSerial(v[7]);
                if (v[8]) infoBuilder.withVendorName(v[8]);
                if (v[9]) infoBuilder.withProductName(v[9]);

                MountRecord record = MountRecordBuilder()
                    .withId(std::stoull(v[0]))
                    .withDevNode(v[2] ? v[2] : "")
                    .withMountPoint(v[3] ? v[3] : "")
                    .withMode(parseMode(v[4]))
                    .withInfo(infoBuilder.build())
                    .build();

                result.push_back(record);
            });

        return result;
    }
};