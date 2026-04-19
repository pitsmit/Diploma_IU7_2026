#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class DevLogger {
private:
    std::shared_ptr<spdlog::logger> logger;

    DevLogger() {
        if (!spdlog::get("system_logger")) {
            logger = spdlog::basic_logger_mt("system_logger", "logs.txt");
        } else {
            logger = spdlog::get("system_logger");
        }

        logger->set_level(spdlog::level::info);
        logger->flush_on(spdlog::level::info);
    }

public:
    static DevLogger& instance() {
        static DevLogger inst;
        return inst;
    }

    std::shared_ptr<spdlog::logger>& get() {
        return logger;
    }
};

#define mylog DevLogger::instance().get()