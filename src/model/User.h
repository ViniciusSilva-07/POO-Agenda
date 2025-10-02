#pragma once
#include <string>

class User {
private:
    std::string name;
    std::string email;
public:
    User() = default;
    User(const std::string& n, const std::string& e);

    std::string getName() const;
    void setName(const std::string& n);

    std::string getEmail() const;
    void setEmail(const std::string& e);
};