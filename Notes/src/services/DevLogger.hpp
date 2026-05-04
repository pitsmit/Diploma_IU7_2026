#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "Config.hpp"

class DevLogger {
private:
    std::shared_ptr<spdlog::logger> logger;

    DevLogger()
    {
        std::string file = Config::getLogFile();

        if (!spdlog::get("system_logger")) {
            logger = spdlog::basic_logger_mt("system_logger", file);
        } else {
            logger = spdlog::get("system_logger");
        }

        logger->set_level(parseLevel(Config::getLogLevel()));
        logger->flush_on(spdlog::level::info);
    }

    static spdlog::level::level_enum parseLevel(const std::string& lvl)
    {
        if (lvl == "debug") return spdlog::level::debug;
        if (lvl == "warn") return spdlog::level::warn;
        if (lvl == "error") return spdlog::level::err;
        if (lvl == "critical") return spdlog::level::critical;
        return spdlog::level::info;
    }

public:
    static DevLogger& instance()
    {
        static DevLogger inst;
        return inst;
    }

    std::shared_ptr<spdlog::logger>& get()
    {
        return logger;
    }

    void setLogger(std::shared_ptr<spdlog::logger> custom) {
        logger = custom;
    }
};

#define mylog DevLogger::instance().get()