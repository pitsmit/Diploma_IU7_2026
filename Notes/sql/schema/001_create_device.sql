CREATE TABLE IF NOT EXISTS "Device" (
	id INTEGER NOT NULL,
	vendorId TEXT NOT NULL CHECK (
        length(vendorId) = 4 AND
        vendorId NOT GLOB '*[^0-9A-Fa-f]*'
    ),
	productId TEXT NOT NULL CHECK (
        length(productId) = 4 AND
        productId NOT GLOB '*[^0-9A-Fa-f]*'
    ),
	serial TEXT CHECK (
        serial IS NULL OR (
            length(serial) >= 12 AND
            serial NOT GLOB '*[^0-9A-F]*'
        )
    ),
    productName TEXT,
    vendorName TEXT,
	validTo DATE,
	PRIMARY KEY(id),
	UNIQUE(vendorId, productId, serial)
);