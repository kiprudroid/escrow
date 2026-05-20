#include "crow.h"
#include "EscrowService.h"
#include "AuthMiddleware.h"

//(helper)for errors and other such information since cpp lacks a native way of handling json . (utils/)
crow::response jsonMessage(int code, const std::string& message);

//(helper)used for the data in the response
crow::response jsonData(int code, crow::json::wvalue data);

void registerEscrowRoutes(crow::App<AuthMiddleware>& app, EscrowService& escrowService) {

    // POST /api/escrow/create
    CROW_ROUTE(app, "/api/escrow/create").methods("POST"_method)
    ([&app, &escrowService](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return jsonMessage(400, "Invalid JSON payload");
        }

        int sellerId = 0;
        double amount = 0.0;

        try {
            if (!body.has("seller_id") || !body.has("amount")) {
                return jsonMessage(400, "seller_id and amount are required");
            }

            sellerId = body["seller_id"].i();
            amount = body["amount"].d();
        } catch (const std::exception&) {
            return jsonMessage(400, "Invalid JSON payload shape");
        }

        const auto& ctx = app.get_context<AuthMiddleware>(req);
        int buyerId = ctx.userId;

        
        int agentId = 0;

        std::string error;
        int escrowId = 0;


        //if some of the request data is missing
        if (!escrowService.createEscrow(buyerId, sellerId, agentId, amount, escrowId, error)) {
            return jsonMessage(400, error);
        }

        User buyer;
        User seller;
        std::string userError;
        if (!escrowService.getUserById(buyerId, buyer, userError) || !escrowService.getUserById(sellerId, seller, userError)) {
            return jsonMessage(400, "Buyer or seller could not be loaded");
        }

        crow::json::wvalue data;
        data["escrow_id"] = escrowId;
        data["status"] = "INITIATED";
        data["buyer_id"] = buyer.getId();
        data["buyer_username"] = buyer.getUsername();
        data["seller_id"] = seller.getId();
        data["seller_username"] = seller.getUsername();
        return jsonData(201, data);
    });

    // GET /api/escrows - list escrows related to authenticated user
    CROW_ROUTE(app, "/api/escrows").methods("GET"_method)
    ([&app, &escrowService](const crow::request& req) {
        const auto& ctx = app.get_context<AuthMiddleware>(req);
        if (!ctx.authenticated) {
            return jsonMessage(401, "Unauthorized");
        }

        int userId = ctx.userId;
        std::vector<EscrowAccount> escrows;
        std::string error;
        if (!escrowService.getEscrowsForUser(userId, escrows, error)) {
            return jsonMessage(500, error);
        }

        crow::json::wvalue data;
        data["escrows"] = crow::json::wvalue::list();
        int index = 0;
        for (const auto& e : escrows) {
            User buyer;
            User seller;
            std::string userError;
            std::string buyerUsername = "Unknown";
            std::string sellerUsername = "Unknown";

            if (escrowService.getUserById(e.getBuyerId(), buyer, userError)) {
                buyerUsername = buyer.getUsername();
            }
            if (escrowService.getUserById(e.getSellerId(), seller, userError)) {
                sellerUsername = seller.getUsername();
            }

            crow::json::wvalue item;
            item["id"] = e.getId();
            item["buyer_id"] = e.getBuyerId();
            item["buyer_username"] = buyerUsername;
            item["seller_id"] = e.getSellerId();
            item["seller_username"] = sellerUsername;
            item["agent_id"] = e.getAgentId();
            item["amount"] = e.getAmount();
            item["balance"] = e.getBalance();
            item["status"] = e.getStatus();
            data["escrows"][index++] = std::move(item);
        }

        return jsonData(200, data);
    });

    //Deposit Money into an escrow
    //POST /api/escrow/(esceow id)/deposit
    CROW_ROUTE(app, "/api/escrow/<int>/deposit").methods("POST"_method)
    ([&escrowService](const crow::request& req, int escrowId) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return jsonMessage(400, "Invalid JSON payload");
        }

        if (!body.has("amount")) {
            return jsonMessage(400, "amount is required");
        }

        double amount = body["amount"].d();
        std::string error;
        if (!escrowService.deposit(escrowId, amount, error)) {
            return jsonMessage(400, error);
        }

        return jsonMessage(200, "Deposit successful, escrow is now SECURED");
    });

    //Ship the Good and the Service to the Buyer
    //POST /api/escrow/(escrow-id)/ship
    CROW_ROUTE(app, "/api/escrow/<int>/ship").methods("POST"_method)
    ([&escrowService](const crow::request& req, int escrowId) {
        (void)req;
        std::string error;
        if (!escrowService.ship(escrowId, error)) {
            return jsonMessage(400, error);
        }

        return jsonMessage(200, "Escrow is now IN_INSPECTION");
    });

    //Approve The Transaction to take Place
    // POST /api/escrow/(escrow-id)/approve
    
    CROW_ROUTE(app, "/api/escrow/<int>/approve").methods("POST"_method)
    ([&escrowService](const crow::request& req, int escrowId) {
        (void)req;
        std::string error;
        if (!escrowService.approve(escrowId, error)) {
            return jsonMessage(400, error);
        }

        return jsonMessage(200, "Escrow is now CLOSED");
    });
}
