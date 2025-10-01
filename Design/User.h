#pragma once
#include <string>

// Representa um usuário da agenda
class User {
private:
    std::string name;
    std::string email;

public:
    User(const std::string& n, const std::string& e);

    std::string getName() const;
    void setName(const std::string& n);

    std::string getEmail() const;
    void setEmail(const std::string& e);
};