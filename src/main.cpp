
#include <iostream>
#include <memory>
#include <sstream>
#include <limits>
#include "model/User.h"
#include "model/Calendar.h"
#include "view/GUIView.h"
#include "controller/Controller.h"
#include "utils/UtilTemplates.h"

int main() {
    // Inicializa a GUI sem solicitar nome/email
    User owner("", "");
    auto calendar = std::make_shared<Calendar>(owner);
    // Inicializa sempre a GUI
    auto guiv = std::make_shared<GUIView>();
    std::shared_ptr<Controller> controller = std::make_shared<Controller>(calendar, guiv);
    guiv->setController(controller);
        // Executa a GUI (abre direto sem cadastro)
        guiv->run();
    return 0;
}