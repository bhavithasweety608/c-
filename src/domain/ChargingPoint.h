#ifndef CHARGING_POINT_H
#define CHARGING_POINT_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class ChargerType {
    TWO_W_STANDARD,
    THREE_W_STANDARD,
    FOUR_W_STANDARD,
    FOUR_W_FAST
};

enum class PointStatus {
    AVAILABLE,
    OCCUPIED,
    FAULTED,
    MAINTENANCE
};

class ChargingPoint {
private:
    int pointId;
    int stationId;
    ChargerType chargerType;
    double maxPowerKw;
    double currentPowerKw;
    PointStatus status;
    std::string vehicleTypeSupported;  // "2W", "3W", or "4W"
    time_t lastMaintenanceDate;
    bool isActive;
    time_t creationDate;

public:
    // Constructor
    ChargingPoint();
    ChargingPoint(int id, int stationId, ChargerType ctype, double power);

    // Getters
    int getPointId() const { return pointId; }
    int getStationId() const { return stationId; }
    ChargerType getChargerType() const { return chargerType; }
    double getMaxPowerKw() const { return maxPowerKw; }
    double getCurrentPowerKw() const { return currentPowerKw; }
    PointStatus getStatus() const { return status; }
    const std::string& getVehicleTypeSupported() const { return vehicleTypeSupported; }
    bool getIsActive() const { return isActive; }
    time_t getLastMaintenanceDate() const { return lastMaintenanceDate; }
    time_t getCreationDate() const { return creationDate; }

    // Setters
    void setPointId(int id) { pointId = id; }
    void setStationId(int sId) { stationId = sId; }
    void setChargerType(ChargerType ctype) { chargerType = ctype; }
    void setMaxPowerKw(double power) { maxPowerKw = power; }
    void setCurrentPowerKw(double power) { currentPowerKw = power; }
    void setStatus(PointStatus s) { status = s; }
    void setVehicleTypeSupported(const std::string& vtype) { vehicleTypeSupported = vtype; }
    void setIsActive(bool active) { isActive = active; }
    void setLastMaintenanceDate(time_t date) { lastMaintenanceDate = date; }

    // Utility
    std::string getChargerTypeString() const;
    std::string getStatusString() const;
    bool isAvailable() const { return status == PointStatus::AVAILABLE && isActive; }
    double getAvailablePowerKw() const { return maxPowerKw - currentPowerKw; }
};

}

#endif // CHARGING_POINT_H