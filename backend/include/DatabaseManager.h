#pragma once

#include <string>
#include <vector>
#include <sqlite3.h>
#include "User.h"
#include "EscrowAccount.h"

class DatabaseManager {
private:
    sqlite3* db;
    std::string dbPath;

public:
    DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    bool init(std::string& error);

    bool createUser(const std::string& username, const std::string& password, const std::string& role, std::string& error);
    bool getUserByUsername(const std::string& username, User& user, std::string& error);
    bool getUserById(int userId, User& user, std::string& error);
    bool validateUser(const std::string& username, const std::string& password, User& user, std::string& error);

    bool createEscrow(int buyerId, int sellerId, int agentId, double amount, int& escrowId, std::string& error);
    bool getEscrowById(int escrowId, EscrowAccount& account, std::string& error);
    bool updateEscrow(int escrowId, const std::string& status, double balance, std::string& error);
    bool getEscrowsForUser(int userId, std::vector<EscrowAccount>& escrows, std::string& error);
};