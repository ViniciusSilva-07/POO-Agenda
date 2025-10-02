#pragma once
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "RecurrenceRule.h"

class Event {
private:
    std::string title;
    std::string description;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::vector<std::string> tags;
    std::shared_ptr<RecurrenceRule> recurrence;

public:
    Event(const std::string& t,
          const std::chrono::system_clock::time_point& start,
          const std::chrono::system_clock::time_point& end);

    // getters / setters
    std::string getTitle() const;
    void setTitle(const std::string& t);

    std::string getDescription() const;
    void setDescription(const std::string& d);

    std::chrono::system_clock::time_point getStartTime() const;
    void setStartTime(const std::chrono::system_clock::time_point& start);

    std::chrono::system_clock::time_point getEndTime() const;
    void setEndTime(const std::chrono::system_clock::time_point& end);

    void addTag(const std::string& tag);
    const std::vector<std::string>& getTags() const;

    std::shared_ptr<RecurrenceRule> getRecurrence() const;
    void setRecurrence(std::shared_ptr<RecurrenceRule> r);

    // operators
    bool operator<(const Event& other) const;
    bool operator==(const Event& other) const;
};

