#pragma once

#include <string>
#include <vector>
#include "DatabaseManager.h"

class EscrowService {
private:
    DatabaseManager& db;

public:
    EscrowService(DatabaseManager& db);

    //virtual functions
    bool createEscrow(int buyerId, int sellerId, int agentId, double amount, int& escrowId, std::string& error);
    bool deposit(int escrowId, double amount, std::string& error);
    bool ship(int escrowId, std::string& error);
    bool approve(int escrowId, std::string& error);
    bool getEscrowsForUser(int userId, std::vector<EscrowAccount>& escrows, std::string& error);
    bool getUserById(int userId, User& user, std::string& error);
};