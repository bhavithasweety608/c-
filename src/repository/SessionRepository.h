#ifndef SESSION_REPOSITORY_H
#define SESSION_REPOSITORY_H

#include "../domain/ChargingSession.h"
#include <vector>
#include <memory>
#include <string>

namespace evc::repository {

class SessionRepository {
public:
    virtual ~SessionRepository() = default;

    // Create
    virtual bool createSession(const evc::domain::ChargingSession& session) = 0;

    // Read
    virtual std::shared_ptr<evc::domain::ChargingSession> getSessionById(const std::string& sessionId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByUserId(int userId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByVehicleId(int vehicleId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingSession>> getActiveSessionsByStation(int stationId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByStatus(evc::domain::SessionStatus status) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingSession>> getAllSessions() = 0;

    // Update
    virtual bool updateSession(const evc::domain::ChargingSession& session) = 0;
    virtual bool updateSessionStatus(const std::string& sessionId, evc::domain::SessionStatus status) = 0;
    virtual bool updateSessionEnergy(const std::string& sessionId, double energyDelivered) = 0;
    virtual bool updateSessionCost(const std::string& sessionId, double cost) = 0;

    // Delete
    virtual bool deleteSession(const std::string& sessionId) = 0;

    // Check
    virtual bool sessionExists(const std::string& sessionId) = 0;
};

class SQLiteSessionRepository : public SessionRepository {
public:
    bool createSession(const evc::domain::ChargingSession& session) override;
    std::shared_ptr<evc::domain::ChargingSession> getSessionById(const std::string& sessionId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByUserId(int userId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByVehicleId(int vehicleId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> getActiveSessionsByStation(int stationId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> getSessionsByStatus(evc::domain::SessionStatus status) override;
    std::vector<std::shared_ptr<evc::domain::ChargingSession>> getAllSessions() override;
    bool updateSession(const evc::domain::ChargingSession& session) override;
    bool updateSessionStatus(const std::string& sessionId, evc::domain::SessionStatus status) override;
    bool updateSessionEnergy(const std::string& sessionId, double energyDelivered) override;
    bool updateSessionCost(const std::string& sessionId, double cost) override;
    bool deleteSession(const std::string& sessionId) override;
    bool sessionExists(const std::string& sessionId) override;

private:
    evc::domain::SessionStatus stringToSessionStatus(const std::string& statusStr);
};

}

#endif // SESSION_REPOSITORY_H