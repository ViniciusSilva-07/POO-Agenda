#pragma once
#include "IView.h"
#include <iostream>
#include <string>

class ConsoleView : public IView {
public:
    ConsoleView() = default;
    void displayMessage(const std::string& msg) override {
        std::cout << msg << std::endl;
    }
};