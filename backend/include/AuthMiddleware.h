#pragma once

#include <string>
#include "crow.h"
#include "DatabaseManager.h"

class AuthMiddleware {
public:
    struct context {
        bool authenticated = false;
        int userId = 0;
        std::string username;
        std::string role;
    };

private:
    static DatabaseManager* db;

public:
    AuthMiddleware() = default;

    static void setDatabaseManager(DatabaseManager* manager);

    void before_handle(crow::request& req, crow::response& res, context& ctx);
    void after_handle(crow::request& req, crow::response& res, context& ctx);
};
