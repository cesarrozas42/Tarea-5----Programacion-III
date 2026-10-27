#ifndef EJ1_ACCOUNT_H
#define EJ1_ACCOUNT_H

#include <string>
#include <utility>

using namespace std;

class TransactionManager;

class Account {
public:
    Account(string id, double initialBalance)
        : id_(move(id)), balance_(initialBalance) {}

    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;

    const string& getId() const { return id_; }
    double getBalance() const { return balance_; }

private:
    friend class TransactionManager;

    string id_;
    double balance_;
};

#endif
