#include "PaymentRecord.h"
#include <ctime>

namespace evc::domain {

PaymentRecord::PaymentRecord() : userId(0), amount(0), currency("INR"),
                                 paymentMethod(PaymentMethod::CARD),
                                 status(PaymentStatus::PENDING),
                                 initiatedAt(0), completedAt(0), retryCount(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

PaymentRecord::PaymentRecord(const std::string& pId, const std::string& sId, int uId, double amt)
    : paymentId(pId), sessionId(sId), userId(uId), amount(amt), currency("INR"),
      paymentMethod(PaymentMethod::CARD),
      status(PaymentStatus::PENDING),
      initiatedAt(0), completedAt(0), retryCount(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
    initiatedAt = time(nullptr);
}

std::string PaymentRecord::getPaymentMethodString() const {
    switch (paymentMethod) {
        case PaymentMethod::CARD:
            return "CARD";
        case PaymentMethod::WALLET:
            return "WALLET";
        case PaymentMethod::UPI:
            return "UPI";
        case PaymentMethod::NETBANKING:
            return "NETBANKING";
        case PaymentMethod::PREPAID:
            return "PREPAID";
        default:
            return "UNKNOWN";
    }
}

std::string PaymentRecord::getStatusString() const {
    switch (status) {
        case PaymentStatus::PENDING:
            return "PENDING";
        case PaymentStatus::COMPLETED:
            return "COMPLETED";
        case PaymentStatus::FAILED:
            return "FAILED";
        case PaymentStatus::REFUNDED:
            return "REFUNDED";
        default:
            return "UNKNOWN";
    }
}

}
