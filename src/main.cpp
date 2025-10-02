
#include <iostream>
#include <memory>
#include <sstream>
#include <limits>
#include "model/User.h"
#include "model/Calendar.h"
#include "view/ConsoleView.h"
#include "controller/Controller.h"
#include "utils/UtilTemplates.h"

int main() {
    std::cout << "=== Agenda POO - Versao 2 (CLI) ===" << std::endl;
    std::string ownerName, ownerEmail;
    std::cout << "Nome do dono da agenda: ";
    std::getline(std::cin, ownerName);
    std::cout << "Email do dono: ";
    std::getline(std::cin, ownerEmail);

    User owner(ownerName, ownerEmail);
    auto calendar = std::make_shared<Calendar>(owner);
    auto view = std::make_shared<ConsoleView>();
    Controller controller(calendar, view);

    while (true) {
        std::cout << "\nComandos:\n"
                  << "1 - criar evento\n"
                  << "2 - listar eventos\n"
                  << "3 - salvar\n"
                  << "4 - carregar\n"
                  << "0 - sair\n"
                  << "Escolha: ";
        int cmd;
        if (!(std::cin >> cmd)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (cmd == 0) break;
        if (cmd == 1) {
            std::string title, start, end, tagsline;
            std::cout << "Titulo: "; std::getline(std::cin, title);
            std::cout << "Inicio (YYYY-MM-DD HH:MM): "; std::getline(std::cin, start);
            std::cout << "Fim (YYYY-MM-DD HH:MM): "; std::getline(std::cin, end);
            std::cout << "Tags (separadas por virgula, opcional): "; std::getline(std::cin, tagsline);
            std::vector<std::string> tags;
            if (!tagsline.empty()) {
                std::istringstream ss(tagsline);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    // trim
                    while (!token.empty() && isspace(token.front())) token.erase(token.begin());
                    while (!token.empty() && isspace(token.back())) token.pop_back();
                    if (!token.empty()) tags.push_back(token);
                }
            }
            controller.createEvent(title, start, end, tags);
        } else if (cmd == 2) {
            controller.listEvents();
        } else if (cmd == 3) {
            std::string filename;
            std::cout << "Nome do arquivo para salvar: "; std::getline(std::cin, filename);
            controller.save(filename);
        } else if (cmd == 4) {
            std::string filename;
            std::cout << "Nome do arquivo para carregar: "; std::getline(std::cin, filename);
            controller.load(filename);
        } else {
            std::cout << "Comando inválido\n";
        }
    }

    std::cout << "Saindo...\n";
    return 0;
}