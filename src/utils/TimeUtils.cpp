#include "TimeUtils.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstring>

using namespace std::chrono;

std::chrono::system_clock::time_point TimeUtils::fromString(const std::string& s) {
    std::tm tm = {};
    std::istringstream ss(s);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    if (ss.fail()) {
        throw std::runtime_error("Formato de data invalido: esperado YYYY-MM-DD HH:MM");
    }
    std::time_t time = std::mktime(&tm);
    return system_clock::from_time_t(time);
}

std::string TimeUtils::toString(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = system_clock::to_time_t(tp);
    std::tm tm;
    // Compatibilidade multiplataforma: MSVC usa localtime_s, POSIX usa localtime_r, MinGW/Windows usa localtime
#if defined(_MSC_VER)
    localtime_s(&tm, &t);
#elif defined(__unix__) || defined(__APPLE__)
    localtime_r(&t, &tm);
#else
    std::tm* tmp = std::localtime(&t);
    if (tmp)
        tm = *tmp;
    else
        memset(&tm, 0, sizeof(std::tm));
#endif
    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return ss.str();
}