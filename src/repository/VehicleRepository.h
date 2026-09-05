#ifndef VEHICLE_REPOSITORY_H
#define VEHICLE_REPOSITORY_H

#include "../domain/Vehicle.h"
#include <vector>
#include <memory>
#include <string>

namespace evc::repository {

class VehicleRepository {
public:
    virtual ~VehicleRepository() = default;

    // Create
    virtual bool createVehicle(const evc::domain::Vehicle& vehicle) = 0;

    // Read
    virtual std::shared_ptr<evc::domain::Vehicle> getVehicleById(int vehicleId) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::Vehicle>> getVehiclesByUserId(int userId) = 0;
    virtual std::shared_ptr<evc::domain::Vehicle> getVehicleByNumber(const std::string& vehicleNumber) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::Vehicle>> getVehiclesByType(evc::domain::VehicleType type) = 0;
    virtual std::vector<std::shared_ptr<evc::domain::Vehicle>> getAllVehicles() = 0;

    // Update
    virtual bool updateVehicle(const evc::domain::Vehicle& vehicle) = 0;
    virtual bool updateBatteryStatus(int vehicleId, double currentPercent, double targetPercent) = 0;

    // Delete
    virtual bool deleteVehicle(int vehicleId) = 0;
    virtual bool deactivateVehicle(int vehicleId) = 0;

    // Check
    virtual bool vehicleExists(int vehicleId) = 0;
    virtual bool vehicleNumberExists(const std::string& vehicleNumber) = 0;
};

class SQLiteVehicleRepository : public VehicleRepository {
public:
    bool createVehicle(const evc::domain::Vehicle& vehicle) override;
    std::shared_ptr<evc::domain::Vehicle> getVehicleById(int vehicleId) override;
    std::vector<std::shared_ptr<evc::domain::Vehicle>> getVehiclesByUserId(int userId) override;
    std::shared_ptr<evc::domain::Vehicle> getVehicleByNumber(const std::string& vehicleNumber) override;
    std::vector<std::shared_ptr<evc::domain::Vehicle>> getVehiclesByType(evc::domain::VehicleType type) override;
    std::vector<std::shared_ptr<evc::domain::Vehicle>> getAllVehicles() override;
    bool updateVehicle(const evc::domain::Vehicle& vehicle) override;
    bool updateBatteryStatus(int vehicleId, double currentPercent, double targetPercent) override;
    bool deleteVehicle(int vehicleId) override;
    bool deactivateVehicle(int vehicleId) override;
    bool vehicleExists(int vehicleId) override;
    bool vehicleNumberExists(const std::string& vehicleNumber) override;

private:
    evc::domain::VehicleType stringToVehicleType(const std::string& typeStr);
};

}

#endif // VEHICLE_REPOSITORY_H