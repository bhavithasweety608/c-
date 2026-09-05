#include "Vehicle.h"
#include <ctime>

namespace evc::domain {

Vehicle::Vehicle() : vehicleId(0), userId(0), vehicleType(VehicleType::TWO_WHEELER),
                     batteryCapacityKwh(0), currentBatteryPercent(0),
                     targetBatteryPercent(100), isActive(true) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

Vehicle::Vehicle(int id, int uid, const std::string& vnum, VehicleType vtype, 
                 double bCapacity)
    : vehicleId(id), userId(uid), vehicleNumber(vnum), vehicleType(vtype),
      batteryCapacityKwh(bCapacity), currentBatteryPercent(0),
      targetBatteryPercent(100), isActive(true) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

std::string Vehicle::getVehicleTypeString() const {
    switch (vehicleType) {
        case VehicleType::TWO_WHEELER:
            return "2W";
        case VehicleType::THREE_WHEELER:
            return "3W";
        case VehicleType::FOUR_WHEELER:
            return "4W";
        default:
            return "UNKNOWN";
    }
}

bool Vehicle::isCompatible(const std::string& chargerType) const {
    if (chargerType == "2W_STANDARD" && vehicleType == VehicleType::TWO_WHEELER)
        return true;
    if (chargerType == "3W_STANDARD" && vehicleType == VehicleType::THREE_WHEELER)
        return true;
    if ((chargerType == "4W_STANDARD" || chargerType == "4W_FAST") && 
        vehicleType == VehicleType::FOUR_WHEELER)
        return true;
    return false;
}

}
