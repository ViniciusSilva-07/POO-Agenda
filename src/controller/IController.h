#pragma once
#include <memory>
#include <string>
#include "Calendar.h"
#include "IView.h"

class IController {
public:
    virtual ~IController() = default;
    virtual void createEvent(const std::string& title,
                             const std::string& start,
                             const std::string& end,
                             const std::vector<std::string>& tags) = 0;
    virtual void listEvents() const = 0;
    virtual void save(const std::string& filename) const = 0;
    virtual void load(const std::string& filename) = 0;
};