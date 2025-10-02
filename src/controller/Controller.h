#pragma once
#include "IController.h"
#include "JSONPersistence.h"
#include <memory>

class Controller : public IController {
private:
    std::shared_ptr<Calendar> calendar;
    std::shared_ptr<IView> view;
    std::shared_ptr<IPersistence> persistence;

public:
    Controller(std::shared_ptr<Calendar> cal, std::shared_ptr<IView> v);

    void createEvent(const std::string& title,
                     const std::string& start,
                     const std::string& end,
                     const std::vector<std::string>& tags) override;

    void listEvents() const override;
    void save(const std::string& filename) const override;
    void load(const std::string& filename) override;
};