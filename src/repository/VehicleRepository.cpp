#include "VehicleRepository.h"
#include "../database/DatabaseManager.h"

namespace evc::repository {

evc::domain::VehicleType SQLiteVehicleRepository::stringToVehicleType(const std::string& typeStr) {
    if (typeStr == "2W") return evc::domain::VehicleType::TWO_WHEELER;
    if (typeStr == "3W") return evc::domain::VehicleType::THREE_WHEELER;
    return evc::domain::VehicleType::FOUR_WHEELER;
}

bool SQLiteVehicleRepository::createVehicle(const evc::domain::Vehicle& vehicle) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "INSERT INTO vehicles (user_id, vehicle_number, vehicle_type, vehicle_name, manufacturer, model, battery_capacity_kwh, current_battery_percent, target_battery_percent, is_active) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        std::to_string(vehicle.getUserId()),
        vehicle.getVehicleNumber(),
        vehicle.getVehicleTypeString(),
        vehicle.getVehicleName(),
        vehicle.getManufacturer(),
        vehicle.getModel(),
        std::to_string(vehicle.getBatteryCapacityKwh()),
        std::to_string(vehicle.getCurrentBatteryPercent()),
        std::to_string(vehicle.getTargetBatteryPercent()),
        vehicle.getIsActive() ? "1" : "0"
    };

    return db->executeUpdateWithParams(sql, params);
}

std::shared_ptr<evc::domain::Vehicle> SQLiteVehicleRepository::getVehicleById(int vehicleId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM vehicles WHERE vehicle_id = " + std::to_string(vehicleId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto vehicle = std::make_shared<evc::domain::Vehicle>();
    const auto& row = results[0];
    
    vehicle->setVehicleId(vehicleId);
    vehicle->setUserId(std::stoi(row.at("user_id")));
    vehicle->setVehicleNumber(row.at("vehicle_number"));
    vehicle->setVehicleType(stringToVehicleType(row.at("vehicle_type")));
    vehicle->setVehicleName(row.at("vehicle_name"));
    vehicle->setManufacturer(row.at("manufacturer"));
    vehicle->setModel(row.at("model"));
    vehicle->setBatteryCapacityKwh(std::stod(row.at("battery_capacity_kwh")));
    vehicle->setCurrentBatteryPercent(std::stod(row.at("current_battery_percent")));
    vehicle->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
    vehicle->setIsActive(row.at("is_active") == "1");

    return vehicle;
}

std::vector<std::shared_ptr<evc::domain::Vehicle>> SQLiteVehicleRepository::getVehiclesByUserId(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::Vehicle>> vehicles;
    
    if (!db->isDBConnected()) return vehicles;

    std::string sql = "SELECT * FROM vehicles WHERE user_id = " + std::to_string(userId) + " AND is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return vehicles;
    }

    for (const auto& row : results) {
        auto vehicle = std::make_shared<evc::domain::Vehicle>();
        vehicle->setVehicleId(std::stoi(row.at("vehicle_id")));
        vehicle->setUserId(userId);
        vehicle->setVehicleNumber(row.at("vehicle_number"));
        vehicle->setVehicleType(stringToVehicleType(row.at("vehicle_type")));
        vehicle->setVehicleName(row.at("vehicle_name"));
        vehicle->setManufacturer(row.at("manufacturer"));
        vehicle->setModel(row.at("model"));
        vehicle->setBatteryCapacityKwh(std::stod(row.at("battery_capacity_kwh")));
        vehicle->setCurrentBatteryPercent(std::stod(row.at("current_battery_percent")));
        vehicle->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        vehicle->setIsActive(row.at("is_active") == "1");
        vehicles.push_back(vehicle);
    }

    return vehicles;
}

std::shared_ptr<evc::domain::Vehicle> SQLiteVehicleRepository::getVehicleByNumber(const std::string& vehicleNumber) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM vehicles WHERE vehicle_number = '" + vehicleNumber + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto vehicle = std::make_shared<evc::domain::Vehicle>();
    const auto& row = results[0];
    
    vehicle->setVehicleId(std::stoi(row.at("vehicle_id")));
    vehicle->setUserId(std::stoi(row.at("user_id")));
    vehicle->setVehicleNumber(row.at("vehicle_number"));
    vehicle->setVehicleType(stringToVehicleType(row.at("vehicle_type")));
    vehicle->setVehicleName(row.at("vehicle_name"));
    vehicle->setManufacturer(row.at("manufacturer"));
    vehicle->setModel(row.at("model"));
    vehicle->setBatteryCapacityKwh(std::stod(row.at("battery_capacity_kwh")));
    vehicle->setCurrentBatteryPercent(std::stod(row.at("current_battery_percent")));
    vehicle->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
    vehicle->setIsActive(row.at("is_active") == "1");

    return vehicle;
}

std::vector<std::shared_ptr<evc::domain::Vehicle>> SQLiteVehicleRepository::getVehiclesByType(evc::domain::VehicleType type) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::Vehicle>> vehicles;
    
    if (!db->isDBConnected()) return vehicles;

    evc::domain::Vehicle tempVehicle;
    tempVehicle.setVehicleType(type);
    
    std::string sql = "SELECT * FROM vehicles WHERE vehicle_type = '" + tempVehicle.getVehicleTypeString() + "' AND is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return vehicles;
    }

    for (const auto& row : results) {
        auto vehicle = std::make_shared<evc::domain::Vehicle>();
        vehicle->setVehicleId(std::stoi(row.at("vehicle_id")));
        vehicle->setUserId(std::stoi(row.at("user_id")));
        vehicle->setVehicleNumber(row.at("vehicle_number"));
        vehicle->setVehicleType(type);
        vehicle->setVehicleName(row.at("vehicle_name"));
        vehicle->setManufacturer(row.at("manufacturer"));
        vehicle->setModel(row.at("model"));
        vehicle->setBatteryCapacityKwh(std::stod(row.at("battery_capacity_kwh")));
        vehicle->setCurrentBatteryPercent(std::stod(row.at("current_battery_percent")));
        vehicle->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        vehicle->setIsActive(row.at("is_active") == "1");
        vehicles.push_back(vehicle);
    }

    return vehicles;
}

std::vector<std::shared_ptr<evc::domain::Vehicle>> SQLiteVehicleRepository::getAllVehicles() {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::Vehicle>> vehicles;
    
    if (!db->isDBConnected()) return vehicles;

    std::string sql = "SELECT * FROM vehicles WHERE is_active = 1";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return vehicles;
    }

    for (const auto& row : results) {
        auto vehicle = std::make_shared<evc::domain::Vehicle>();
        vehicle->setVehicleId(std::stoi(row.at("vehicle_id")));
        vehicle->setUserId(std::stoi(row.at("user_id")));
        vehicle->setVehicleNumber(row.at("vehicle_number"));
        vehicle->setVehicleType(stringToVehicleType(row.at("vehicle_type")));
        vehicle->setVehicleName(row.at("vehicle_name"));
        vehicle->setManufacturer(row.at("manufacturer"));
        vehicle->setModel(row.at("model"));
        vehicle->setBatteryCapacityKwh(std::stod(row.at("battery_capacity_kwh")));
        vehicle->setCurrentBatteryPercent(std::stod(row.at("current_battery_percent")));
        vehicle->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        vehicle->setIsActive(row.at("is_active") == "1");
        vehicles.push_back(vehicle);
    }

    return vehicles;
}

bool SQLiteVehicleRepository::updateVehicle(const evc::domain::Vehicle& vehicle) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE vehicles SET user_id = ?, vehicle_number = ?, vehicle_type = ?, vehicle_name = ?, manufacturer = ?, model = ?, battery_capacity_kwh = ?, current_battery_percent = ?, target_battery_percent = ?, is_active = ? WHERE vehicle_id = ?";
    std::vector<std::string> params = {
        std::to_string(vehicle.getUserId()),
        vehicle.getVehicleNumber(),
        vehicle.getVehicleTypeString(),
        vehicle.getVehicleName(),
        vehicle.getManufacturer(),
        vehicle.getModel(),
        std::to_string(vehicle.getBatteryCapacityKwh()),
        std::to_string(vehicle.getCurrentBatteryPercent()),
        std::to_string(vehicle.getTargetBatteryPercent()),
        vehicle.getIsActive() ? "1" : "0",
        std::to_string(vehicle.getVehicleId())
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteVehicleRepository::updateBatteryStatus(int vehicleId, double currentPercent, double targetPercent) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE vehicles SET current_battery_percent = ?, target_battery_percent = ? WHERE vehicle_id = ?";
    std::vector<std::string> params = {
        std::to_string(currentPercent),
        std::to_string(targetPercent),
        std::to_string(vehicleId)
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteVehicleRepository::deleteVehicle(int vehicleId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "DELETE FROM vehicles WHERE vehicle_id = " + std::to_string(vehicleId);
    return db->executeUpdate(sql);
}

bool SQLiteVehicleRepository::deactivateVehicle(int vehicleId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE vehicles SET is_active = 0 WHERE vehicle_id = " + std::to_string(vehicleId);
    return db->executeUpdate(sql);
}

bool SQLiteVehicleRepository::vehicleExists(int vehicleId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM vehicles WHERE vehicle_id = " + std::to_string(vehicleId);
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

bool SQLiteVehicleRepository::vehicleNumberExists(const std::string& vehicleNumber) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM vehicles WHERE vehicle_number = '" + vehicleNumber + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

}
