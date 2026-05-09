CREATE TABLE IF NOT EXISTS DeviceInfo (
    id INTEGER PRIMARY KEY AUTOINCREMENT,

    vendorId TEXT NOT NULL CHECK (
        length(vendorId) = 4 AND
        vendorId NOT GLOB '*[^0-9A-Fa-f]*'
    ),

    productId TEXT NOT NULL CHECK (
        length(productId) = 4 AND
        productId NOT GLOB '*[^0-9A-Fa-f]*'
    ),

    serial TEXT,

    vendorName TEXT,
    productName TEXT,

    UNIQUE(vendorId, productId, serial)
);