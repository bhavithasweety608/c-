#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class VehicleType {
    TWO_WHEELER,    // 2W
    THREE_WHEELER,  // 3W
    FOUR_WHEELER    // 4W
};

class Vehicle {
private:
    int vehicleId;
    int userId;
    std::string vehicleNumber;
    VehicleType vehicleType;
    std::string vehicleName;
    std::string manufacturer;
    std::string model;
    double batteryCapacityKwh;
    double currentBatteryPercent;
    double targetBatteryPercent;
    time_t registrationDate;
    bool isActive;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    Vehicle();
    Vehicle(int id, int uid, const std::string& vnum, VehicleType vtype, 
            double bCapacity);

    // Getters
    int getVehicleId() const { return vehicleId; }
    int getUserId() const { return userId; }
    const std::string& getVehicleNumber() const { return vehicleNumber; }
    VehicleType getVehicleType() const { return vehicleType; }
    const std::string& getVehicleName() const { return vehicleName; }
    const std::string& getManufacturer() const { return manufacturer; }
    const std::string& getModel() const { return model; }
    double getBatteryCapacityKwh() const { return batteryCapacityKwh; }
    double getCurrentBatteryPercent() const { return currentBatteryPercent; }
    double getTargetBatteryPercent() const { return targetBatteryPercent; }
    bool getIsActive() const { return isActive; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setVehicleId(int id) { vehicleId = id; }
    void setUserId(int uid) { userId = uid; }
    void setVehicleNumber(const std::string& vnum) { vehicleNumber = vnum; }
    void setVehicleType(VehicleType vtype) { vehicleType = vtype; }
    void setVehicleName(const std::string& name) { vehicleName = name; }
    void setManufacturer(const std::string& mfg) { manufacturer = mfg; }
    void setModel(const std::string& m) { model = m; }
    void setBatteryCapacityKwh(double cap) { batteryCapacityKwh = cap; }
    void setCurrentBatteryPercent(double percent) { currentBatteryPercent = percent; }
    void setTargetBatteryPercent(double percent) { targetBatteryPercent = percent; }
    void setIsActive(bool active) { isActive = active; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    double getEnergyRequiredKwh() const {
        return batteryCapacityKwh * (targetBatteryPercent - currentBatteryPercent) / 100.0;
    }
    std::string getVehicleTypeString() const;
    bool isCompatible(const std::string& chargerType) const;
};

}

#endif // VEHICLE_H