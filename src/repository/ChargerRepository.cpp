#include "ChargerRepository.h"
#include "../database/DatabaseManager.h"

namespace evc::repository {

evc::domain::ChargerType SQLiteChargerRepository::stringToChargerType(const std::string& typeStr) {
    if (typeStr == "2W_STANDARD") return evc::domain::ChargerType::TWO_W_STANDARD;
    if (typeStr == "3W_STANDARD") return evc::domain::ChargerType::THREE_W_STANDARD;
    if (typeStr == "4W_STANDARD") return evc::domain::ChargerType::FOUR_W_STANDARD;
    return evc::domain::ChargerType::FOUR_W_FAST;
}

evc::domain::PointStatus SQLiteChargerRepository::stringToPointStatus(const std::string& statusStr) {
    if (statusStr == "AVAILABLE") return evc::domain::PointStatus::AVAILABLE;
    if (statusStr == "OCCUPIED") return evc::domain::PointStatus::OCCUPIED;
    if (statusStr == "FAULTED") return evc::domain::PointStatus::FAULTED;
    return evc::domain::PointStatus::MAINTENANCE;
}

// Charging Point Operations

bool SQLiteChargerRepository::createChargingPoint(const evc::domain::ChargingPoint& point) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "INSERT INTO charging_points (station_id, charger_type, max_power_kw, current_power_kw, current_status, vehicle_type_supported, is_active) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        std::to_string(point.getStationId()),
        point.getChargerTypeString(),
        std::to_string(point.getMaxPowerKw()),
        std::to_string(point.getCurrentPowerKw()),
        point.getStatusString(),
        point.getVehicleTypeSupported(),
        point.getIsActive() ? "1" : "0"
    };

    return db->executeUpdateWithParams(sql, params);
}

std::shared_ptr<evc::domain::ChargingPoint> SQLiteChargerRepository::getChargingPointById(int pointId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM charging_points WHERE point_id = " + std::to_string(pointId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto point = std::make_shared<evc::domain::ChargingPoint>();
    const auto& row = results[0];
    
    point->setPointId(pointId);
    point->setStationId(std::stoi(row.at("station_id")));
    point->setChargerType(stringToChargerType(row.at("charger_type")));
    point->setMaxPowerKw(std::stod(row.at("max_power_kw")));
    point->setCurrentPowerKw(std::stod(row.at("current_power_kw")));
    point->setStatus(stringToPointStatus(row.at("current_status")));
    point->setVehicleTypeSupported(row.at("vehicle_type_supported"));
    point->setIsActive(row.at("is_active") == "1");

    return point;
}

std::vector<std::shared_ptr<evc::domain::ChargingPoint>> SQLiteChargerRepository::getChargingPointsByStation(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> points;
    
    if (!db->isDBConnected()) return points;

    std::string sql = "SELECT * FROM charging_points WHERE station_id = " + std::to_string(stationId) + " AND is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return points;
    }

    for (const auto& row : results) {
        auto point = std::make_shared<evc::domain::ChargingPoint>();
        point->setPointId(std::stoi(row.at("point_id")));
        point->setStationId(stationId);
        point->setChargerType(stringToChargerType(row.at("charger_type")));
        point->setMaxPowerKw(std::stod(row.at("max_power_kw")));
        point->setCurrentPowerKw(std::stod(row.at("current_power_kw")));
        point->setStatus(stringToPointStatus(row.at("current_status")));
        point->setVehicleTypeSupported(row.at("vehicle_type_supported"));
        point->setIsActive(row.at("is_active") == "1");
        points.push_back(point);
    }

    return points;
}

std::vector<std::shared_ptr<evc::domain::ChargingPoint>> SQLiteChargerRepository::getAvailableChargingPoints(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> points;
    
    if (!db->isDBConnected()) return points;

    std::string sql = "SELECT * FROM charging_points WHERE station_id = " + std::to_string(stationId) + 
                      " AND current_status = 'AVAILABLE' AND is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return points;
    }

    for (const auto& row : results) {
        auto point = std::make_shared<evc::domain::ChargingPoint>();
        point->setPointId(std::stoi(row.at("point_id")));
        point->setStationId(stationId);
        point->setChargerType(stringToChargerType(row.at("charger_type")));
        point->setMaxPowerKw(std::stod(row.at("max_power_kw")));
        point->setCurrentPowerKw(std::stod(row.at("current_power_kw")));
        point->setStatus(evc::domain::PointStatus::AVAILABLE);
        point->setVehicleTypeSupported(row.at("vehicle_type_supported"));
        point->setIsActive(row.at("is_active") == "1");
        points.push_back(point);
    }

    return points;
}

std::vector<std::shared_ptr<evc::domain::ChargingPoint>> SQLiteChargerRepository::getChargingPointsByType(const std::string& chargerType) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> points;
    
    if (!db->isDBConnected()) return points;

    std::string sql = "SELECT * FROM charging_points WHERE charger_type = '" + chargerType + "' AND is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return points;
    }

    for (const auto& row : results) {
        auto point = std::make_shared<evc::domain::ChargingPoint>();
        point->setPointId(std::stoi(row.at("point_id")));
        point->setStationId(std::stoi(row.at("station_id")));
        point->setChargerType(stringToChargerType(row.at("charger_type")));
        point->setMaxPowerKw(std::stod(row.at("max_power_kw")));
        point->setCurrentPowerKw(std::stod(row.at("current_power_kw")));
        point->setStatus(stringToPointStatus(row.at("current_status")));
        point->setVehicleTypeSupported(row.at("vehicle_type_supported"));
        point->setIsActive(row.at("is_active") == "1");
        points.push_back(point);
    }

    return points;
}

bool SQLiteChargerRepository::updateChargingPoint(const evc::domain::ChargingPoint& point) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_points SET charger_type = ?, max_power_kw = ?, current_power_kw = ?, current_status = ?, vehicle_type_supported = ?, is_active = ? WHERE point_id = ?";
    std::vector<std::string> params = {
        point.getChargerTypeString(),
        std::to_string(point.getMaxPowerKw()),
        std::to_string(point.getCurrentPowerKw()),
        point.getStatusString(),
        point.getVehicleTypeSupported(),
        point.getIsActive() ? "1" : "0",
        std::to_string(point.getPointId())
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteChargerRepository::updateChargingPointStatus(int pointId, evc::domain::PointStatus status) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    evc::domain::ChargingPoint tempPoint;
    tempPoint.setStatus(status);
    
    std::string sql = "UPDATE charging_points SET current_status = ? WHERE point_id = ?";
    std::vector<std::string> params = {tempPoint.getStatusString(), std::to_string(pointId)};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteChargerRepository::updateChargingPointPower(int pointId, double currentPowerKw) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_points SET current_power_kw = ? WHERE point_id = ?";
    std::vector<std::string> params = {std::to_string(currentPowerKw), std::to_string(pointId)};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteChargerRepository::deleteChargingPoint(int pointId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "DELETE FROM charging_points WHERE point_id = " + std::to_string(pointId);
    return db->executeUpdate(sql);
}

// Charging Station Operations

bool SQLiteChargerRepository::createChargingStation(const evc::domain::ChargingStation& station) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "INSERT INTO charging_stations (station_name, location, latitude, longitude, total_power_capacity_kw, available_power_kw, operator_name, is_operational) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        station.getStationName(),
        station.getLocation(),
        std::to_string(station.getLatitude()),
        std::to_string(station.getLongitude()),
        std::to_string(station.getTotalPowerCapacityKw()),
        std::to_string(station.getAvailablePowerKw()),
        station.getOperatorName(),
        station.getIsOperational() ? "1" : "0"
    };

    return db->executeUpdateWithParams(sql, params);
}

std::shared_ptr<evc::domain::ChargingStation> SQLiteChargerRepository::getChargingStationById(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM charging_stations WHERE station_id = " + std::to_string(stationId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto station = std::make_shared<evc::domain::ChargingStation>();
    const auto& row = results[0];
    
    station->setStationId(stationId);
    station->setStationName(row.at("station_name"));
    station->setLocation(row.at("location"));
    station->setLatitude(std::stod(row.at("latitude")));
    station->setLongitude(std::stod(row.at("longitude")));
    station->setTotalPowerCapacityKw(std::stod(row.at("total_power_capacity_kw")));
    station->setAvailablePowerKw(std::stod(row.at("available_power_kw")));
    station->setOperatorName(row.at("operator_name"));
    station->setIsOperational(row.at("is_operational") == "1");

    return station;
}

std::vector<std::shared_ptr<evc::domain::ChargingStation>> SQLiteChargerRepository::getAllChargingStations() {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingStation>> stations;
    
    if (!db->isDBConnected()) return stations;

    std::string sql = "SELECT * FROM charging_stations";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return stations;
    }

    for (const auto& row : results) {
        auto station = std::make_shared<evc::domain::ChargingStation>();
        station->setStationId(std::stoi(row.at("station_id")));
        station->setStationName(row.at("station_name"));
        station->setLocation(row.at("location"));
        station->setLatitude(std::stod(row.at("latitude")));
        station->setLongitude(std::stod(row.at("longitude")));
        station->setTotalPowerCapacityKw(std::stod(row.at("total_power_capacity_kw")));
        station->setAvailablePowerKw(std::stod(row.at("available_power_kw")));
        station->setOperatorName(row.at("operator_name"));
        station->setIsOperational(row.at("is_operational") == "1");
        stations.push_back(station);
    }

    return stations;
}

std::vector<std::shared_ptr<evc::domain::ChargingStation>> SQLiteChargerRepository::getOperationalStations() {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingStation>> stations;
    
    if (!db->isDBConnected()) return stations;

    std::string sql = "SELECT * FROM charging_stations WHERE is_operational = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return stations;
    }

    for (const auto& row : results) {
        auto station = std::make_shared<evc::domain::ChargingStation>();
        station->setStationId(std::stoi(row.at("station_id")));
        station->setStationName(row.at("station_name"));
        station->setLocation(row.at("location"));
        station->setLatitude(std::stod(row.at("latitude")));
        station->setLongitude(std::stod(row.at("longitude")));
        station->setTotalPowerCapacityKw(std::stod(row.at("total_power_capacity_kw")));
        station->setAvailablePowerKw(std::stod(row.at("available_power_kw")));
        station->setOperatorName(row.at("operator_name"));
        station->setIsOperational(true);
        stations.push_back(station);
    }

    return stations;
}

bool SQLiteChargerRepository::updateChargingStation(const evc::domain::ChargingStation& station) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_stations SET station_name = ?, location = ?, latitude = ?, longitude = ?, total_power_capacity_kw = ?, available_power_kw = ?, operator_name = ?, is_operational = ? WHERE station_id = ?";
    std::vector<std::string> params = {
        station.getStationName(),
        station.getLocation(),
        std::to_string(station.getLatitude()),
        std::to_string(station.getLongitude()),
        std::to_string(station.getTotalPowerCapacityKw()),
        std::to_string(station.getAvailablePowerKw()),
        station.getOperatorName(),
        station.getIsOperational() ? "1" : "0",
        std::to_string(station.getStationId())
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteChargerRepository::updateStationPower(int stationId, double availablePowerKw) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_stations SET available_power_kw = ? WHERE station_id = ?";
    std::vector<std::string> params = {std::to_string(availablePowerKw), std::to_string(stationId)};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteChargerRepository::deleteChargingStation(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "DELETE FROM charging_stations WHERE station_id = " + std::to_string(stationId);
    return db->executeUpdate(sql);
}

bool SQLiteChargerRepository::chargingPointExists(int pointId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM charging_points WHERE point_id = " + std::to_string(pointId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

bool SQLiteChargerRepository::chargingStationExists(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM charging_stations WHERE station_id = " + std::to_string(stationId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

}
