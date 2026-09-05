#ifndef CHARGING_STATION_H
#define CHARGING_STATION_H

#include <string>
#include <ctime>
#include <vector>

namespace evc::domain {

class ChargingStation {
private:
    int stationId;
    std::string stationName;
    std::string location;
    double latitude;
    double longitude;
    double totalPowerCapacityKw;
    double availablePowerKw;
    std::string operatorName;
    bool isOperational;
    int totalChargingPoints;
    int availableChargingPoints;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    ChargingStation();
    ChargingStation(int id, const std::string& name, const std::string& loc);

    // Getters
    int getStationId() const { return stationId; }
    const std::string& getStationName() const { return stationName; }
    const std::string& getLocation() const { return location; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    double getTotalPowerCapacityKw() const { return totalPowerCapacityKw; }
    double getAvailablePowerKw() const { return availablePowerKw; }
    const std::string& getOperatorName() const { return operatorName; }
    bool getIsOperational() const { return isOperational; }
    int getTotalChargingPoints() const { return totalChargingPoints; }
    int getAvailableChargingPoints() const { return availableChargingPoints; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setStationId(int id) { stationId = id; }
    void setStationName(const std::string& name) { stationName = name; }
    void setLocation(const std::string& loc) { location = loc; }
    void setLatitude(double lat) { latitude = lat; }
    void setLongitude(double lon) { longitude = lon; }
    void setTotalPowerCapacityKw(double power) { totalPowerCapacityKw = power; }
    void setAvailablePowerKw(double power) { availablePowerKw = power; }
    void setOperatorName(const std::string& op) { operatorName = op; }
    void setIsOperational(bool op) { isOperational = op; updatedAt = time(nullptr); }
    void setTotalChargingPoints(int count) { totalChargingPoints = count; }
    void setAvailableChargingPoints(int count) { availableChargingPoints = count; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    double getUtilizationPercent() const;
    bool hasSufficientPower(double requiredPowerKw) const;
    bool isNearby(double userLat, double userLon, double radiusKm) const;
    void allocatePower(double powerKw);
    void releasePower(double powerKw);
};

}

#endif // CHARGING_STATION_H