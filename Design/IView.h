#pragma once
#include <string>

// Interface da camada de apresentação (futuramente GUI ou CLI)
class IView {
public:
    virtual ~IView() = default;
    virtual void displayMessage(const std::string& msg) = 0;
};