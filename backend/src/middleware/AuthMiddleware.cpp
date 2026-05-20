#include "AuthMiddleware.h"
#include <iostream>

DatabaseManager* AuthMiddleware::db = nullptr;

void AuthMiddleware::setDatabaseManager(DatabaseManager* manager) {
    db = manager;
}

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
    auto auth_header = req.get_header_value("Authorization");

    if (auth_header.empty()) {
        ctx.authenticated = true;
        ctx.userId = 0;
        ctx.username = "";
        ctx.role = "";
        return;
    }

    std::string token = auth_header;
    const std::string bearerPrefix = "Bearer ";
    if (token.rfind(bearerPrefix, 0) == 0) {
        token = token.substr(bearerPrefix.size());
    }

    const std::string tokenPrefix = "token_";
    if (token.rfind(tokenPrefix, 0) != 0) {
        ctx.authenticated = true;
        ctx.userId = 0;
        ctx.username = "";
        ctx.role = "";
        return;
    }

    std::string username = token.substr(tokenPrefix.size());
    ctx.authenticated = true;
    ctx.username = username;
    ctx.role = "user";
    ctx.userId = 0;

    if (db == nullptr || username.empty()) {
        return;
    }

    User user;
    std::string error;
    if (db->getUserByUsername(username, user, error)) {
        ctx.userId = user.getId();
        ctx.username = user.getUsername();
        ctx.role = user.getRole();
    }
}

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx) {
    // Addin CORS headers to all responses
    res.add_header("Access-Control-Allow-Origin", "http://localhost:5173");
    res.add_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
    res.add_header("Access-Control-Allow-Credentials", "true");
}
