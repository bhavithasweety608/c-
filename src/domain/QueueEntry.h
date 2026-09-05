#ifndef QUEUE_ENTRY_H
#define QUEUE_ENTRY_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class QueueStatus {
    WAITING,
    ASSIGNED,
    CHARGING,
    COMPLETED,
    CANCELLED
};

class QueueEntry {
private:
    int queueId;
    std::string tokenNumber;
    int userId;
    int vehicleId;
    int stationId;
    std::string desiredChargerType;
    time_t joinedTime;
    int queuePosition;
    QueueStatus status;
    int estimatedWaitMinutes;
    int priorityLevel;  // 0=normal, 1=medium, 2=high (emergency)
    bool isEmergency;
    time_t createdAt;

public:
    // Constructor
    QueueEntry();
    QueueEntry(int qId, const std::string& token, int uId, int vId, int sId);

    // Getters
    int getQueueId() const { return queueId; }
    const std::string& getTokenNumber() const { return tokenNumber; }
    int getUserId() const { return userId; }
    int getVehicleId() const { return vehicleId; }
    int getStationId() const { return stationId; }
    const std::string& getDesiredChargerType() const { return desiredChargerType; }
    time_t getJoinedTime() const { return joinedTime; }
    int getQueuePosition() const { return queuePosition; }
    QueueStatus getStatus() const { return status; }
    int getEstimatedWaitMinutes() const { return estimatedWaitMinutes; }
    int getPriorityLevel() const { return priorityLevel; }
    bool getIsEmergency() const { return isEmergency; }
    time_t getCreatedAt() const { return createdAt; }

    // Setters
    void setQueueId(int id) { queueId = id; }
    void setTokenNumber(const std::string& token) { tokenNumber = token; }
    void setUserId(int uId) { userId = uId; }
    void setVehicleId(int vId) { vehicleId = vId; }
    void setStationId(int sId) { stationId = sId; }
    void setDesiredChargerType(const std::string& ctype) { desiredChargerType = ctype; }
    void setJoinedTime(time_t time) { joinedTime = time; }
    void setQueuePosition(int pos) { queuePosition = pos; }
    void setStatus(QueueStatus s) { status = s; }
    void setEstimatedWaitMinutes(int minutes) { estimatedWaitMinutes = minutes; }
    void setPriorityLevel(int level) { priorityLevel = level; }
    void setIsEmergency(bool emergency) { isEmergency = emergency; }

    // Utility
    std::string getStatusString() const;
    bool isHighPriority() const { return priorityLevel >= 2; }
};

}

#endif // QUEUE_ENTRY_H