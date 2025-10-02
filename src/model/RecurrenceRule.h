#pragma once
#include <string>

class RecurrenceRule {
public:
    virtual ~RecurrenceRule() = default;
    virtual std::string getDescription() const = 0;
};