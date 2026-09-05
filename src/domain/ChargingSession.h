#ifndef CHARGING_SESSION_H
#define CHARGING_SESSION_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class SessionStatus {
    IDLE,
    CHARGING,
    PAUSED,
    COMPLETED,
    ERROR
};

class ChargingSession {
private:
    std::string sessionId;
    int vehicleId;
    int chargingPointId;
    int userId;
    time_t startTime;
    time_t endTime;
    SessionStatus status;
    double energyDeliveredKwh;
    double initialBatteryPercent;
    double targetBatteryPercent;
    double energyRequiredKwh;
    int estimatedChargingTimeMinutes;
    int actualChargingTimeMinutes;
    double totalCost;
    double chargerEfficiencyPercent;
    bool isEmergencySession;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    ChargingSession();
    ChargingSession(const std::string& sId, int vId, int ptId, int uId);

    // Getters
    const std::string& getSessionId() const { return sessionId; }
    int getVehicleId() const { return vehicleId; }
    int getChargingPointId() const { return chargingPointId; }
    int getUserId() const { return userId; }
    time_t getStartTime() const { return startTime; }
    time_t getEndTime() const { return endTime; }
    SessionStatus getStatus() const { return status; }
    double getEnergyDeliveredKwh() const { return energyDeliveredKwh; }
    double getInitialBatteryPercent() const { return initialBatteryPercent; }
    double getTargetBatteryPercent() const { return targetBatteryPercent; }
    double getEnergyRequiredKwh() const { return energyRequiredKwh; }
    int getEstimatedChargingTimeMinutes() const { return estimatedChargingTimeMinutes; }
    int getActualChargingTimeMinutes() const { return actualChargingTimeMinutes; }
    double getTotalCost() const { return totalCost; }
    double getChargerEfficiencyPercent() const { return chargerEfficiencyPercent; }
    bool getIsEmergencySession() const { return isEmergencySession; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setSessionId(const std::string& sId) { sessionId = sId; }
    void setVehicleId(int vId) { vehicleId = vId; }
    void setChargingPointId(int ptId) { chargingPointId = ptId; }
    void setUserId(int uId) { userId = uId; }
    void setStartTime(time_t st) { startTime = st; }
    void setEndTime(time_t et) { endTime = et; }
    void setStatus(SessionStatus s) { status = s; updatedAt = time(nullptr); }
    void setEnergyDeliveredKwh(double energy) { energyDeliveredKwh = energy; }
    void setInitialBatteryPercent(double percent) { initialBatteryPercent = percent; }
    void setTargetBatteryPercent(double percent) { targetBatteryPercent = percent; }
    void setEnergyRequiredKwh(double energy) { energyRequiredKwh = energy; }
    void setEstimatedChargingTimeMinutes(int minutes) { estimatedChargingTimeMinutes = minutes; }
    void setActualChargingTimeMinutes(int minutes) { actualChargingTimeMinutes = minutes; }
    void setTotalCost(double cost) { totalCost = cost; }
    void setChargerEfficiencyPercent(double efficiency) { chargerEfficiencyPercent = efficiency; }
    void setIsEmergencySession(bool emergency) { isEmergencySession = emergency; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    std::string getStatusString() const;
    int getDurationMinutes() const;
    double getEffectiveEnergyDelivered() const;
};

}

#endif // CHARGING_SESSION_H