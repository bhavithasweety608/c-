#include "VirtualQueueEntry.h"
#include <ctime>

namespace evc::domain {

VirtualQueueEntry::VirtualQueueEntry() : queueId(0), userId(0), vehicleId(0), stationId(0),
                                         joinedTime(0), estimatedTurnTime(0),
                                         status(VirtualQueueStatus::ONLINE_WAITING),
                                         arrivalConfirmedTime(0), expiryTime(0),
                                         isPhysicallyPresent(false), lastPositionUpdate(0),
                                         queuePosition(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

VirtualQueueEntry::VirtualQueueEntry(int qId, int uId, int vId, int sId)
    : queueId(qId), userId(uId), vehicleId(vId), stationId(sId),
      joinedTime(0), estimatedTurnTime(0),
      status(VirtualQueueStatus::ONLINE_WAITING),
      arrivalConfirmedTime(0), expiryTime(0),
      isPhysicallyPresent(false), lastPositionUpdate(0),
      queuePosition(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
    joinedTime = time(nullptr);
}

std::string VirtualQueueEntry::getStatusString() const {
    switch (status) {
        case VirtualQueueStatus::ONLINE_WAITING:
            return "ONLINE_WAITING";
        case VirtualQueueStatus::NOTIFIED_15MIN:
            return "NOTIFIED_15MIN";
        case VirtualQueueStatus::NOTIFIED_ARRIVING:
            return "NOTIFIED_ARRIVING";
        case VirtualQueueStatus::CONFIRMED_ARRIVAL:
            return "CONFIRMED_ARRIVAL";
        case VirtualQueueStatus::EXPIRED:
            return "EXPIRED";
        case VirtualQueueStatus::CHARGING:
            return "CHARGING";
        case VirtualQueueStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}

bool VirtualQueueEntry::isExpired() const {
    if (expiryTime == 0) return false;
    return time(nullptr) > expiryTime;
}

int VirtualQueueEntry::getMinutesUntilTurn() const {
    if (estimatedTurnTime == 0) return 0;
    time_t now = time(nullptr);
    if (estimatedTurnTime <= now) return 0;
    return static_cast<int>((estimatedTurnTime - now) / 60);
}

int VirtualQueueEntry::getWaitingTimeMinutes() const {
    if (joinedTime == 0) return 0;
    return static_cast<int>((time(nullptr) - joinedTime) / 60);
}

bool VirtualQueueEntry::hasReceived15MinAlert() const {
    for (const auto& notif : notifications) {
        if (notif.notificationType == "15MIN_ALERT") {
            return true;
        }
    }
    return false;
}

bool VirtualQueueEntry::hasReceivedArrivalAlert() const {
    for (const auto& notif : notifications) {
        if (notif.notificationType == "ARRIVAL_ALERT") {
            return true;
        }
    }
    return false;
}

}
