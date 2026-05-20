#pragma once

#include <string>
#include "DatabaseManager.h"

class AuthService {
private:
    DatabaseManager& db;

public:
    AuthService(DatabaseManager& db);

    bool registerUser(const std::string& username, const std::string& password, const std::string& role, std::string& error);
    bool login(const std::string& username, const std::string& password, std::string& token, std::string& error);
};
