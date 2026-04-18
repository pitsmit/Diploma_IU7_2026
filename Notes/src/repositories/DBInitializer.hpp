#pragma once

#include "DBConnection.hpp"
#include <string>

class DBInitializer {
public:
    static void init(DBConnection& db) {
        db.execute(R"(
CREATE TABLE IF NOT EXISTS "Device" (
	id INTEGER NOT NULL,
	name TEXT,
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
	validTo DATE,
	PRIMARY KEY(id),
	UNIQUE(vendorId, productId, serial)
);

CREATE TABLE IF NOT EXISTS "Process" (
	id INTEGER NOT NULL,
	name TEXT,
	path TEXT NOT NULL UNIQUE,
	validTo DATE,
	PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS "Event" (
	id INTEGER NOT NULL,
	name TEXT NOT NULL UNIQUE,
	isJournaling BOOLEAN NOT NULL DEFAULT false,
	PRIMARY KEY(id)
);

CREATE TABLE IF NOT EXISTS "Record" (
	id INTEGER NOT NULL,
	datetime DATETIME NOT NULL,
	eventId INTEGER NOT NULL,
	deviceName TEXT,
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
	procName TEXT,
	procPath TEXT,
	filePath TEXT,
	result TEXT NOT NULL CHECK (result IN ('success', 'denied', 'error')),
	PRIMARY KEY(id),
	FOREIGN KEY (eventId) REFERENCES "Event"(id)
	ON UPDATE NO ACTION ON DELETE NO ACTION
);

        )");
    }
};