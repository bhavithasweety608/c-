#include "ChargingStation.h"
#include <ctime>
#include <cmath>

namespace evc::domain {

ChargingStation::ChargingStation() : stationId(0), latitude(0), longitude(0),
                                     totalPowerCapacityKw(0), availablePowerKw(0),
                                     isOperational(true), totalChargingPoints(0),
                                     availableChargingPoints(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

ChargingStation::ChargingStation(int id, const std::string& name, const std::string& loc)
    : stationId(id), stationName(name), location(loc),
      latitude(0), longitude(0),
      totalPowerCapacityKw(0), availablePowerKw(0),
      isOperational(true), totalChargingPoints(0),
      availableChargingPoints(0) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

double ChargingStation::getUtilizationPercent() const {
    if (totalPowerCapacityKw == 0) return 0;
    double usedPower = totalPowerCapacityKw - availablePowerKw;
    return (usedPower / totalPowerCapacityKw) * 100.0;
}

bool ChargingStation::hasSufficientPower(double requiredPowerKw) const {
    return availablePowerKw >= requiredPowerKw && isOperational;
}

bool ChargingStation::isNearby(double userLat, double userLon, double radiusKm) const {
    // Haversine formula for distance calculation
    const double R = 6371.0;  // Earth's radius in km
    const double PI = 3.14159265359;
    
    double lat1 = latitude * PI / 180.0;
    double lat2 = userLat * PI / 180.0;
    double deltaLat = (userLat - latitude) * PI / 180.0;
    double deltaLon = (userLon - longitude) * PI / 180.0;
    
    double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
               cos(lat1) * cos(lat2) * sin(deltaLon / 2) * sin(deltaLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double distance = R * c;
    
    return distance <= radiusKm;
}

void ChargingStation::allocatePower(double powerKw) {
    if (powerKw > 0 && powerKw <= availablePowerKw) {
        availablePowerKw -= powerKw;
        updatedAt = time(nullptr);
    }
}

void ChargingStation::releasePower(double powerKw) {
    if (powerKw > 0) {
        availablePowerKw += powerKw;
        if (availablePowerKw > totalPowerCapacityKw) {
            availablePowerKw = totalPowerCapacityKw;
        }
        updatedAt = time(nullptr);
    }
}

}
