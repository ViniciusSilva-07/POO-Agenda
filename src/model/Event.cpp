#include "Event.h"
#include <algorithm>

Event::Event(const std::string& t,
             const std::chrono::system_clock::time_point& start,
             const std::chrono::system_clock::time_point& end)
    : title(t), startTime(start), endTime(end) {}

std::string Event::getTitle() const { return title; }
void Event::setTitle(const std::string& t) { title = t; }

std::string Event::getDescription() const { return description; }
void Event::setDescription(const std::string& d) { description = d; }

std::chrono::system_clock::time_point Event::getStartTime() const { return startTime; }
void Event::setStartTime(const std::chrono::system_clock::time_point& start) { startTime = start; }

std::chrono::system_clock::time_point Event::getEndTime() const { return endTime; }
void Event::setEndTime(const std::chrono::system_clock::time_point& end) { endTime = end; }

void Event::addTag(const std::string& tag) {
    if (std::find(tags.begin(), tags.end(), tag) == tags.end()) tags.push_back(tag);
}

const std::vector<std::string>& Event::getTags() const { return tags; }

std::shared_ptr<RecurrenceRule> Event::getRecurrence() const { return recurrence; }
void Event::setRecurrence(std::shared_ptr<RecurrenceRule> r) { recurrence = r; }

bool Event::operator<(const Event& other) const {
    return startTime < other.startTime;
}

bool Event::operator==(const Event& other) const {
    return title == other.title && startTime == other.startTime && endTime == other.endTime;
}
