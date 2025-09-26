#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Event.h"

// Interface para persistência (poderá ter implementações em JSON, SQLite, etc.)
class IPersistence {
public:
    virtual ~IPersistence() = default;

    virtual void save(const std::vector<std::shared_ptr<Event>>& events,
                      const std::string& filename) = 0;
    virtual std::vector<std::shared_ptr<Event>> load(const std::string& filename) = 0;
};