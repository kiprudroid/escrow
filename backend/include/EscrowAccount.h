#pragma once

#include <string>
#include "EscrowState.h"

class EscrowAccount {
private:
    int id;
    int buyerId;
    int sellerId;
    int agentId;
    double amount;
    double balance;
    std::string status;
    EscrowState* state;

    EscrowState* createStateForStatus(const std::string& status);
    void setState(EscrowState* newState);

public:
    EscrowAccount();
    EscrowAccount(int id, int buyerId, int sellerId, int agentId, double amount, double balance, const std::string& status);

    //destructor
    ~EscrowAccount();

    
    EscrowAccount(const EscrowAccount& other);
    EscrowAccount& operator=(const EscrowAccount& other);

    int getId() const;
    int getBuyerId() const;
    int getSellerId() const;
    int getAgentId() const;
    double getAmount() const;
    double getBalance() const;
    const std::string& getStatus() const;

    void setBalance(double newBalance);
    void setStatus(const std::string& newStatus);

    void deposit(double amount);
    void ship();
    void approve();
};
