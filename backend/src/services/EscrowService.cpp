#include "EscrowService.h"
#include <stdexcept>


//this service Checks Whether all the data is present in the request

EscrowService::EscrowService(DatabaseManager& db) : db(db) {}

bool EscrowService::createEscrow(int buyerId, int sellerId, int agentId, double amount, int& escrowId, std::string& error) {
    if (amount <= 0) {
        error = "Amount must be greater than 0";
        return false;
    }

    User buyer;
    if (!db.getUserById(buyerId, buyer, error)) {
        error = "Buyer does not exist";
        return false;
    }

    User seller;
    if (!db.getUserById(sellerId, seller, error)) {
        error = "Seller does not exist";
        return false;
    }

    if (buyer.getRole() != "BUYER") {
        error = "Authenticated user must be a buyer to create an escrow";
        return false;
    }

    if (seller.getRole() != "SELLER") {
        error = "Selected seller must have role SELLER";
        return false;
    }

    return db.createEscrow(buyerId, sellerId, agentId, amount, escrowId, error);
}

bool EscrowService::deposit(int escrowId, double amount, std::string& error) {
    EscrowAccount account;
    if (!db.getEscrowById(escrowId, account, error)) {
        return false;
    }

    try {
        account.deposit(amount);
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }

    return db.updateEscrow(escrowId, account.getStatus(), account.getBalance(), error);
}

bool EscrowService::ship(int escrowId, std::string& error) {
    EscrowAccount account;
    if (!db.getEscrowById(escrowId, account, error)) {
        return false;
    }

    try {
        account.ship();
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }

    return db.updateEscrow(escrowId, account.getStatus(), account.getBalance(), error);
}

bool EscrowService::approve(int escrowId, std::string& error) {
    EscrowAccount account;
    if (!db.getEscrowById(escrowId, account, error)) {
        return false;
    }

    try {
        account.approve();
    } catch (const std::exception& ex) {
        error = ex.what();
        return false;
    }

    return db.updateEscrow(escrowId, account.getStatus(), account.getBalance(), error);
}

bool EscrowService::getEscrowsForUser(int userId, std::vector<EscrowAccount>& escrows, std::string& error) {
    return db.getEscrowsForUser(userId, escrows, error);
}

bool EscrowService::getUserById(int userId, User& user, std::string& error) {
    return db.getUserById(userId, user, error);
}
