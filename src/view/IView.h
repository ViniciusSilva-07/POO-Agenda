#pragma once
#include <string>

class IView {
public:
    virtual ~IView() = default;
    virtual void displayMessage(const std::string& msg) = 0;
};