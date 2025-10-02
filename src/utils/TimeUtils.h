#pragma once
#include <string>
#include <chrono>

namespace TimeUtils {
    // Parse "YYYY-MM-DD HH:MM" into time_point
    std::chrono::system_clock::time_point fromString(const std::string& s);
    // Format time_point into "YYYY-MM-DD HH:MM"
    std::string toString(const std::chrono::system_clock::time_point& tp);
}