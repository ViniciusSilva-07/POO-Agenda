#pragma once
#include "RecurrenceRule.h"
#include <string>

class RecurrenceRuleDaily : public RecurrenceRule {
private:
    int intervalDays; // every n days
public:
    RecurrenceRuleDaily(int interval = 1);
    std::string getDescription() const override;
    int getInterval() const;
};