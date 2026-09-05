#include "Reservation.h"
#include <ctime>

namespace evc::domain {

Reservation::Reservation() : userId(0), vehicleId(0), stationId(0),
                             reservedDate(0), startTime(0), endTime(0),
                             status(ReservationStatus::CONFIRMED) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

Reservation::Reservation(const std::string& rId, int uId, int vId, int sId)
    : reservationId(rId), userId(uId), vehicleId(vId), stationId(sId),
      reservedDate(0), startTime(0), endTime(0),
      status(ReservationStatus::CONFIRMED) {
    createdAt = time(nullptr);
    updatedAt = time(nullptr);
}

std::string Reservation::getStatusString() const {
    switch (status) {
        case ReservationStatus::CONFIRMED:
            return "CONFIRMED";
        case ReservationStatus::CANCELLED:
            return "CANCELLED";
        case ReservationStatus::COMPLETED:
            return "COMPLETED";
        case ReservationStatus::EXPIRED:
            return "EXPIRED";
        default:
            return "UNKNOWN";
    }
}

bool Reservation::isExpired() const {
    time_t now = time(nullptr);
    return endTime > 0 && now > endTime;
}

int Reservation::getDurationMinutes() const {
    if (startTime == 0 || endTime == 0) return 0;
    return static_cast<int>((endTime - startTime) / 60);
}

}
