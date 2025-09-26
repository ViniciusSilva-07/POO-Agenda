#pragma once
#include <memory>
#include "Calendar.h"
#include "IView.h"

// Interface do controlador que conecta modelo (Calendar/Event) e view
class IController {
public:
    virtual ~IController() = default;

    virtual void createEvent(const std::string& title) = 0;
    virtual void listEvents() const = 0;
};