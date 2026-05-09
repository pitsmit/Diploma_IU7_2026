CREATE TABLE IF NOT EXISTS MountRecord (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    deviceInfoId INTEGER NOT NULL,

    devNode TEXT NOT NULL UNIQUE,
    mountPoint TEXT NOT NULL UNIQUE,

    mode TEXT NOT NULL CHECK (
        mode IN ('RO', 'RW')
    ),

    FOREIGN KEY(deviceInfoId)
        REFERENCES DeviceInfo(id)
);