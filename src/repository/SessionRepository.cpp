#include "SessionRepository.h"
#include "../database/DatabaseManager.h"

namespace evc::repository {

evc::domain::SessionStatus SQLiteSessionRepository::stringToSessionStatus(const std::string& statusStr) {
    if (statusStr == "IDLE") return evc::domain::SessionStatus::IDLE;
    if (statusStr == "CHARGING") return evc::domain::SessionStatus::CHARGING;
    if (statusStr == "PAUSED") return evc::domain::SessionStatus::PAUSED;
    if (statusStr == "COMPLETED") return evc::domain::SessionStatus::COMPLETED;
    return evc::domain::SessionStatus::ERROR;
}

bool SQLiteSessionRepository::createSession(const evc::domain::ChargingSession& session) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "INSERT INTO charging_sessions (session_id, vehicle_id, charging_point_id, user_id, status, initial_battery_percent, target_battery_percent, energy_required_kwh, estimated_charging_time_minutes, charger_efficiency_percent, is_emergency_session) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    std::vector<std::string> params = {
        session.getSessionId(),
        std::to_string(session.getVehicleId()),
        std::to_string(session.getChargingPointId()),
        std::to_string(session.getUserId()),
        session.getStatusString(),
        std::to_string(session.getInitialBatteryPercent()),
        std::to_string(session.getTargetBatteryPercent()),
        std::to_string(session.getEnergyRequiredKwh()),
        std::to_string(session.getEstimatedChargingTimeMinutes()),
        std::to_string(session.getChargerEfficiencyPercent()),
        session.getIsEmergencySession() ? "1" : "0"
    };

    return db->executeUpdateWithParams(sql, params);
}

std::shared_ptr<evc::domain::ChargingSession> SQLiteSessionRepository::getSessionById(const std::string& sessionId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return nullptr;

    std::string sql = "SELECT * FROM charging_sessions WHERE session_id = '" + sessionId + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results) || results.empty()) {
        return nullptr;
    }

    auto session = std::make_shared<evc::domain::ChargingSession>();
    const auto& row = results[0];
    
    session->setSessionId(sessionId);
    session->setVehicleId(std::stoi(row.at("vehicle_id")));
    session->setChargingPointId(std::stoi(row.at("charging_point_id")));
    session->setUserId(std::stoi(row.at("user_id")));
    session->setStatus(stringToSessionStatus(row.at("status")));
    session->setInitialBatteryPercent(std::stod(row.at("initial_battery_percent")));
    session->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
    session->setEnergyRequiredKwh(std::stod(row.at("energy_required_kwh")));
    session->setEstimatedChargingTimeMinutes(std::stoi(row.at("estimated_charging_time_minutes")));
    session->setEnergyDeliveredKwh(std::stod(row.at("energy_delivered_kwh")));
    session->setActualChargingTimeMinutes(std::stoi(row.at("actual_charging_time_minutes")));
    session->setTotalCost(std::stod(row.at("total_cost")));
    session->setChargerEfficiencyPercent(std::stod(row.at("charger_efficiency_percent")));
    session->setIsEmergencySession(row.at("is_emergency_session") == "1");

    return session;
}

std::vector<std::shared_ptr<evc::domain::ChargingSession>> SQLiteSessionRepository::getSessionsByUserId(int userId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> sessions;
    
    if (!db->isDBConnected()) return sessions;

    std::string sql = "SELECT * FROM charging_sessions WHERE user_id = " + std::to_string(userId) + " ORDER BY created_at DESC";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return sessions;
    }

    for (const auto& row : results) {
        auto session = std::make_shared<evc::domain::ChargingSession>();
        session->setSessionId(row.at("session_id"));
        session->setVehicleId(std::stoi(row.at("vehicle_id")));
        session->setChargingPointId(std::stoi(row.at("charging_point_id")));
        session->setUserId(userId);
        session->setStatus(stringToSessionStatus(row.at("status")));
        session->setInitialBatteryPercent(std::stod(row.at("initial_battery_percent")));
        session->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        session->setEnergyRequiredKwh(std::stod(row.at("energy_required_kwh")));
        session->setEstimatedChargingTimeMinutes(std::stoi(row.at("estimated_charging_time_minutes")));
        session->setEnergyDeliveredKwh(std::stod(row.at("energy_delivered_kwh")));
        session->setActualChargingTimeMinutes(std::stoi(row.at("actual_charging_time_minutes")));
        session->setTotalCost(std::stod(row.at("total_cost")));
        session->setChargerEfficiencyPercent(std::stod(row.at("charger_efficiency_percent")));
        session->setIsEmergencySession(row.at("is_emergency_session") == "1");
        sessions.push_back(session);
    }

    return sessions;
}

std::vector<std::shared_ptr<evc::domain::ChargingSession>> SQLiteSessionRepository::getSessionsByVehicleId(int vehicleId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> sessions;
    
    if (!db->isDBConnected()) return sessions;

    std::string sql = "SELECT * FROM charging_sessions WHERE vehicle_id = " + std::to_string(vehicleId) + " ORDER BY created_at DESC";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return sessions;
    }

    for (const auto& row : results) {
        auto session = std::make_shared<evc::domain::ChargingSession>();
        session->setSessionId(row.at("session_id"));
        session->setVehicleId(vehicleId);
        session->setChargingPointId(std::stoi(row.at("charging_point_id")));
        session->setUserId(std::stoi(row.at("user_id")));
        session->setStatus(stringToSessionStatus(row.at("status")));
        session->setInitialBatteryPercent(std::stod(row.at("initial_battery_percent")));
        session->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        session->setEnergyRequiredKwh(std::stod(row.at("energy_required_kwh")));
        session->setEstimatedChargingTimeMinutes(std::stoi(row.at("estimated_charging_time_minutes")));
        session->setEnergyDeliveredKwh(std::stod(row.at("energy_delivered_kwh")));
        session->setActualChargingTimeMinutes(std::stoi(row.at("actual_charging_time_minutes")));
        session->setTotalCost(std::stod(row.at("total_cost")));
        session->setChargerEfficiencyPercent(std::stod(row.at("charger_efficiency_percent")));
        session->setIsEmergencySession(row.at("is_emergency_session") == "1");
        sessions.push_back(session);
    }

    return sessions;
}

std::vector<std::shared_ptr<evc::domain::ChargingSession>> SQLiteSessionRepository::getActiveSessionsByStation(int stationId) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> sessions;
    
    if (!db->isDBConnected()) return sessions;

    std::string sql = "SELECT cs.* FROM charging_sessions cs JOIN charging_points cp ON cs.charging_point_id = cp.point_id WHERE cp.station_id = " + std::to_string(stationId) + " AND cs.status IN ('CHARGING', 'PAUSED')";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return sessions;
    }

    for (const auto& row : results) {
        auto session = std::make_shared<evc::domain::ChargingSession>();
        session->setSessionId(row.at("session_id"));
        session->setVehicleId(std::stoi(row.at("vehicle_id")));
        session->setChargingPointId(std::stoi(row.at("charging_point_id")));
        session->setUserId(std::stoi(row.at("user_id")));
        session->setStatus(stringToSessionStatus(row.at("status")));
        sessions.push_back(session);
    }

    return sessions;
}

std::vector<std::shared_ptr<evc::domain::ChargingSession>> SQLiteSessionRepository::getSessionsByStatus(evc::domain::SessionStatus status) {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> sessions;
    
    if (!db->isDBConnected()) return sessions;

    evc::domain::ChargingSession tempSession;
    tempSession.setStatus(status);
    
    std::string sql = "SELECT * FROM charging_sessions WHERE status = '" + tempSession.getStatusString() + "' ORDER BY created_at DESC";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return sessions;
    }

    for (const auto& row : results) {
        auto session = std::make_shared<evc::domain::ChargingSession>();
        session->setSessionId(row.at("session_id"));
        session->setVehicleId(std::stoi(row.at("vehicle_id")));
        session->setChargingPointId(std::stoi(row.at("charging_point_id")));
        session->setUserId(std::stoi(row.at("user_id")));
        session->setStatus(status);
        session->setInitialBatteryPercent(std::stod(row.at("initial_battery_percent")));
        session->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        session->setEnergyRequiredKwh(std::stod(row.at("energy_required_kwh")));
        session->setEstimatedChargingTimeMinutes(std::stoi(row.at("estimated_charging_time_minutes")));
        session->setEnergyDeliveredKwh(std::stod(row.at("energy_delivered_kwh")));
        session->setActualChargingTimeMinutes(std::stoi(row.at("actual_charging_time_minutes")));
        session->setTotalCost(std::stod(row.at("total_cost")));
        session->setChargerEfficiencyPercent(std::stod(row.at("charger_efficiency_percent")));
        session->setIsEmergencySession(row.at("is_emergency_session") == "1");
        sessions.push_back(session);
    }

    return sessions;
}

std::vector<std::shared_ptr<evc::domain::ChargingSession>> SQLiteSessionRepository::getAllSessions() {
    auto db = evc::database::DatabaseManager::getInstance();
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> sessions;
    
    if (!db->isDBConnected()) return sessions;

    std::string sql = "SELECT * FROM charging_sessions ORDER BY created_at DESC";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return sessions;
    }

    for (const auto& row : results) {
        auto session = std::make_shared<evc::domain::ChargingSession>();
        session->setSessionId(row.at("session_id"));
        session->setVehicleId(std::stoi(row.at("vehicle_id")));
        session->setChargingPointId(std::stoi(row.at("charging_point_id")));
        session->setUserId(std::stoi(row.at("user_id")));
        session->setStatus(stringToSessionStatus(row.at("status")));
        session->setInitialBatteryPercent(std::stod(row.at("initial_battery_percent")));
        session->setTargetBatteryPercent(std::stod(row.at("target_battery_percent")));
        session->setEnergyRequiredKwh(std::stod(row.at("energy_required_kwh")));
        session->setEstimatedChargingTimeMinutes(std::stoi(row.at("estimated_charging_time_minutes")));
        session->setEnergyDeliveredKwh(std::stod(row.at("energy_delivered_kwh")));
        session->setActualChargingTimeMinutes(std::stoi(row.at("actual_charging_time_minutes")));
        session->setTotalCost(std::stod(row.at("total_cost")));
        session->setChargerEfficiencyPercent(std::stod(row.at("charger_efficiency_percent")));
        session->setIsEmergencySession(row.at("is_emergency_session") == "1");
        sessions.push_back(session);
    }

    return sessions;
}

bool SQLiteSessionRepository::updateSession(const evc::domain::ChargingSession& session) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_sessions SET vehicle_id = ?, charging_point_id = ?, user_id = ?, status = ?, energy_delivered_kwh = ?, actual_charging_time_minutes = ?, total_cost = ? WHERE session_id = ?";
    std::vector<std::string> params = {
        std::to_string(session.getVehicleId()),
        std::to_string(session.getChargingPointId()),
        std::to_string(session.getUserId()),
        session.getStatusString(),
        std::to_string(session.getEnergyDeliveredKwh()),
        std::to_string(session.getActualChargingTimeMinutes()),
        std::to_string(session.getTotalCost()),
        session.getSessionId()
    };

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteSessionRepository::updateSessionStatus(const std::string& sessionId, evc::domain::SessionStatus status) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    evc::domain::ChargingSession tempSession;
    tempSession.setStatus(status);
    
    std::string sql = "UPDATE charging_sessions SET status = ? WHERE session_id = ?";
    std::vector<std::string> params = {tempSession.getStatusString(), sessionId};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteSessionRepository::updateSessionEnergy(const std::string& sessionId, double energyDelivered) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_sessions SET energy_delivered_kwh = ? WHERE session_id = ?";
    std::vector<std::string> params = {std::to_string(energyDelivered), sessionId};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteSessionRepository::updateSessionCost(const std::string& sessionId, double cost) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "UPDATE charging_sessions SET total_cost = ? WHERE session_id = ?";
    std::vector<std::string> params = {std::to_string(cost), sessionId};

    return db->executeUpdateWithParams(sql, params);
}

bool SQLiteSessionRepository::deleteSession(const std::string& sessionId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "DELETE FROM charging_sessions WHERE session_id = '" + sessionId + "'";
    return db->executeUpdate(sql);
}

bool SQLiteSessionRepository::sessionExists(const std::string& sessionId) {
    auto db = evc::database::DatabaseManager::getInstance();
    if (!db->isDBConnected()) return false;

    std::string sql = "SELECT COUNT(*) as count FROM charging_sessions WHERE session_id = '" + sessionId + "'";
    std::vector<std::map<std::string, std::string>> results;
    
    if (!db->executeQuery(sql, results)) {
        return false;
    }

    return std::stoi(results[0].at("count")) > 0;
}

}
