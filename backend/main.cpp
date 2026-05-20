#include "crow.h"
#include <iostream>
#include "DatabaseManager.h"
#include "AuthService.h"
#include "EscrowService.h"
#include "AuthMiddleware.h"

void setupRoutes(crow::App<AuthMiddleware>& app, AuthService& authService, EscrowService& escrowService);

int main() {
    DatabaseManager db("escrow.db");
    std::string error;
    if (!db.init(error)) {
        std::cerr << "Database init failed: " << error << std::endl;
        return 1;
    }

    AuthMiddleware::setDatabaseManager(&db);

    AuthService authService(db);
    EscrowService escrowService(db);

    crow::App<AuthMiddleware> app;

    // Handle OPTIONS requests for CORS preflight
    CROW_ROUTE(app, "/api/auth/register").methods("OPTIONS"_method)
    ([]() { return crow::response(200); });

    CROW_ROUTE(app, "/api/auth/login").methods("OPTIONS"_method)
    ([]() { return crow::response(200); });

    CROW_ROUTE(app, "/api/escrow/create").methods("OPTIONS"_method)
    ([]() { return crow::response(200); });

    CROW_ROUTE(app, "/api/escrow/<int>/deposit").methods("OPTIONS"_method)
    ([](int) { return crow::response(200); });

    CROW_ROUTE(app, "/api/escrow/<int>/ship").methods("OPTIONS"_method)
    ([](int) { return crow::response(200); });

    CROW_ROUTE(app, "/api/escrow/<int>/approve").methods("OPTIONS"_method)
    ([](int) { return crow::response(200); });

    setupRoutes(app, authService, escrowService);

    app.port(18080).multithreaded().run();
    return 0;
}
