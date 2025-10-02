#include "RecurrenceRuleDaily.h"
#include <sstream>

RecurrenceRuleDaily::RecurrenceRuleDaily(int interval) : intervalDays(interval) {}

std::string RecurrenceRuleDaily::getDescription() const {
    std::ostringstream ss;
    ss << "Daily every " << intervalDays << " day(s)";
    return ss.str();
}

int RecurrenceRuleDaily::getInterval() const { return intervalDays; }