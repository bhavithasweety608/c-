#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <ctime>

namespace evc::domain {

enum class ReservationStatus {
    CONFIRMED,
    CANCELLED,
    COMPLETED,
    EXPIRED
};

class Reservation {
private:
    std::string reservationId;
    int userId;
    int vehicleId;
    int stationId;
    std::string chargerType;
    time_t reservedDate;
    time_t startTime;
    time_t endTime;
    ReservationStatus status;
    std::string confirmationCode;
    time_t createdAt;
    time_t updatedAt;

public:
    // Constructor
    Reservation();
    Reservation(const std::string& rId, int uId, int vId, int sId);

    // Getters
    const std::string& getReservationId() const { return reservationId; }
    int getUserId() const { return userId; }
    int getVehicleId() const { return vehicleId; }
    int getStationId() const { return stationId; }
    const std::string& getChargerType() const { return chargerType; }
    time_t getReservedDate() const { return reservedDate; }
    time_t getStartTime() const { return startTime; }
    time_t getEndTime() const { return endTime; }
    ReservationStatus getStatus() const { return status; }
    const std::string& getConfirmationCode() const { return confirmationCode; }
    time_t getCreatedAt() const { return createdAt; }
    time_t getUpdatedAt() const { return updatedAt; }

    // Setters
    void setReservationId(const std::string& id) { reservationId = id; }
    void setUserId(int uId) { userId = uId; }
    void setVehicleId(int vId) { vehicleId = vId; }
    void setStationId(int sId) { stationId = sId; }
    void setChargerType(const std::string& ctype) { chargerType = ctype; }
    void setReservedDate(time_t date) { reservedDate = date; }
    void setStartTime(time_t time) { startTime = time; }
    void setEndTime(time_t time) { endTime = time; }
    void setStatus(ReservationStatus s) { status = s; updatedAt = time(nullptr); }
    void setConfirmationCode(const std::string& code) { confirmationCode = code; }
    void setUpdatedAt(time_t time) { updatedAt = time; }

    // Utility
    std::string getStatusString() const;
    bool isActive() const { return status == ReservationStatus::CONFIRMED; }
    bool isExpired() const;
    int getDurationMinutes() const;
};

}

#endif // RESERVATION_H