#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <ctime>
#include <vector>

namespace evc::domain {

enum class WalletTransactionType {
    CREDIT,
    DEBIT
};

struct WalletTransaction {
    std::string transactionId;
    WalletTransactionType type;
    double amount;
    std::string reason;  // Charging, Refund, Promotional, Manual
    double previousBalance;
    double newBalance;
    time_t timestamp;
};

class Wallet {
private:
    int walletId;
    int userId;
    double balance;
    double totalCreditsAdded;
    double totalSpent;
    time_t lastUpdated;
    std::vector<WalletTransaction> transactions;
    time_t createdAt;

public:
    // Constructor
    Wallet();
    Wallet(int wId, int uId);

    // Getters
    int getWalletId() const { return walletId; }
    int getUserId() const { return userId; }
    double getBalance() const { return balance; }
    double getTotalCreditsAdded() const { return totalCreditsAdded; }
    double getTotalSpent() const { return totalSpent; }
    time_t getLastUpdated() const { return lastUpdated; }
    const std::vector<WalletTransaction>& getTransactions() const { return transactions; }
    time_t getCreatedAt() const { return createdAt; }

    // Setters
    void setWalletId(int id) { walletId = id; }
    void setUserId(int uId) { userId = uId; }
    void setBalance(double bal) { balance = bal; lastUpdated = time(nullptr); }
    void setTotalCreditsAdded(double credits) { totalCreditsAdded = credits; }
    void setTotalSpent(double spent) { totalSpent = spent; }
    void setLastUpdated(time_t time) { lastUpdated = time; }

    // Utility
    bool hassufficientBalance(double amount) const { return balance >= amount; }
    void addCredit(double amount, const std::string& reason);
    void debitAmount(double amount, const std::string& reason);
    void addTransaction(const WalletTransaction& trans) { transactions.push_back(trans); }
    double getMonthlySpend() const;
};

}

#endif // WALLET_H