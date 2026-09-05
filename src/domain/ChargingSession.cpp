#include "ChargingSession.h"
#include <ctime>

namespace evc::domain {

ChargingSession::ChargingSession() : vehicleId(0), chargingPointId(0), userId(0),
                                     startTime(0), endTime(0), status(SessionStatus::IDLE),
                                     energyDeliveredKwh(0), initialBatteryPercent(0),
                                     targetBatteryPercent(100), energyRequiredKwh(0),
                                     estimatedChargingTimeMinutes(0), actualChargingTimeMinutes(0),
                                     totalCost(0), chargerEfficiencyPercent(95),
                                     isEmergencySession(false) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

ChargingSession::ChargingSession(const std::string& sId, int vId, int ptId, int uId)
    : sessionId(sId), vehicleId(vId), chargingPointId(ptId), userId(uId),
      startTime(0), endTime(0), status(SessionStatus::IDLE),
      energyDeliveredKwh(0), initialBatteryPercent(0),
      targetBatteryPercent(100), energyRequiredKwh(0),
      estimatedChargingTimeMinutes(0), actualChargingTimeMinutes(0),
      totalCost(0), chargerEfficiencyPercent(95),
      isEmergencySession(false) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

std::string ChargingSession::getStatusString() const {
    switch (status) {
        case SessionStatus::IDLE:
            return "IDLE";
        case SessionStatus::CHARGING:
            return "CHARGING";
        case SessionStatus::PAUSED:
            return "PAUSED";
        case SessionStatus::COMPLETED:
            return "COMPLETED";
        case SessionStatus::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

int ChargingSession::getDurationMinutes() const {
    if (startTime == 0 || endTime == 0) return 0;
    return static_cast<int>((endTime - startTime) / 60);
}

double ChargingSession::getEffectiveEnergyDelivered() const {
    return energyDeliveredKwh * (chargerEfficiencyPercent / 100.0);
}

}
