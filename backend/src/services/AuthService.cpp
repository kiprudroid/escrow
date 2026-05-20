#include "AuthService.h"

AuthService::AuthService(DatabaseManager& db) : db(db) {}

bool AuthService::registerUser(const std::string& username, const std::string& password, const std::string& role, std::string& error) {
    return db.createUser(username, password, role, error);
}

bool AuthService::login(const std::string& username, const std::string& password, std::string& token, std::string& error) {
    User user;
    if (!db.validateUser(username, password, user, error)) {
        return false;
    }

    token = "token_" + user.getUsername();
    return true;
}
