#ifndef PAYMENT_RECORD_H
#define PAYMENT_RECORD_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class PaymentMethod {
    CARD,
    WALLET,
    UPI,
    NETBANKING,
    PREPAID
};

enum class PaymentStatus {
    PENDING,
    COMPLETED,
    FAILED,
    REFUNDED
};

class PaymentRecord {
private:
    std::string paymentId;
    std::string sessionId;
    int userId;
    double amount;
    std::string currency;
    PaymentMethod paymentMethod;
    std::string paymentGateway;
    std::string transactionId;
    std::string gatewayResponse;
    PaymentStatus status;
    time_t initiatedAt;
    time_t completedAt;
    std::string receiptUrl;
    std::string failureReason;
    int retryCount;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    PaymentRecord();
    PaymentRecord(const std::string& pId, const std::string& sId, int uId, double amt);

    // Getters
    const std::string& getPaymentId() const { return paymentId; }
    const std::string& getSessionId() const { return sessionId; }
    int getUserId() const { return userId; }
    double getAmount() const { return amount; }
    const std::string& getCurrency() const { return currency; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    const std::string& getPaymentGateway() const { return paymentGateway; }
    const std::string& getTransactionId() const { return transactionId; }
    const std::string& getGatewayResponse() const { return gatewayResponse; }
    PaymentStatus getStatus() const { return status; }
    time_t getInitiatedAt() const { return initiatedAt; }
    time_t getCompletedAt() const { return completedAt; }
    const std::string& getReceiptUrl() const { return receiptUrl; }
    const std::string& getFailureReason() const { return failureReason; }
    int getRetryCount() const { return retryCount; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setPaymentId(const std::string& id) { paymentId = id; }
    void setSessionId(const std::string& sId) { sessionId = sId; }
    void setUserId(int uId) { userId = uId; }
    void setAmount(double amt) { amount = amt; }
    void setCurrency(const std::string& cur) { currency = cur; }
    void setPaymentMethod(PaymentMethod pm) { paymentMethod = pm; }
    void setPaymentGateway(const std::string& gw) { paymentGateway = gw; }
    void setTransactionId(const std::string& tId) { transactionId = tId; }
    void setGatewayResponse(const std::string& resp) { gatewayResponse = resp; }
    void setStatus(PaymentStatus s) { status = s; updatedAt = time(nullptr); }
    void setInitiatedAt(time_t time) { initiatedAt = time; }
    void setCompletedAt(time_t time) { completedAt = time; }
    void setReceiptUrl(const std::string& url) { receiptUrl = url; }
    void setFailureReason(const std::string& reason) { failureReason = reason; }
    void setRetryCount(int count) { retryCount = count; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    std::string getPaymentMethodString() const;
    std::string getStatusString() const;
    bool isSuccessful() const { return status == PaymentStatus::COMPLETED; }
    bool canRetry() const { return (status == PaymentStatus::FAILED || status == PaymentStatus::PENDING) && retryCount < 3; }
};

}

#endif // PAYMENT_RECORD_H