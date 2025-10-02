#include "Calendar.h"
#include <stdexcept>

Calendar::Calendar(const User& u) : owner(u) {}

void Calendar::addEvent(std::shared_ptr<Event> e) {
    if (!e) throw std::invalid_argument("Event null");
    events.push_back(e);
    std::sort(events.begin(), events.end(), [](const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b){
        return *a < *b;
    });
}

void Calendar::removeEvent(const Event& e) {
    auto it = std::remove_if(events.begin(), events.end(), [&](const std::shared_ptr<Event>& ev){
        return *ev == e;
    });
    if (it != events.end()) events.erase(it, events.end());
}

std::vector<std::shared_ptr<Event>> Calendar::findEventsByTitle(const std::string& title) const {
    std::vector<std::shared_ptr<Event>> res;
    for (auto &e : events) if (e->getTitle().find(title) != std::string::npos) res.push_back(e);
    return res;
}

std::vector<std::shared_ptr<Event>> Calendar::findEventsByTag(const std::string& tag) const {
    std::vector<std::shared_ptr<Event>> res;
    for (auto &e : events) {
        for (auto &t : e->getTags()) if (t == tag) { res.push_back(e); break; }
    }
    return res;
}

const std::vector<std::shared_ptr<Event>>& Calendar::getAllEvents() const { return events; }
const User& Calendar::getOwner() const { return owner; }
