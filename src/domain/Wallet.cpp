#include "Wallet.h"
#include <ctime>
#include <algorithm>

namespace evc::domain {

Wallet::Wallet() : walletId(0), userId(0), balance(0),
                   totalCreditsAdded(0), totalSpent(0) {
    createdAt = time(nullptr);
    lastUpdated = time(nullptr);
}

Wallet::Wallet(int wId, int uId) : walletId(wId), userId(uId), balance(0),
                                    totalCreditsAdded(0), totalSpent(0) {
    createdAt = time(nullptr);
    lastUpdated = time(nullptr);
}

void Wallet::addCredit(double amount, const std::string& reason) {
    if (amount <= 0) return;
    
    WalletTransaction trans;
    trans.previousBalance = balance;
    balance += amount;
    trans.newBalance = balance;
    trans.amount = amount;
    trans.type = WalletTransactionType::CREDIT;
    trans.reason = reason;
    trans.timestamp = time(nullptr);
    
    transactions.push_back(trans);
    totalCreditsAdded += amount;
    lastUpdated = time(nullptr);
}

void Wallet::debitAmount(double amount, const std::string& reason) {
    if (amount <= 0 || amount > balance) return;
    
    WalletTransaction trans;
    trans.previousBalance = balance;
    balance -= amount;
    trans.newBalance = balance;
    trans.amount = amount;
    trans.type = WalletTransactionType::DEBIT;
    trans.reason = reason;
    trans.timestamp = time(nullptr);
    
    transactions.push_back(trans);
    totalSpent += amount;
    lastUpdated = time(nullptr);
}

double Wallet::getMonthlySpend() const {
    time_t now = time(nullptr);
    time_t oneMonthAgo = now - (30 * 24 * 60 * 60);
    
    double monthlySpend = 0;
    for (const auto& trans : transactions) {
        if (trans.type == WalletTransactionType::DEBIT && 
            trans.timestamp >= oneMonthAgo && trans.timestamp <= now) {
            monthlySpend += trans.amount;
        }
    }
    return monthlySpend;
}

}
