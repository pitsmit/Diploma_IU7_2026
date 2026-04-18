#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class DevLogger {
private:
    std::shared_ptr<spdlog::logger> logger;

    DevLogger() {
        logger = spdlog::basic_logger_mt("system_logger", "logs.txt");
        logger->set_level(spdlog::level::info);
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

#define logger DevLogger::instance().get()