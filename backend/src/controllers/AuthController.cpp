#include "crow.h"
#include "AuthService.h"
#include "AuthMiddleware.h"

crow::response jsonMessage(int code, const std::string& message);

void registerAuthRoutes(crow::App<AuthMiddleware>& app, AuthService& authService) {

    //POST /api/auth/register
    CROW_ROUTE(app, "/api/auth/register").methods("POST"_method)
    ([&authService](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return jsonMessage(400, "Invalid JSON payload");
        }

        if (!body.has("username") || !body.has("password") || !body.has("role")) {
            return jsonMessage(400, "username, password, and role are required");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();
        std::string role = body["role"].s();

        if (username.empty() || password.empty() || role.empty()) {
            return jsonMessage(400, "username, password, and role cannot be empty");
        }

        std::string error;
        if (!authService.registerUser(username, password, role, error)) {
            return jsonMessage(400, error);
        }

        return jsonMessage(201, "User registered successfully");
    });

    // POST /api/auth/login

    CROW_ROUTE(app, "/api/auth/login").methods("POST"_method)
    ([&authService](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return jsonMessage(400, "Invalid JSON payload");
        }

        if (!body.has("username") || !body.has("password")) {
            return jsonMessage(400, "username and password are required");
        }

        std::string username = body["username"].s();
        std::string password = body["password"].s();

        std::string token;
        std::string error;
        if (!authService.login(username, password, token, error)) {
            return jsonMessage(401, error);
        }

        return crow::response(200, token);
    });

    // GET /api/auth/me
    // getting the data associated with the current User
    CROW_ROUTE(app, "/api/auth/me").methods("GET"_method)
    ([&app](const crow::request& req) {
        const auto& ctx = app.get_context<AuthMiddleware>(req);
        if (!ctx.authenticated) {
            return jsonMessage(401, "Unauthorized");
        }

        crow::json::wvalue data;
        data["id"] = ctx.userId;
        data["username"] = ctx.username;
        data["role"] = ctx.role;

        return crow::response(200, data.dump());
    });
}
