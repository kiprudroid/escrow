#pragma once

#include <string>

class User {
private:
    int id;
    std::string username;
    std::string password;
    std::string role;

public:
    User();
    User(int id, const std::string& username, const std::string& password, const std::string& role);

    int getId() const;
    const std::string& getUsername() const;
    const std::string& getPassword() const;
    const std::string& getRole() const;
};
