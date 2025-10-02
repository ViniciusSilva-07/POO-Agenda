#pragma once
#include "IPersistence.h"
#include <string>

class JSONPersistence : public IPersistence {
public:
    void save(const std::vector<std::shared_ptr<Event>>& events, const std::string& filename) override;
    std::vector<std::shared_ptr<Event>> load(const std::string& filename) override;
};