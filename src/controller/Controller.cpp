#include "Controller.h"
#include "TimeUtils.h"
#include <iostream>
#include <sstream>
#include <map>

// helper: convert YYYY-MM-DD to DD-MM-YYYY for display
static std::string ymdToDmy(const std::string &ymd) {
    if (ymd.size() < 10) return ymd;
    return ymd.substr(8,2) + std::string("-") + ymd.substr(5,2) + std::string("-") + ymd.substr(0,4);
}

Controller::Controller(std::shared_ptr<Calendar> cal, std::shared_ptr<IView> v)
    : calendar(cal), view(v), persistence(std::make_shared<JSONPersistence>()) {}

void Controller::createEvent(const std::string& title,
                             const std::string& start,
                             const std::string& end,
                             const std::vector<std::string>& tags) {
    try {
        auto st = TimeUtils::fromString(start);
        auto en = TimeUtils::fromString(end);
    if (en < st) throw std::runtime_error("Horario de termino anterior ao inicio");
        auto ev = std::make_shared<Event>(title, st, en);
        for (auto &t : tags) ev->addTag(t);
    calendar->addEvent(ev);
    } catch (const std::exception& ex) {
        view->displayMessage(std::string("Erro ao criar evento: ") + ex.what());
    }
}

void Controller::listEvents() const {
    const auto &events = calendar->getAllEvents();
    if (events.empty()) {
        return;
    }
    // listing behavior kept silent in GUI context
}

std::vector<std::string> Controller::getEventStrings() const {
    std::vector<std::string> out;
    const auto &events = calendar->getAllEvents();
    out.reserve(events.size());
    for (size_t i = 0; i < events.size(); ++i) {
        auto &e = events[i];
        std::ostringstream ss;
        // Format: N) Title | Dia(s) | Horario | tags
    std::string startY = TimeUtils::toString(e->getStartTime()).substr(0,10);
    std::string endY = TimeUtils::toString(e->getEndTime()).substr(0,10);
    std::string startY_dmy = ymdToDmy(startY);
    std::string endY_dmy = ymdToDmy(endY);
    std::string startT = TimeUtils::toString(e->getStartTime()).substr(11);
    std::string endT = TimeUtils::toString(e->getEndTime()).substr(11);
    ss << i+1 << ") " << e->getTitle() << " | ";
    if (startY == endY) ss << startY_dmy; else ss << startY_dmy << " -> " << endY_dmy;
    ss << " | " << startT << " -> " << endT;
        if (!e->getTags().empty()) {
            ss << " | tags: ";
            for (size_t j=0;j<e->getTags().size();++j) {
                ss << e->getTags()[j];
                if (j+1 < e->getTags().size()) ss << ",";
            }
        }
        if (e->getRecurrence()) ss << " | rec: " << e->getRecurrence()->getDescription();
        out.push_back(ss.str());
    }
    return out;
}

// Group events by month and return a human readable list: "YYYY-MM: N events" then indented events
std::vector<std::string> Controller::getEventStringsByMonth() const {
    std::map<std::string, std::vector<std::string>> byMonth;
    const auto &events = calendar->getAllEvents();
    for (size_t i = 0; i < events.size(); ++i) {
        auto &e = events[i];
        auto sdate = TimeUtils::toString(e->getStartTime()); // YYYY-MM-DD HH:MM
        auto edate = TimeUtils::toString(e->getEndTime());
        std::string sy = sdate.substr(0,7); // YYYY-MM
    std::string startY = sdate.substr(0,10);
    std::string endY = edate.substr(0,10);
    std::string startY_dmy = ymdToDmy(startY);
    std::string endY_dmy = ymdToDmy(endY);
    std::string startT = sdate.substr(11);
    std::string endT = edate.substr(11);
    std::ostringstream ss;
    ss << "  - " << e->getTitle() << " | ";
    if (startY == endY) ss << startY_dmy; else ss << startY_dmy << " -> " << endY_dmy;
    ss << " | " << startT << " -> " << endT;
        if (!e->getTags().empty()) {
            ss << " | tags: ";
            for (size_t j=0;j<e->getTags().size();++j) {
                ss << e->getTags()[j]; if (j+1 < e->getTags().size()) ss << ",";
            }
        }
        byMonth[sy].push_back(ss.str());
    }
    std::vector<std::string> out;
    for (auto &p : byMonth) {
        // p.first is YYYY-MM; convert MM to month name (no accents)
        std::string ym = p.first; // YYYY-MM
        std::string year = ym.substr(0,4);
        std::string mm = ym.substr(5,2);
        static const char* months[] = {"Janeiro","Fevereiro","Marco","Abril","Maio","Junho","Julho","Agosto","Setembro","Outubro","Novembro","Dezembro"};
        int mi = 1;
        try { mi = std::stoi(mm); } catch(...) { mi = 1; }
        if (mi < 1 || mi > 12) mi = 1;
        std::ostringstream hdr; hdr << year << "-" << months[mi-1] << " (" << p.second.size() << " eventos)";
        out.push_back(hdr.str());
        for (auto &line : p.second) out.push_back(line);
        // blank separator between months
        out.push_back("");
    }
    // remove last blank separator if present
    if (!out.empty() && out.back().empty()) out.pop_back();
    return out;
}

void Controller::save(const std::string& filename) const {
    try {
    persistence->save(calendar->getAllEvents(), filename);
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

static std::string dateToYMD(const std::chrono::system_clock::time_point& tp) {
    return TimeUtils::toString(tp).substr(0,10); // YYYY-MM-DD
}

std::vector<std::string> Controller::getEventStringsForDay(const std::string& ymd) const {
    std::vector<std::string> out;
    const auto &events = calendar->getAllEvents();
    for (auto &e : events) {
    std::string startY = dateToYMD(e->getStartTime());
    std::string endY = dateToYMD(e->getEndTime());
    std::string startY_dmy = ymdToDmy(startY);
    std::string endY_dmy = ymdToDmy(endY);
        // include event if the requested day is between start and end (inclusive)
        if (startY <= ymd && ymd <= endY) {
            std::ostringstream ss;
            // Format: Title | Dia(s) | Horario | tags
            std::string startT = TimeUtils::toString(e->getStartTime()).substr(11);
            std::string endT = TimeUtils::toString(e->getEndTime()).substr(11);
            ss << e->getTitle() << " | ";
            if (startY == endY) ss << startY_dmy; else ss << startY_dmy << " -> " << endY_dmy;
            ss << " | " << startT << " -> " << endT;
            if (!e->getTags().empty()) {
                ss << " | tags: ";
                for (size_t j=0;j<e->getTags().size();++j) {
                    ss << e->getTags()[j]; if (j+1 < e->getTags().size()) ss << ",";
                }
            }
            out.push_back(ss.str());
        }
    }
    return out;
}