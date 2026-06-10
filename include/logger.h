#pragma once

#include <string>
#include <mutex>

enum class LogLevel { INFO, WARN, ERR, DEBUG };

class Logger {
public:
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
    static void debug(const std::string& msg);

    static void request(const std::string& method, const std::string& path,
                        int status, double ms);

private:
    static void log(LogLevel level, const std::string& msg);
    static std::mutex mutex_;
    static std::string timestamp();
    static std::string level_str(LogLevel level);
    static std::string level_color(LogLevel level);
};
