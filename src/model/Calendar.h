#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "Event.h"
#include "User.h"

class Calendar {
private:
    std::vector<std::shared_ptr<Event>> events;
    User owner;

public:
    Calendar(const User& u);

    void addEvent(std::shared_ptr<Event> e);
    void removeEvent(const Event& e);

    std::vector<std::shared_ptr<Event>> findEventsByTitle(const std::string& title) const;
    std::vector<std::shared_ptr<Event>> findEventsByTag(const std::string& tag) const;

    const std::vector<std::shared_ptr<Event>>& getAllEvents() const;
    const User& getOwner() const;
};