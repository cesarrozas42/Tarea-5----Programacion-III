#ifndef EJ1_TRANSACTION_H
#define EJ1_TRANSACTION_H

#include <string>

using namespace std;

enum class TransactionType { Deposit, Withdraw };

class Transaction {
public:
    Transaction(unsigned long id, TransactionType type, string accountId,
                double amount, double balanceAfter, bool success)
        : id_(id), type_(type), accountId_(move(accountId)),
          amount_(amount), balanceAfter_(balanceAfter), success_(success) {}

    unsigned long getId() const { return id_; }
    TransactionType getType() const { return type_; }
    const string& getAccountId() const { return accountId_; }
    double getAmount() const { return amount_; }
    double getBalanceAfter() const { return balanceAfter_; }
    bool isSuccessful() const { return success_; }

    string toString() const {
        return "#" + to_string(id_) + " " +
               (type_ == TransactionType::Deposit ? "DEPOSIT " : "WITHDRAW ") +
               accountId_ + " " + to_string(amount_) +
               (success_ ? " OK" : " RECHAZADA") +
               " -> balance " + to_string(balanceAfter_);
    }

private:
    unsigned long id_;
    TransactionType type_;
    string accountId_;
    double amount_;
    double balanceAfter_;
    bool success_;
};

#endif
