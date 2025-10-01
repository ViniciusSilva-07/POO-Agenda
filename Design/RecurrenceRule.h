#pragma once
#include <string>

// Classe abstrata para representar regra de recorrência de eventos
class RecurrenceRule {
public:
    virtual ~RecurrenceRule() = default;

    // Método polimórfico: retorna descrição da regra
    virtual std::string getDescription() const = 0;

    // Poderíamos futuramente implementar classes concretas:
    // DailyRecurrence, WeeklyRecurrence, MonthlyRecurrence...
};
