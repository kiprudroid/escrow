#include "EscrowAccount.h"
#include <stdexcept>

//Escrow Deposits only happen when transaction is in the initiated state
// This file has implemented JS-Like state management

void EscrowState::deposit(EscrowAccount& account, double amount) {
    throw std::runtime_error("Deposit not allowed in state " + name());
}

void EscrowState::ship(EscrowAccount& account) {
    throw std::runtime_error("Ship not allowed in state " + name());
}

void EscrowState::approve(EscrowAccount& account) {
    throw std::runtime_error("Approve not allowed in state " + name());
}

std::string InitiatedState::name() const {
    return "INITIATED";
}

void InitiatedState::deposit(EscrowAccount& account, double amount) {
    if (amount <= 0) {
        throw std::runtime_error("Deposit amount must be greater than 0");
    }
    if (amount != account.getAmount()) {
        throw std::runtime_error("Deposit amount must match escrow amount");
    }
    account.setBalance(account.getBalance() + amount);
    account.setStatus("SECURED");
}

std::string SecuredState::name() const {
    return "SECURED";
}

void SecuredState::ship(EscrowAccount& account) {
    account.setStatus("IN_INSPECTION");
}

std::string InInspectionState::name() const {
    return "IN_INSPECTION";
}

void InInspectionState::approve(EscrowAccount& account) {
    account.setBalance(0);
    account.setStatus("CLOSED");
}

std::string ClosedState::name() const {
    return "CLOSED";
}

EscrowAccount::EscrowAccount()
    : id(0), buyerId(0), sellerId(0), agentId(0), amount(0), balance(0), status("INITIATED"), state(nullptr) {
    setState(createStateForStatus(status));
}

EscrowAccount::EscrowAccount(int id, int buyerId, int sellerId, int agentId, double amount, double balance, const std::string& status)
    : id(id), buyerId(buyerId), sellerId(sellerId), agentId(agentId), amount(amount), balance(balance), status(status), state(nullptr) {
    setState(createStateForStatus(status));
}

EscrowAccount::~EscrowAccount() {
    delete state;
    state = nullptr;
}

EscrowAccount::EscrowAccount(const EscrowAccount& other)
    : id(other.id), buyerId(other.buyerId), sellerId(other.sellerId), agentId(other.agentId),
      amount(other.amount), balance(other.balance), status(other.status), state(nullptr) {
    // Create a new state object based on the status
    setState(createStateForStatus(status));
}

EscrowAccount& EscrowAccount::operator=(const EscrowAccount& other) {
    if (this == &other) {
        return *this;
    }
    
    id = other.id;
    buyerId = other.buyerId;
    sellerId = other.sellerId;
    agentId = other.agentId;
    amount = other.amount;
    balance = other.balance;
    status = other.status;
    
    // Create a new state object based on the status
    setState(createStateForStatus(status));
    
    return *this;
}

int EscrowAccount::getId() const {
    return id;
}

int EscrowAccount::getBuyerId() const {
    return buyerId;
}

int EscrowAccount::getSellerId() const {
    return sellerId;
}

int EscrowAccount::getAgentId() const {
    return agentId;
}

double EscrowAccount::getAmount() const {
    return amount;
}

double EscrowAccount::getBalance() const {
    return balance;
}

const std::string& EscrowAccount::getStatus() const {
    return status;
}

void EscrowAccount::setBalance(double newBalance) {
    balance = newBalance;
}

void EscrowAccount::setStatus(const std::string& newStatus) {
    status = newStatus;
    setState(createStateForStatus(status));
}

void EscrowAccount::deposit(double amount) {
    if (!state) {
        throw std::runtime_error("Escrow state is not initialized");
    }
    state->deposit(*this, amount);
}

void EscrowAccount::ship() {
    if (!state) {
        throw std::runtime_error("Escrow state is not initialized");
    }
    state->ship(*this);
}

void EscrowAccount::approve() {
    if (!state) {
        throw std::runtime_error("Escrow state is not initialized");
    }
    state->approve(*this);
}

EscrowState* EscrowAccount::createStateForStatus(const std::string& status) {
    if (status == "INITIATED") {
        return new InitiatedState();
    }
    if (status == "SECURED") {
        return new SecuredState();
    }
    if (status == "IN_INSPECTION") {
        return new InInspectionState();
    }
    if (status == "CLOSED") {
        return new ClosedState();
    }
    return new InitiatedState();
}

void EscrowAccount::setState(EscrowState* newState) {
    if (state != nullptr) {
        delete state;
    }
    state = newState;
}
