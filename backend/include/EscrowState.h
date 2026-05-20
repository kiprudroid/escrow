#pragma once

#include <string>

class EscrowAccount;

class EscrowState {
public:
    virtual ~EscrowState() = default;

    virtual std::string name() const = 0;
    virtual void deposit(EscrowAccount& account, double amount);
    virtual void ship(EscrowAccount& account);
    virtual void approve(EscrowAccount& account);
};

class InitiatedState : public EscrowState {
public:
    std::string name() const override;
    void deposit(EscrowAccount& account, double amount) override;
};

class SecuredState : public EscrowState {
public:
    std::string name() const override;
    void ship(EscrowAccount& account) override;
};

class InInspectionState : public EscrowState {
public:
    std::string name() const override;
    void approve(EscrowAccount& account) override;
};

class ClosedState : public EscrowState {
public:
    std::string name() const override;
};
