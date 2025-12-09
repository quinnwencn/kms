//
// Created by Quinn Wen on 2025/12/1.
//

#pragma once

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <fmt/core.h>
#include <string>
#include <filesystem>

#include "kms/config.h"

class Logger {
public:
    Logger() = default;
    ~Logger() = default;

    static void Init(const kms::LogConfig& config) {
        namespace logging = boost::log;

        std::string logfile = GetTimestampFilename(config.LogDir());

        logging::add_file_log(
            logging::keywords::file_name = logfile,
            logging::keywords::rotation_size = 100 * 1024 * 1024, // 100 MB
            logging::keywords::format = "[%TimeStamp%] [%Severity%] %Message%"
        );

        logging::core::get()->set_filter(logging::trivial::severity >= \
            static_cast<boost::log::trivial::severity_level>(config.LogLevel()));
        logging::add_common_attributes();
    }

    template <typename... Args>
    static void Trace(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::trace, fmtStr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Debug(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::debug, fmtStr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Info(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::info, fmtStr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Warn(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::warning, fmtStr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Error(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::error, fmtStr, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Fatal(const char* fmtStr, Args&&... args) {
        log(boost::log::trivial::fatal, fmtStr, std::forward<Args>(args)...);
    }

private:
    static boost::log::sources::severity_logger<
    boost::log::trivial::severity_level>& Get() {
        static boost::log::sources::severity_logger<
            boost::log::trivial::severity_level> lg;
        return lg;
    }

    template <typename... Args>
    static void log(boost::log::trivial::severity_level logLevel, const char* fmt_str, Args&&... args) {
        std::string msg = fmt::format(fmt_str, std::forward<Args>(args)...);
        BOOST_LOG_SEV(Get(), logLevel) << "[" << __FILE__ << ":" << __LINE__ << "] "<< msg;
    }

    static std::string GetTimestampFilename(const std::filesystem::path& folder, const std::string& prefix = "kms") {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        std::tm tm_now;
#ifdef _WIN32
        localtime_s(&tm_now, &time_t_now);
#else
        localtime_r(&time_t_now, &tm_now);
#endif
        return fmt::format("{}/{}-{}-{} {}:{}:{}.log",
            folder.string(),
            prefix,
            tm_now.tm_year + 1900,
            tm_now.tm_mon + 1,
            tm_now.tm_mday,
            tm_now.tm_hour,
            tm_now.tm_min,
            tm_now.tm_sec);
    }

};
