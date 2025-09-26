#pragma once
#include <vector>
#include <memory>
#include "Event.h"
#include "User.h"

// Classe que representa a Agenda (contém eventos e pertence a um usuário)
class Calendar {
private:
    std::vector<std::shared_ptr<Event>> events; // composição: Calendar contém Events
    User owner;                                 // dono da agenda

public:
    Calendar(const User& u);

    void addEvent(std::shared_ptr<Event> e);
    void removeEvent(const Event& e);

    // Busca por data, título ou tag
    std::vector<std::shared_ptr<Event>> findEventsByTitle(const std::string& title) const;
    std::vector<std::shared_ptr<Event>> findEventsByTag(const std::string& tag) const;

    const std::vector<std::shared_ptr<Event>>& getAllEvents() const;
    const User& getOwner() const;
};
