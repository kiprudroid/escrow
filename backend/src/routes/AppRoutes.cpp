#include "crow.h"
#include "AuthMiddleware.h"
#include "AuthService.h"
#include "EscrowService.h"

void registerAuthRoutes(crow::App<AuthMiddleware>& app, AuthService& authService);
void registerEscrowRoutes(crow::App<AuthMiddleware>& app, EscrowService& escrowService);

void setupRoutes(crow::App<AuthMiddleware>& app, AuthService& authService, EscrowService& escrowService) {
    registerAuthRoutes(app, authService);
    registerEscrowRoutes(app, escrowService);
}
