#ifndef CHARGER_REPOSITORY_H
#define CHARGER_REPOSITORY_H

#include "../domain/ChargingPoint.h"
#include "../domain/ChargingStation.h"
#include <vector>
#include <memory>
#include <string>

namespace evc::repository {

class ChargerRepository {
public:
    virtual ~ChargerRepository() = default;

    // Charging Point Operations
    virtual bool createChargingPoint(const evc::domain::ChargingPoint& point) = 0;
    virtual std::shared_ptr<evc::domain::ChargingPoint> getChargingPointById(int pointId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getChargingPointsByStation(int stationId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getAvailableChargingPoints(int stationId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getChargingPointsByType(const std::string& chargerType) = 0;
    virtual bool updateChargingPoint(const evc::domain::ChargingPoint& point) = 0;
    virtual bool updateChargingPointStatus(int pointId, evc::domain::PointStatus status) = 0;
    virtual bool updateChargingPointPower(int pointId, double currentPowerKw) = 0;
    virtual bool deleteChargingPoint(int pointId) = 0;

    // Charging Station Operations
    virtual bool createChargingStation(const evc::domain::ChargingStation& station) = 0;
    virtual std::shared_ptr<evc::domain::ChargingStation> getChargingStationById(int stationId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingStation>> getAllChargingStations() = 0;
    virtual std::vector<std::shared_ptr<evc::domain::ChargingStation>> getOperationalStations() = 0;
    virtual bool updateChargingStation(const evc::domain::ChargingStation& station) = 0;
    virtual bool updateStationPower(int stationId, double availablePowerKw) = 0;
    virtual bool deleteChargingStation(int stationId) = 0;

    // Checks
    virtual bool chargingPointExists(int pointId) = 0;
    virtual bool chargingStationExists(int stationId) = 0;
};

class SQLiteChargerRepository : public ChargerRepository {
public:
    bool createChargingPoint(const evc::domain::ChargingPoint& point) override;
    std::shared_ptr<evc::domain::ChargingPoint> getChargingPointById(int pointId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getChargingPointsByStation(int stationId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getAvailableChargingPoints(int stationId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingPoint>> getChargingPointsByType(const std::string& chargerType) override;
    bool updateChargingPoint(const evc::domain::ChargingPoint& point) override;
    bool updateChargingPointStatus(int pointId, evc::domain::PointStatus status) override;
    bool updateChargingPointPower(int pointId, double currentPowerKw) override;
    bool deleteChargingPoint(int pointId) override;

    bool createChargingStation(const evc::domain::ChargingStation& station) override;
    std::shared_ptr<evc::domain::ChargingStation> getChargingStationById(int stationId) override;
    std::vector<std::shared_ptr<evc::domain::ChargingStation>> getAllChargingStations() override;
    std::vector<std::shared_ptr<evc::domain::ChargingStation>> getOperationalStations() override;
    bool updateChargingStation(const evc::domain::ChargingStation& station) override;
    bool updateStationPower(int stationId, double availablePowerKw) override;
    bool deleteChargingStation(int stationId) override;

    bool chargingPointExists(int pointId) override;
    bool chargingStationExists(int stationId) override;

private:
    evc::domain::ChargerType stringToChargerType(const std::string& typeStr);
    evc::domain::PointStatus stringToPointStatus(const std::string& statusStr);
};

}

#endif // CHARGER_REPOSITORY_H