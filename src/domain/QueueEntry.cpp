#include "QueueEntry.h"
#include <ctime>

namespace evc::domain {

QueueEntry::QueueEntry() : queueId(0), userId(0), vehicleId(0), stationId(0),
                           joinedTime(0), queuePosition(0), status(QueueStatus::WAITING),
                           estimatedWaitMinutes(0), priorityLevel(0), isEmergency(false) {
    createdAt = time(nullptr);
}

QueueEntry::QueueEntry(int qId, const std::string& token, int uId, int vId, int sId)
    : queueId(qId), tokenNumber(token), userId(uId), vehicleId(vId), stationId(sId),
      joinedTime(0), queuePosition(0), status(QueueStatus::WAITING),
      estimatedWaitMinutes(0), priorityLevel(0), isEmergency(false) {
    createdAt = time(nullptr);
    joinedTime = time(nullptr);
}

std::string QueueEntry::getStatusString() const {
    switch (status) {
        case QueueStatus::WAITING:
            return "WAITING";
        case QueueStatus::ASSIGNED:
            return "ASSIGNED";
        case QueueStatus::CHARGING:
            return "CHARGING";
        case QueueStatus::COMPLETED:
            return "COMPLETED";
        case QueueStatus::CANCELLED:
            return "CANCELLED";
        default:
            return "UNKNOWN";
    }
}

}
