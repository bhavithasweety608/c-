#ifndef VIRTUAL_QUEUE_ENTRY_H
#define VIRTUAL_QUEUE_ENTRY_H

#include <string>
#include <ctime>
#include <vector>

namespace evc::domain {

enum class VirtualQueueStatus {
    ONLINE_WAITING,
    NOTIFIED_15MIN,
    NOTIFIED_ARRIVING,
    CONFIRMED_ARRIVAL,
    EXPIRED,
    CHARGING,
    COMPLETED
};

struct NotificationLog {
    std::string notificationType;  // POSITION_UPDATE, 15MIN_ALERT, 5MIN_ALERT, etc.
    std::string message;
    time_t sentTime;
    std::string channel;  // SMS, EMAIL, PUSH, IN_APP
    bool isRead;
};

class VirtualQueueEntry {
private:
    int queueId;
    int userId;
    int vehicleId;
    int stationId;
    time_t joinedTime;
    time_t estimatedTurnTime;
    VirtualQueueStatus status;
    time_t arrivalConfirmedTime;
    time_t expiryTime;
    bool isPhysicallyPresent;
    time_t lastPositionUpdate;
    int queuePosition;
    std::vector<NotificationLog> notifications;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    VirtualQueueEntry();
    VirtualQueueEntry(int qId, int uId, int vId, int sId);

    // Getters
    int getQueueId() const { return queueId; }
    int getUserId() const { return userId; }
    int getVehicleId() const { return vehicleId; }
    int getStationId() const { return stationId; }
    time_t getJoinedTime() const { return joinedTime; }
    time_t getEstimatedTurnTime() const { return estimatedTurnTime; }
    VirtualQueueStatus getStatus() const { return status; }
    time_t getArrivalConfirmedTime() const { return arrivalConfirmedTime; }
    time_t getExpiryTime() const { return expiryTime; }
    bool getIsPhysicallyPresent() const { return isPhysicallyPresent; }
    time_t getLastPositionUpdate() const { return lastPositionUpdate; }
    int getQueuePosition() const { return queuePosition; }
    const std::vector<NotificationLog>& getNotifications() const { return notifications; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setQueueId(int id) { queueId = id; }
    void setUserId(int uId) { userId = uId; }
    void setVehicleId(int vId) { vehicleId = vId; }
    void setStationId(int sId) { stationId = sId; }
    void setJoinedTime(time_t time) { joinedTime = time; }
    void setEstimatedTurnTime(time_t time) { estimatedTurnTime = time; }
    void setStatus(VirtualQueueStatus s) { status = s; updatedAt = time(nullptr); }
    void setArrivalConfirmedTime(time_t time) { arrivalConfirmedTime = time; }
    void setExpiryTime(time_t time) { expiryTime = time; }
    void setIsPhysicallyPresent(bool present) { isPhysicallyPresent = present; }
    void setLastPositionUpdate(time_t time) { lastPositionUpdate = time; }
    void setQueuePosition(int pos) { queuePosition = pos; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    std::string getStatusString() const;
    bool isExpired() const;
    int getMinutesUntilTurn() const;
    int getWaitingTimeMinutes() const;
    void addNotification(const NotificationLog& log) { notifications.push_back(log); }
    bool hasReceived15MinAlert() const;
    bool hasReceivedArrivalAlert() const;
};

}

#endif // VIRTUAL_QUEUE_ENTRY_H