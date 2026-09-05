#include "ChargingPoint.h"
#include <ctime>

namespace evc::domain {

ChargingPoint::ChargingPoint() : pointId(0), stationId(0), chargerType(ChargerType::TWO_W_STANDARD),
                                 maxPowerKw(0), currentPowerKw(0), status(PointStatus::AVAILABLE),
                                 vehicleTypeSupported("2W"), isActive(true) {
    creationDate = time(nullptr);
    lastMaintenanceDate = time(nullptr);
}

ChargingPoint::ChargingPoint(int id, int stId, ChargerType ctype, double power)
    : pointId(id), stationId(stId), chargerType(ctype), maxPowerKw(power),
      currentPowerKw(0), status(PointStatus::AVAILABLE), isActive(true) {
    creationDate = time(nullptr);
    lastMaintenanceDate = time(nullptr);
    
    // Set vehicle type based on charger type
    switch (ctype) {
        case ChargerType::TWO_W_STANDARD:
            vehicleTypeSupported = "2W";
            break;
        case ChargerType::THREE_W_STANDARD:
            vehicleTypeSupported = "3W";
            break;
        case ChargerType::FOUR_W_STANDARD:
        case ChargerType::FOUR_W_FAST:
            vehicleTypeSupported = "4W";
            break;
    }
}

std::string ChargingPoint::getChargerTypeString() const {
    switch (chargerType) {
        case ChargerType::TWO_W_STANDARD:
            return "2W_STANDARD";
        case ChargerType::THREE_W_STANDARD:
            return "3W_STANDARD";
        case ChargerType::FOUR_W_STANDARD:
            return "4W_STANDARD";
        case ChargerType::FOUR_W_FAST:
            return "4W_FAST";
        default:
            return "UNKNOWN";
    }
}

std::string ChargingPoint::getStatusString() const {
    switch (status) {
        case PointStatus::AVAILABLE:
            return "AVAILABLE";
        case PointStatus::OCCUPIED:
            return "OCCUPIED";
        case PointStatus::FAULTED:
            return "FAULTED";
        case PointStatus::MAINTENANCE:
            return "MAINTENANCE";
        default:
            return "UNKNOWN";
    }
}

}
