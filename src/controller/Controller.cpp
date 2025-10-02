#include "Controller.h"
#include "TimeUtils.h"
#include <iostream>
#include <sstream>

Controller::Controller(std::shared_ptr<Calendar> cal, std::shared_ptr<IView> v)
    : calendar(cal), view(v), persistence(std::make_shared<JSONPersistence>()) {}

void Controller::createEvent(const std::string& title,
                             const std::string& start,
                             const std::string& end,
                             const std::vector<std::string>& tags) {
    try {
        auto st = TimeUtils::fromString(start);
        auto en = TimeUtils::fromString(end);
        if (en < st) throw std::runtime_error("Horário de término anterior ao início");
        auto ev = std::make_shared<Event>(title, st, en);
        for (auto &t : tags) ev->addTag(t);
        calendar->addEvent(ev);
        view->displayMessage("Evento criado: " + title);
    } catch (const std::exception& ex) {
        view->displayMessage(std::string("Erro ao criar evento: ") + ex.what());
    }
}

void Controller::listEvents() const {
    const auto &events = calendar->getAllEvents();
    if (events.empty()) {
        view->displayMessage("Nenhum evento.");
        return;
    }
    for (size_t i = 0; i < events.size(); ++i) {
        auto &e = events[i];
        std::ostringstream ss;
        ss << i+1 << ") " << e->getTitle() << " | " << TimeUtils::toString(e->getStartTime())
           << " -> " << TimeUtils::toString(e->getEndTime());
        if (!e->getTags().empty()) {
            ss << " | tags: ";
            for (size_t j=0;j<e->getTags().size();++j) {
                ss << e->getTags()[j];
                if (j+1 < e->getTags().size()) ss << ",";
            }
        }
        if (e->getRecurrence()) ss << " | rec: " << e->getRecurrence()->getDescription();
        view->displayMessage(ss.str());
    }
}

void Controller::save(const std::string& filename) const {
    try {
        persistence->save(calendar->getAllEvents(), filename);
        view->displayMessage("Salvo em " + filename);
    } catch (const std::exception &ex) {
        view->displayMessage(std::string("Erro ao salvar: ") + ex.what());
    }
}

void Controller::load(const std::string& filename) {
    try {
        auto events = persistence->load(filename);
        // replace calendar events with loaded ones
        // naive: create new calendar with same owner
        User owner = calendar->getOwner();
        calendar = std::make_shared<Calendar>(owner);
        for (auto &e : events) calendar->addEvent(e);
        view->displayMessage("Carregado de " + filename);
    } catch (const std::exception &ex) {
        view->displayMessage(std::string("Erro ao carregar: ") + ex.what());
    }
}