#pragma once
#include <string>
#include <vector>
#include <sstream>

template<typename T>
std::string join(const std::vector<T>& elems, const std::string& sep) {
    std::ostringstream ss;
    for (size_t i = 0; i < elems.size(); ++i) {
        ss << elems[i];
        if (i + 1 < elems.size()) ss << sep;
    }
    return ss.str();
}