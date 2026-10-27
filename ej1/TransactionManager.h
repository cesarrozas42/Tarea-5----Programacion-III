#ifndef EJ1_TRANSACTIONMANAGER_H
#define EJ1_TRANSACTIONMANAGER_H

#include <mutex>
#include <vector>

#include "Account.h"
#include "Transaction.h"

using namespace std;

class TransactionManager {
public:
    static TransactionManager& getInstance() {
        static TransactionManager instance;
        return instance;
    }

    TransactionManager(const TransactionManager&) = delete;
    TransactionManager& operator=(const TransactionManager&) = delete;
    TransactionManager(TransactionManager&&) = delete;
    TransactionManager& operator=(TransactionManager&&) = delete;

    bool deposit(Account* account, double amount) {
        if (account == nullptr) return false;

        lock_guard<mutex> lock(mutex_);

        const bool ok = amount > 0;
        if (ok) account->balance_ += amount;

        history_.emplace_back(nextId_++, TransactionType::Deposit,
                              account->getId(), amount, account->balance_, ok);
        return ok;
    }

    bool withdraw(Account* account, double amount) {
        if (account == nullptr) return false;

        lock_guard<mutex> lock(mutex_);

        // La verificacion de fondos y el descuento van juntos bajo el mismo
        // lock: asi dos hilos no pueden pasar el chequeo con el mismo saldo.
        const bool ok = amount > 0 && account->balance_ >= amount;
        if (ok) account->balance_ -= amount;

        history_.emplace_back(nextId_++, TransactionType::Withdraw,
                              account->getId(), amount, account->balance_, ok);
        return ok;
    }

    vector<Transaction> getHistory() const {
        lock_guard<mutex> lock(mutex_);
        return history_;
    }

    size_t getTransactionCount() const {
        lock_guard<mutex> lock(mutex_);
        return history_.size();
    }

private:
    TransactionManager() = default;
    ~TransactionManager() = default;

    mutable mutex mutex_;
    vector<Transaction> history_;
    unsigned long nextId_ = 1;
};

#endif
