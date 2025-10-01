#pragma once
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include "RecurrenceRule.h"

// Representa um evento na agenda
class Event {
private:
    std::string title;                 // título do evento
    std::string description;           // descrição opcional
    std::chrono::system_clock::time_point startTime; // horário de início
    std::chrono::system_clock::time_point endTime;   // horário de término
    std::vector<std::string> tags;     // categorias/tags do evento
    std::shared_ptr<RecurrenceRule> recurrence; // regra de recorrência (composição)

public:
    Event(const std::string& t, const std::chrono::system_clock::time_point& start,
          const std::chrono::system_clock::time_point& end);

    // Getters e setters com encapsulamento
    std::string getTitle() const;
    void setTitle(const std::string& t);

    std::chrono::system_clock::time_point getStartTime() const;
    void setStartTime(const std::chrono::system_clock::time_point& start);

    std::chrono::system_clock::time_point getEndTime() const;
    void setEndTime(const std::chrono::system_clock::time_point& end);

    void addTag(const std::string& tag);
    const std::vector<std::string>& getTags() const;

    std::shared_ptr<RecurrenceRule> getRecurrence() const;
    void setRecurrence(std::shared_ptr<RecurrenceRule> r);

    // Sobrecarga de operadores para ordenação e comparação
    bool operator<(const Event& other) const;
    bool operator==(const Event& other) const;
};
