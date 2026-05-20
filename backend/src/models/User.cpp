#include "User.h"

User::User() : id(0), username(""), password(""), role("") {}

User::User(int id, const std::string& username, const std::string& password, const std::string& role)
    : id(id), username(username), password(password), role(role) {}

int User::getId() const {
    return id;
}

const std::string& User::getUsername() const {
    return username;
}

const std::string& User::getPassword() const {
    return password;
}

const std::string& User::getRole() const {
    return role;
}
