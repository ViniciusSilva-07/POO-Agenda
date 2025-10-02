#include "User.h"

User::User(const std::string& n, const std::string& e) : name(n), email(e) {}

std::string User::getName() const { return name; }
void User::setName(const std::string& n) { name = n; }

std::string User::getEmail() const { return email; }
void User::setEmail(const std::string& e) { email = e; }