#include "logger.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

std::mutex Logger::mutex_;

void Logger::info(const std::string& msg)  { log(LogLevel::INFO, msg); }
void Logger::warn(const std::string& msg)  { log(LogLevel::WARN, msg); }
void Logger::error(const std::string& msg) { log(LogLevel::ERR, msg); }
void Logger::debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }

void Logger::request(const std::string& method, const std::string& path,
                     int status, double ms) {
    std::ostringstream oss;
    oss << method << " " << path
        << " -> " << status
        << " (" << std::fixed << std::setprecision(1) << ms << "ms)";
    log(LogLevel::INFO, oss.str());
}

void Logger::log(LogLevel level, const std::string& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << level_color(level)
              << "[" << timestamp() << "] "
              << "[" << level_str(level) << "] "
              << msg
              << "\033[0m"
              << std::endl;
}

std::string Logger::timestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

std::string Logger::level_str(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERR:   return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
    }
    return "?????";
}

std::string Logger::level_color(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "\033[32m";
        case LogLevel::WARN:  return "\033[33m";
        case LogLevel::ERR:   return "\033[31m";
        case LogLevel::DEBUG: return "\033[36m";
    }
    return "";
}
