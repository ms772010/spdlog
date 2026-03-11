#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

namespace spdlog {

enum class BusinessType {
    ScreenRecord,   
    DesktopOpen,    
    KeyboardRecord, 
    SoundRecord     
};

class BusinessLogger {
public:
    static constexpr size_t MaxFileSize = 30 * 1024 * 1024; 
    static constexpr size_t MaxFiles = 3;                   

    BusinessLogger(const std::string& log_dir);
    ~BusinessLogger() = default;

    void log(BusinessType type, const std::string& message);
    void log(BusinessType type, level::level_enum level, const std::string& message);

    template<typename... Args>
    void log(BusinessType type, level::level_enum level, const char* fmt, Args&&... args) {
        auto logger = get_logger(type);
        if (logger) {
            logger->log(level, fmt, std::forward<Args>(args)...);
        }
    }

    template<typename... Args>
    void trace(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::trace, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void debug(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warn(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::err, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void critical(BusinessType type, const char* fmt, Args&&... args) {
        log(type, level::level_enum::critical, fmt, std::forward<Args>(args)...);
    }

    void flush(BusinessType type);
    void flush_all();

private:
    std::shared_ptr<logger> get_logger(BusinessType type);
    std::string get_business_name(BusinessType type) const;
    std::string get_log_filename(BusinessType type) const;

    std::unordered_map<BusinessType, std::shared_ptr<logger>> loggers_;
    std::mutex mutex_;
    std::string log_dir_;
};

inline BusinessLogger::BusinessLogger(const std::string& log_dir)
    : log_dir_(log_dir) {
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e [%t] %^%l%$ : %v");
}

inline std::string BusinessLogger::get_business_name(BusinessType type) const {
    switch (type) {
        case BusinessType::ScreenRecord:
            return "screen_record";
        case BusinessType::DesktopOpen:
            return "desktop_open";
        case BusinessType::KeyboardRecord:
            return "keyboard_record";
        case BusinessType::SoundRecord:
            return "sound_record";
        default:
            return "unknown";
    }
}

inline std::string BusinessLogger::get_log_filename(BusinessType type) const {
    return log_dir_ + "/" + get_business_name(type) + ".log";
}

inline std::shared_ptr<logger> BusinessLogger::get_logger(BusinessType type) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = loggers_.find(type);
    if (it != loggers_.end()) {
        return it->second;
    }

    std::string filename = get_log_filename(type);
    auto rotating_sink = std::make_shared<sinks::rotating_file_sink_mt>(
        filename,
        MaxFileSize,
        MaxFiles,
        true
    );

    std::string logger_name = "business_" + get_business_name(type);
    auto new_logger = std::make_shared<spdlog::logger>(logger_name, std::move(rotating_sink));
    
    new_logger->set_level(level::level_enum::debug);
    new_logger->flush_on(level::level_enum::warn);

    loggers_[type] = new_logger;
    spdlog::register_logger(new_logger);

    return new_logger;
}

inline void BusinessLogger::log(BusinessType type, const std::string& message) {
    auto logger = get_logger(type);
    if (logger) {
        logger->info(message);
    }
}

inline void BusinessLogger::log(BusinessType type, level::level_enum level, const std::string& message) {
    auto logger = get_logger(type);
    if (logger) {
        logger->log(level, message);
    }
}

inline void BusinessLogger::flush(BusinessType type) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = loggers_.find(type);
    if (it != loggers_.end()) {
        it->second->flush();
    }
}

inline void BusinessLogger::flush_all() {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& pair : loggers_) {
        pair.second->flush();
    }
}

} // namespace spdlog
