#pragma once
#include "IView.h"
#include <string>

// Classe que no futuro será a implementação da interface gráfica usando Qt
class GuiView : public IView {
public:
    GuiView() = default;
    virtual ~GuiView() = default;

    // Exibe uma mensagem na interface gráfica
    void displayMessage(const std::string& msg) override;
};